#include "versions.h"

std::string format_version(uint64_t version) {
  std::string out;
  out += "v" + std::to_string(VERSION_MAJOR(version));
  out += "." + std::to_string(VERSION_MINOR(version));
  out += "." + std::to_string(VERSION_PATCH(version));

  int metadata = VERSION_METADATA(version);
  switch (metadata) {
    case VERSION_METADATA_DEV:
      out += "-dev";
      break;

    case VERSION_METADATA_LATEST:
      out += "-latest";
      break;
  }

  return out;
}