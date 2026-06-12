#include "FilesWindow.h"
#include "imgui.h"

namespace csopesy {

FilesWindow::FilesWindow() : AWindow("File Explorer") {
    m_currentPath = "C:\\Users\\User\\Documents";

    // Build placeholder folder tree
    m_root.name = "This PC";
    m_root.open = true;

    FolderNode cDrive; cDrive.name = "C:\\";
    {
        FolderNode users; users.name = "Users";
        {
            FolderNode user; user.name = "User";
            FolderNode docs; docs.name = "Documents"; docs.open = true;
            FolderNode dwnl; dwnl.name = "Downloads";
            FolderNode desk; desk.name = "Desktop";
            FolderNode pics; pics.name = "Pictures";
            user.children = { docs, dwnl, desk, pics };
            users.children = { user };
        }
        FolderNode prog; prog.name = "Program Files";
        {
            FolderNode msft;  msft.name  = "Microsoft";
            FolderNode steam; steam.name = "Steam";
            prog.children = { msft, steam };
        }
        FolderNode win; win.name = "Windows";
        cDrive.children = { users, prog, win };
    }

    FolderNode dDrive; dDrive.name = "D:\\";
    {
        FolderNode games;   games.name   = "Games";
        FolderNode backups; backups.name = "Backups";
        dDrive.children = { games, backups };
    }

    m_root.children = { cDrive, dDrive };

    // Default file listing for Documents
    m_files = {
        { "report_final.docx",      "Word Document",  "245 KB",  "2026-06-10" },
        { "budget_2026.xlsx",        "Excel Sheet",    "128 KB",  "2026-06-08" },
        { "notes.txt",               "Text File",       "12 KB",  "2026-06-11" },
        { "presentation.pptx",       "Presentation",   "1.8 MB",  "2026-05-30" },
        { "photo_backup.zip",        "ZIP Archive",    "412 MB",  "2026-04-15" },
        { "csopesy_design.pdf",      "PDF Document",   "3.2 MB",  "2026-06-01" },
        { "config.json",             "JSON File",        "4 KB",  "2026-06-09" },
        { "main.cpp",                "C++ Source",      "18 KB",  "2026-06-12" },
        { "CMakeLists.txt",          "CMake Script",     "3 KB",  "2026-06-12" },
        { "README.md",               "Markdown",         "6 KB",  "2026-06-05" },
    };
}

void FilesWindow::draw() {
    if (!isVisible) return;

    ImVec2 displaySize = ImGui::GetIO().DisplaySize;
    ImGui::SetNextWindowSize(ImVec2(660.0f, 460.0f), ImGuiCond_Once);
    ImGui::SetNextWindowPos(
        ImVec2((displaySize.x - 660.0f) * 0.5f, (displaySize.y - 460.0f) * 0.5f),
        ImGuiCond_Once);

    ImGui::PushStyleColor(ImGuiCol_TitleBg,       ImVec4(0.10f, 0.10f, 0.16f, 1.0f));
    ImGui::PushStyleColor(ImGuiCol_TitleBgActive,  ImVec4(0.12f, 0.20f, 0.38f, 1.0f));
    ImGui::PushStyleColor(ImGuiCol_WindowBg,       ImVec4(0.08f, 0.08f, 0.12f, 0.97f));
    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 4.0f);

    ImGui::Begin("File Explorer", &isVisible, ImGuiWindowFlags_NoCollapse);

    // Address bar
    ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4(0.12f, 0.12f, 0.20f, 1.0f));
    char pathBuf[256];
    snprintf(pathBuf, sizeof(pathBuf), "%s", m_currentPath.c_str());
    ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x - 140.0f);
    ImGui::InputText("##path", pathBuf, sizeof(pathBuf), ImGuiInputTextFlags_ReadOnly);
    ImGui::PopStyleColor();
    ImGui::SameLine(0, 6);
    ImGui::SetNextItemWidth(130.0f);
    ImGui::InputTextWithHint("##fsearch", "Search...", m_searchBuf, sizeof(m_searchBuf));

    ImGui::Spacing();

    // Two-panel layout
    float totalW   = ImGui::GetContentRegionAvail().x;
    float treeW    = 170.0f;
    float contentH = ImGui::GetContentRegionAvail().y - 30.0f;

    // Left: folder tree
    ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.07f, 0.07f, 0.11f, 1.0f));
    ImGui::BeginChild("##tree", ImVec2(treeW, contentH), true);
    drawFolderTree(m_root, 0);
    ImGui::EndChild();
    ImGui::PopStyleColor();

    ImGui::SameLine(0, 4);

    // Right: file list
    ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.09f, 0.09f, 0.14f, 1.0f));
    ImGui::BeginChild("##filelist", ImVec2(0.0f, contentH), true);
    drawFileList();
    ImGui::EndChild();
    ImGui::PopStyleColor();

    // Status bar
    ImGui::Separator();
    ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.55f, 0.55f, 0.65f, 1.0f));
    ImGui::Text("%d items", (int)m_files.size());
    if (m_selectedFile >= 0) {
        ImGui::SameLine(120.0f);
        ImGui::Text("%s  |  %s",
            m_files[m_selectedFile].name.c_str(),
            m_files[m_selectedFile].size.c_str());
    }
    ImGui::PopStyleColor();

    ImGui::End();
    ImGui::PopStyleVar();
    ImGui::PopStyleColor(3);
}

void FilesWindow::drawFolderTree(FolderNode& node, int depth) {
    ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow |
                               ImGuiTreeNodeFlags_SpanAvailWidth;
    if (node.children.empty()) flags |= ImGuiTreeNodeFlags_Leaf;
    if (node.open)             flags |= ImGuiTreeNodeFlags_DefaultOpen;

    // Tint folders
    ImGui::PushStyleColor(ImGuiCol_Text,
        depth == 0 ? ImVec4(0.90f, 0.90f, 1.00f, 1.0f)
                   : ImVec4(0.75f, 0.85f, 1.00f, 1.0f));

    bool opened = ImGui::TreeNodeEx(node.name.c_str(), flags);
    ImGui::PopStyleColor();

    if (ImGui::IsItemClicked() && !node.children.empty()) {
        m_currentPath = "C:\\" + node.name;
    }

    if (opened) {
        for (auto& child : node.children) {
            drawFolderTree(child, depth + 1);
        }
        ImGui::TreePop();
    }
}

void FilesWindow::drawFileList() {
    static ImGuiTableFlags flags =
        ImGuiTableFlags_SizingStretchProp |
        ImGuiTableFlags_Resizable         |
        ImGuiTableFlags_BordersInnerV     |
        ImGuiTableFlags_RowBg             |
        ImGuiTableFlags_ScrollY;

    ImGui::PushStyleColor(ImGuiCol_TableHeaderBg, ImVec4(0.12f, 0.12f, 0.20f, 1.0f));
    ImGui::PushStyleColor(ImGuiCol_TableRowBg,    ImVec4(0.09f, 0.09f, 0.14f, 1.0f));
    ImGui::PushStyleColor(ImGuiCol_TableRowBgAlt, ImVec4(0.11f, 0.11f, 0.17f, 1.0f));

    if (ImGui::BeginTable("FileTable", 4, flags)) {
        ImGui::TableSetupScrollFreeze(0, 1);
        ImGui::TableSetupColumn("Name",         ImGuiTableColumnFlags_WidthStretch);
        ImGui::TableSetupColumn("Type",         ImGuiTableColumnFlags_WidthFixed, 110.0f);
        ImGui::TableSetupColumn("Size",         ImGuiTableColumnFlags_WidthFixed,  72.0f);
        ImGui::TableSetupColumn("Date modified",ImGuiTableColumnFlags_WidthFixed,  95.0f);
        ImGui::TableHeadersRow();

        for (int i = 0; i < (int)m_files.size(); i++) {
            const FileEntry& f = m_files[i];

            // Search filter
            if (m_searchBuf[0] != '\0') {
                bool found = false;
                for (int h = 0; f.name[h]; h++) {
                    bool match = true;
                    for (int n = 0; m_searchBuf[n]; n++) {
                        char hc = (f.name[h+n] >= 'A' && f.name[h+n] <= 'Z') ? f.name[h+n]+32 : f.name[h+n];
                        char nc = (m_searchBuf[n] >= 'A' && m_searchBuf[n] <= 'Z') ? m_searchBuf[n]+32 : m_searchBuf[n];
                        if (hc != nc) { match = false; break; }
                    }
                    if (match) { found = true; break; }
                }
                if (!found) continue;
            }

            ImGui::TableNextRow();
            ImGui::TableSetColumnIndex(0);
            char selId[32];
            snprintf(selId, sizeof(selId), "##fsel%d", i);
            bool sel = (m_selectedFile == i);
            if (ImGui::Selectable(selId, sel,
                    ImGuiSelectableFlags_SpanAllColumns,
                    ImVec2(0.0f, 0.0f))) {
                m_selectedFile = i;
            }
            ImGui::SameLine();
            // Color-code by type
            ImVec4 col = {0.85f, 0.85f, 0.95f, 1.0f};
            if (f.type == "C++ Source" || f.type == "CMake Script" || f.type == "JSON File")
                col = {0.55f, 0.90f, 0.55f, 1.0f};
            else if (f.type == "PDF Document" || f.type == "Word Document")
                col = {0.55f, 0.75f, 1.00f, 1.0f};
            else if (f.type == "ZIP Archive")
                col = {1.00f, 0.80f, 0.35f, 1.0f};
            ImGui::TextColored(col, "%s", f.name.c_str());

            ImGui::TableSetColumnIndex(1);
            ImGui::TextDisabled("%s", f.type.c_str());

            ImGui::TableSetColumnIndex(2);
            ImGui::Text("%s", f.size.c_str());

            ImGui::TableSetColumnIndex(3);
            ImGui::TextDisabled("%s", f.modified.c_str());
        }
        ImGui::EndTable();
    }
    ImGui::PopStyleColor(3);
}

} // namespace csopesy
