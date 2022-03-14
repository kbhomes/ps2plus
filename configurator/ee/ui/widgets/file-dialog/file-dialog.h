#include <imgui/imgui.h>

#include <dirent.h>
#include <string>
#include <time.h>
#include <vector>

namespace ImGui::Widgets {
    class FileDialog_ {
    public:
        enum EntryType {
            EntryTypeFile,
            EntryTypeDirectory,
        };

        struct Entry {
            std::string name;
            EntryType type;
            size_t size;
            struct timespec modified;
        };

        FileDialog_(std::string initialDirectory);
        void ScanDirectory(std::string directory);
        void ScanSubdirectory(std::string directory);
        void ScanParentDirectory();
        void SetDeviceList(std::vector<std::string> deviceList);
        void ShowDeviceList();

        std::vector<std::string> GetDeviceList();
        std::vector<Entry> GetEntries();
        std::vector<std::string> GetDirectoryComponents();
        std::string GetCurrentDirectory();
        std::string GetSelectedPath();
        void SetSelectedPath(std::string path);
        bool IsShowingDeviceList();
        bool HasRecentlyChanged();

    private:

        bool _opened = false;
        bool _showingDeviceList = false;
        std::string _initialDirectory;
        std::string _currentDirectory;
        std::vector<std::string> _currentDirectoryComponents;
        std::vector<Entry> _currentEntries;
        std::string _selectedPath;
        std::vector<std::string> _deviceList;
        bool _recentlyChanged;
    };

    bool FileDialog(const char *key, const char *initialDirectory, std::vector<std::string> deviceList, char *selectedFile);
};