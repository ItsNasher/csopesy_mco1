#pragma once

#include "core/AWindow.h"
#include <string>
#include <vector>

namespace csopesy {

struct FileEntry {
    std::string name;
    std::string type;
    std::string size;
    std::string modified;
};

struct FolderNode {
    std::string          name;
    std::vector<FolderNode> children;
    bool                 open = false;
};

class FilesWindow : public AWindow {
public:
    FilesWindow();
    ~FilesWindow() = default;

    void draw() override;

private:
    void drawFolderTree(FolderNode& node, int depth);
    void drawFileList();

    FolderNode           m_root;
    std::string          m_currentPath;
    std::vector<FileEntry> m_files;
    int                  m_selectedFile = -1;
    char                 m_searchBuf[128] = {};
};

} // namespace csopesy
