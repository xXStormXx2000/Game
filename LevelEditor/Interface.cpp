#include "Interface.h"

DynamicArray<std::filesystem::path> Interface::findFiles(const std::filesystem::path& folder) {
    if (!std::filesystem::exists(folder) || !std::filesystem::is_directory(folder)) {
        debugMessage("Invalid directory: " << folder);
        return {};
    }
    DynamicArray<std::filesystem::path> out;
    for (const std::filesystem::directory_entry& entry : std::filesystem::recursive_directory_iterator(folder)) {
        if (std::filesystem::is_regular_file(entry.path())) {
            out.pushBack(entry.path());
        }
    }
    return out;
}

void Interface::setGamePath(const std::filesystem::path& path) {
	this->gamePath = path;
}

void Interface::start(Entity) {
    artFiles = findFiles(gamePath / "Assets" / "Art");
    audioFiles = findFiles(gamePath / "Assets" / "Audio");
    musicFiles = findFiles(gamePath / "Assets" / "Music");
}
