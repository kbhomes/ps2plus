#include "file-dialog.h"

#include "components/footer-command-menu.h"
#include "ui/fonts/playstation.h"

#define IMGUI_DISABLE_SSE
#include <imgui/imgui_internal.h>

#include <iostream>
#include <map>
#include <memory>
#include <sstream>
#include <string>
#include <sys/dir.h>

#define PATH_SEP '/'
#define MAX_PATH 255

namespace PS2Plus::Components {
template <typename... Args> std::string stringformat(const std::string& format, Args... args) {
  int size_s = std::snprintf(nullptr, 0, format.c_str(), args...) + 1;
  auto size = static_cast<size_t>(size_s);
  std::unique_ptr<char[]> buf(new char[size]);
  std::snprintf(buf.get(), size, format.c_str(), args...);
  return std::string(buf.get(), buf.get() + size - 1);
}

std::string friendlytime(struct timespec ts) {
  char buff[100];
  strftime(buff, sizeof buff, "%D %T", gmtime(&ts.tv_sec));
  return buff;
}

std::string friendlysize(unsigned long bytes) {
  if (bytes < 1024) {
    return stringformat("%d B", bytes);
  } else if (bytes < (1024 * 1024)) {
    return stringformat("%.1f KB", bytes / 1024.f);
  } else {
    return stringformat("%.1f MB", bytes / 1024.f / 1024.f);
  }
}

std::vector<std::string> split(const std::string& s, char delim) {
  std::stringstream ss(s);
  std::string item;
  std::vector<std::string> elems;
  while (std::getline(ss, item, delim)) {
    elems.push_back(std::move(item));
  }
  return elems;
}

std::string join(const std::vector<std::string>& lst, const std::string& delim) {
  std::string ret;
  for (const auto& s : lst) {
    if (!ret.empty())
      ret += delim;
    ret += s;
  }
  return ret;
}

std::string joinpath(const std::string& a, const std::string& b) {
  if (a.empty()) {
    return b;
  } else if (a.back() == PATH_SEP) {
    return a + b;
  } else {
    return a + PATH_SEP + b;
  }
}

inline int diralphasort(const struct dirent **a, const struct dirent **b) {
  bool a_dir = S_ISDIR((*a)->d_stat.st_mode);
  bool b_dir = S_ISDIR((*b)->d_stat.st_mode);

  if (a_dir != b_dir) {
    return a_dir ? -1 : 1;
  } else {
    return strcoll((*a)->d_name, (*b)->d_name);
  }
}

int scandirx(const char *dirname, struct dirent ***namelist, int (*filter)(const struct dirent *),
             int (*compare)(const struct dirent **, const struct dirent **)) {
  struct dirent **files = NULL;
  size_t size = 0;
  size_t allocated = 0;
  const size_t init_size = 1;
  DIR *dir = NULL;
  struct dirent *entry;
  struct dirent *tmp = NULL;
  size_t i;
  int result = 0;

  /* Open directory stream */
  dir = opendir(dirname);
  if (dir) {

    /* Read directory entries to memory */
    while (1) {

      /* Enlarge pointer table to make room for another pointer */
      if (size >= allocated) {
        void *p;
        size_t num_entries;

        /* Compute number of entries in the enlarged pointer table */
        if (size < init_size) {
          /* Allocate initial pointer table */
          num_entries = init_size;
        } else {
          /* Double the size */
          num_entries = size * 2;
        }

        /* Allocate first pointer table or enlarge existing table */
        p = realloc(files, sizeof(void *) * num_entries);
        if (p != NULL) {
          /* Got the memory */
          files = (dirent **)p;
          allocated = num_entries;
        } else {
          /* Out of memory */
          result = -1;
          break;
        }
      }

      /* Allocate room for temporary directory entry */
      if (tmp == NULL) {
        tmp = (struct dirent *)malloc(sizeof(struct dirent));
        if (tmp == NULL) {
          /* Cannot allocate temporary directory entry */
          result = -1;
          break;
        }
      }

      /* Read directory entry to temporary area */
      entry = readdir(dir);
      if (entry != NULL) {
        int pass;

        /* Determine whether to include the entry in result */
        if (filter) {
          /* Let the filter function decide */
          pass = filter(entry);
        } else {
          /* No filter function, include everything */
          pass = 1;
        }

        if (pass) {
          /* Copy the entry into a temporary space to store in the pointer table */
          memcpy(tmp, entry, sizeof(struct dirent));

          /* Store the temporary entry to pointer table */
          files[size++] = tmp;
          tmp = NULL;

          /* Keep up with the number of files */
          result++;
        }

      } else {

        /*
         * End of directory stream reached => sort entries and
         * exit.
         */
        qsort(files, size, sizeof(void *), (int (*)(const void *, const void *))compare);
        break;
      }
    }

  } else {
    /* Cannot open directory */
    result = /*Error*/ -1;
  }

  /* Release temporary directory entry */
  free(tmp);

  /* Release allocated memory on error */
  if (result < 0) {
    for (i = 0; i < size; i++) {
      free(files[i]);
    }
    free(files);
    files = NULL;
  }

  /* Close directory stream */
  if (dir) {
    closedir(dir);
  }

  /* Pass pointer table to caller */
  if (namelist) {
    *namelist = files;
  }
  return result;
}

std::map<const char *, FileDialog_ *> dialogs;

FileDialog_::FileDialog_(std::string initialDirectory, bool dironly)
    : _currentDirectoryComponents(), _currentEntries(), _selectedPath(), _deviceList(), _dironly(dironly) {
  _opened = false;
  _initialDirectory = initialDirectory;
  _currentDirectory = initialDirectory;
}

void FileDialog_::ScanDirectory(std::string directory) {
  struct dirent **namelist;
  int n;
  int i = 0;

  // For directory-mode dialogs, always use this directory as the selected path
  if (_dironly) {
    SetSelectedPath(directory);
  }

  n = scandirx(directory.c_str(), &namelist, NULL, diralphasort);

  if (n < 0) {
    printf("[PS2Plus::Components::FileDialog] scandir failed for directory: %s\n", directory.c_str());
  } else {
    // The path exists, so load it in the dialog
    _recentlyChanged = true;
    _showingDeviceList = false;
    _currentDirectory = directory;
    _currentEntries.clear();
    _currentDirectoryComponents.clear();

    // Parse directory components
    _currentDirectoryComponents = split(directory, PATH_SEP);

    // Loop through each entry
    while (i < n) {
      struct dirent *entry = namelist[i];
      std::string name = entry->d_name;
      EntryType type = S_ISDIR(entry->d_stat.st_mode) ? EntryTypeDirectory : EntryTypeFile;
      size_t size = S_ISREG(entry->d_stat.st_mode) ? entry->d_stat.st_size : 0;
      struct timespec modified = entry->d_stat.st_mtim;

      if (!_dironly || (_dironly && type == EntryTypeDirectory)) {
        _currentEntries.push_back({name, type, size, modified});
      }

      free(namelist[i++]);
    }

    free(namelist);
  }
}

void FileDialog_::ScanSubdirectory(std::string directory) { ScanDirectory(joinpath(_currentDirectory, directory)); }

void FileDialog_::ScanParentDirectory() { ScanDirectory(_currentDirectory.substr(0, _currentDirectory.find_last_of('/'))); }

void FileDialog_::ShowDeviceList() {
  _recentlyChanged = true;
  _showingDeviceList = true;
  _currentDirectory = "";
  _currentEntries.clear();
  _currentDirectoryComponents.clear();

  for (auto device : _deviceList) {
    _currentEntries.push_back({device, EntryTypeDirectory, 0});
  }
}

void FileDialog_::SetDeviceList(std::vector<std::string> deviceList) { _deviceList = deviceList; }

std::vector<std::string> FileDialog_::GetDeviceList() { return _deviceList; }

std::vector<FileDialog_::Entry> FileDialog_::GetEntries() { return _currentEntries; }

std::vector<std::string> FileDialog_::GetDirectoryComponents() { return _currentDirectoryComponents; }

std::string FileDialog_::GetCurrentDirectory() { return _currentDirectory; }

std::string FileDialog_::GetSelectedPath() { return _selectedPath; }

void FileDialog_::SetSelectedPath(std::string path) { _selectedPath = path; }

bool FileDialog_::IsShowingDeviceList() { return _showingDeviceList; }

bool FileDialog_::HasRecentlyChanged() {
  bool value = _recentlyChanged;
  _recentlyChanged = false;
  return value;
}

bool FileDialog(const char *key, const char *initialDirectory, std::vector<std::string> deviceList, char *selectedFile, bool dironly) {
  FileDialog_ *dialog;

  auto result = dialogs.find(key);
  if (result != dialogs.end()) {
    dialog = result->second;
  } else {
    dialog = new FileDialog_(initialDirectory ? initialDirectory : "", dironly);
    dialog->SetDeviceList(deviceList);

    if (initialDirectory == NULL) {
      dialog->ShowDeviceList();
    } else {
      dialog->ScanDirectory(initialDirectory);
    }
    dialogs.insert({key, dialog});
  }

  bool recentlyChanged = dialog->HasRecentlyChanged();

  {
    if (!dialog->GetDeviceList().empty()) {
      if (ImGui::SmallButton("Devices")) {
        dialog->ShowDeviceList();
      }

      if (!dialog->GetDirectoryComponents().empty()) {
        ImGui::SameLine(0, 2.0f);
        ImGui::TextColored(ImVec4(0.5f, 0.5f, 0.5f, 0.5f), "|");
        ImGui::SameLine(0, 2.0f);
      }
    }

    std::vector<std::string> directoryComponents = dialog->GetDirectoryComponents();
    float windowMaxContentX = ImGui::GetWindowPos().x + ImGui::GetWindowContentRegionMax().x;
    auto collapsedComponentIterator = directoryComponents.end();

    for (auto component = directoryComponents.begin(); component != directoryComponents.end(); ++component) {
      if (ImGui::SmallButton(component->c_str())) {
        // Set the new directory to the path of all the previous components
        dialog->ScanDirectory(join(std::vector<std::string>(directoryComponents.begin(), component + 1), std::string(1, PATH_SEP)));
      }

      if (component + 1 != directoryComponents.end()) {
        ImGui::SameLine(0, 2.0f);

        float lastComponentMaxX = ImGui::GetItemRectMax().x;
        auto nextComponent = component + 1;
        float nextComponentWidth = ImGui::CalcTextSize(nextComponent->c_str()).x;
        float nextComponentMaxX =
            lastComponentMaxX + ImGui::GetStyle().ItemSpacing.x + ImGui::GetStyle().FramePadding.x * 2 + nextComponentWidth;

        if (nextComponentMaxX >= windowMaxContentX) {
          // Remaining components are offscreen -- we'll need to show a context menu
          collapsedComponentIterator = component + 1;
          break;
        }
      }
    }

    if (collapsedComponentIterator != directoryComponents.end()) {
      const ImVec2& pos = ImGui::GetCursorScreenPos();
      if (ImGui::SmallButton("##CollapsedComponents")) {
        ImGui::OpenPopup("CollapsedComponentsPopup");
      }
      ImGui::RenderArrow(ImGui::GetWindowDrawList(), ImVec2(pos.x + 2.f, pos.y + ImGui::GetFontSize() * 0.1),
                         ImGui::GetColorU32(ImGuiCol_Text), ImGuiDir_Right, 0.75);

      if (ImGui::BeginPopup("CollapsedComponentsPopup")) {
        for (auto component = collapsedComponentIterator; component != directoryComponents.end(); component++) {
          if (ImGui::Selectable(component->c_str())) {
            // Set the new directory to the path of all the previous components
            dialog->ScanDirectory(join(std::vector<std::string>(directoryComponents.begin(), component + 1), std::string(1, PATH_SEP)));
          }
        }
        ImGui::EndPopup();
      }
    }
  }

  {
    std::vector<FileDialog_::Entry> displayedEntries = dialog->GetEntries();

    if (ImGui::BeginTable("FilesTable", 4, ImGuiTableFlags_ScrollX | ImGuiTableFlags_ScrollY,
                          ImVec2(0, (-2 * ImGui::GetFrameHeightWithSpacing()) + (-2 * ImGui::GetStyle().ItemSpacing.y)))) {
      ImGui::TableSetupColumn("##Type", ImGuiTableColumnFlags_WidthFixed, 0.0f);
      ImGui::TableSetupColumn("Name", ImGuiTableColumnFlags_WidthFixed, 0.0f);
      ImGui::TableSetupColumn("Size", ImGuiTableColumnFlags_WidthFixed, 0.0f);
      ImGui::TableSetupColumn("Modified", ImGuiTableColumnFlags_WidthFixed, 0.0f);
      ImGui::TableHeadersRow();

      if (recentlyChanged) {
        ImGui::SetItemDefaultFocus();
        ImGui::SetKeyboardFocusHere();
      }

      if (!dialog->IsShowingDeviceList()) {
        ImGui::TableNextRow();
        ImGui::TableNextColumn();
        const ImVec2& pos = ImGui::GetCursorScreenPos();
        ImGui::RenderArrow(ImGui::GetWindowDrawList(), ImVec2(pos.x, pos.y + ImGui::GetFontSize() * 0.1),
                           ImGui::GetColorU32(ImGuiCol_TextDisabled), ImGuiDir_Up, 0.65);
        ImGui::Dummy(ImVec2(ImGui::GetFontSize() * 0.5, ImGui::GetFontSize()));

        ImGui::TableNextColumn();
        if (ImGui::Selectable("../", nullptr, ImGuiSelectableFlags_SpanAllColumns | ImGuiSelectableFlags_DontClosePopups)) {
          dialog->ScanParentDirectory();
        }

        ImGui::TableNextColumn(); // No size for this entry
        ImGui::TableNextColumn(); // No modified time for this entry
      }

      for (FileDialog_::Entry& entry : displayedEntries) {
        std::string entryFullPath = joinpath(dialog->GetCurrentDirectory(), entry.name);
        bool isEntrySelected = (entryFullPath == dialog->GetSelectedPath());

        std::string entryLabel = entry.name;
        if (entry.type == FileDialog_::EntryTypeDirectory) {
          entryLabel += PATH_SEP;
        }

        ImGui::TableNextRow();
        ImGui::TableNextColumn();
        if (entry.type == FileDialog_::EntryTypeDirectory) {
          const ImVec2& pos = ImGui::GetCursorScreenPos();
          ImGui::RenderArrow(ImGui::GetWindowDrawList(), ImVec2(pos.x, pos.y + ImGui::GetFontSize() * 0.1),
                             ImGui::GetColorU32(ImGuiCol_Text), ImGuiDir_Right, 0.65);
          ImGui::Dummy(ImVec2(ImGui::GetFontSize() * 0.5, ImGui::GetFontSize()));
        }

        ImGui::TableNextColumn();
        if (ImGui::Selectable(entryLabel.c_str(), isEntrySelected,
                              ImGuiSelectableFlags_SpanAllColumns | ImGuiSelectableFlags_DontClosePopups)) {
          if (entry.type == FileDialog_::EntryTypeDirectory) {
            dialog->ScanSubdirectory(entry.name);
          } else {
            dialog->SetSelectedPath(entryFullPath);
          }
        }

        ImGui::TableNextColumn();
        if (entry.type == FileDialog_::EntryTypeFile) {
          ImGui::Text(friendlysize(entry.size).c_str());
        }

        ImGui::TableNextColumn();
        if (entry.type == FileDialog_::EntryTypeFile) {
          ImGui::Text(friendlytime(entry.modified).c_str());
        }
      }
      ImGui::EndTable();
    }
  }

  ImGui::Separator();

  std::string selectedPath = dialog->GetSelectedPath();
  ImGui::AlignTextToFramePadding();
  if (dironly) {
    ImGui::Text("Directory:");
  } else {
    ImGui::Text("File:");
  }
  ImGui::SameLine();
  ImGui::SetNextItemWidth(-FLT_MIN);
  ImGui::BeginDisabled();
  ImGui::InputText("##SelectedPath", (char *)selectedPath.c_str(), selectedPath.size(), ImGuiInputTextFlags_ReadOnly);
  ImGui::EndDisabled();

  ImGui::Separator();

  bool finished = false;

  // ImGui::BeginDisabled(selectedPath.empty());
  // if (ImGui::Button("OK")) {
  //   strncpy(selectedFile, selectedPath.c_str(), MAX_PATH);
  //   dialogs.erase(key);
  //   finished = true;
  // }
  // ImGui::EndDisabled();
  // ImGui::SameLine();
  // if (ImGui::Button("Cancel")) {
  //   selectedFile[0] = 0;
  //   dialogs.erase(key);
  //   finished = true;
  // }

  PS2Plus::Components::BeginFooterCommandMenu("FileMenu", PS2Plus::Components::kFooterCommandMenuEnabledAlways);
  {
    if (PS2Plus::Components::FooterCommand("OK", ICON_PLAYSTATION_START_BUTTON, ICON_PLAYSTATION_COLOR_GREEN, ImGuiKey_GamepadStart)) {
      strncpy(selectedFile, selectedPath.c_str(), MAX_PATH);
      dialogs.erase(key);
      finished = true;
    }

    if (PS2Plus::Components::FooterCommand("Cancel", ICON_PLAYSTATION_SELECT_BUTTON, ICON_PLAYSTATION_COLOR_RED, ImGuiKey_GamepadBack)) {
      selectedFile[0] = 0;
      dialogs.erase(key);
      finished = true;
    }

    if (!dialog->GetDeviceList().empty()) {
      if (PS2Plus::Components::FooterCommand("Devices", ICON_PLAYSTATION_SQUARE_BUTTON, ICON_PLAYSTATION_COLOR_SQUARE, ImGuiKey_GamepadFaceLeft)) {
        dialog->ShowDeviceList();
      }
    }

    if (!dialog->IsShowingDeviceList()) {
      if (PS2Plus::Components::FooterCommand("Up", ICON_PLAYSTATION_TRIANGLE_BUTTON, ICON_PLAYSTATION_COLOR_TRIANGLE, ImGuiKey_GamepadFaceUp)) {
        dialog->ScanParentDirectory();
      }
    }
  }
  PS2Plus::Components::EndFooterCommandMenu();

  return finished;
}
} // namespace PS2Plus::Components