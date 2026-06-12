#include "TaskManager.h"
#include "imgui.h"

#include <cmath>
#include <cstring>

namespace csopesy {

TaskManager::TaskManager() : AWindow("Task Manager") {
    m_processes = {
        {"System Idle Process",    0,  92.0f,    0.0f,  "Percentage of time the processor is idle"},
        {"System",                 4,   0.1f,    2.1f,  "NT Kernel & System"},
        {"Registry",             128,   0.0f,   48.2f,  "Registry"},
        {"csrss.exe",            544,   0.1f,    4.1f,  "Client Server Runtime Process"},
        {"services.exe",         736,   0.2f,    5.8f,  "Services and Controller app"},
        {"lsass.exe",            780,   0.3f,   12.4f,  "Local Security Authority Process"},
        {"svchost.exe",          916,   0.6f,   45.3f,  "Host Process for Windows Services"},
        {"explorer.exe",        2048,   1.2f,   67.2f,  "Windows Explorer"},
        {"csopesy.exe",         1024,   2.8f,  128.7f,  "CSOPESY OS Simulator"},
        {"Code.exe",            3120,   3.5f,  412.5f,  "Visual Studio Code"},
        {"RuntimeBroker.exe",   4256,   0.4f,   18.6f,  "Runtime Broker"},
        {"SearchIndexer.exe",   5012,   0.9f,   34.1f,  "Microsoft Windows Search Indexer"},
        {"WmiPrvSE.exe",        5780,   0.2f,   10.3f,  "WMI Provider Host"},
        {"msedge.exe",          6144,   1.8f,  234.9f,  "Microsoft Edge"},
        {"SecurityHealthSvc.exe", 7008, 0.1f,   8.7f,   "Windows Security Health Service"},
    };
}

void TaskManager::draw() {
    if (!isVisible) return;

    ImVec2 displaySize = ImGui::GetIO().DisplaySize;
    ImGui::SetNextWindowSize(ImVec2(720.0f, 500.0f), ImGuiCond_Once);
    ImGui::SetNextWindowPos(
        ImVec2((displaySize.x - 720.0f) * 0.5f, (displaySize.y - 500.0f) * 0.5f),
        ImGuiCond_Once);

    ImGui::PushStyleColor(ImGuiCol_TitleBg,       ImVec4(0.10f, 0.10f, 0.16f, 1.0f));
    ImGui::PushStyleColor(ImGuiCol_TitleBgActive,  ImVec4(0.12f, 0.20f, 0.38f, 1.0f));
    ImGui::PushStyleColor(ImGuiCol_WindowBg,       ImVec4(0.08f, 0.08f, 0.12f, 0.97f));
    ImGui::PushStyleColor(ImGuiCol_Tab,            ImVec4(0.12f, 0.12f, 0.18f, 1.0f));
    ImGui::PushStyleColor(ImGuiCol_TabHovered,     ImVec4(0.20f, 0.30f, 0.50f, 1.0f));
    ImGui::PushStyleColor(ImGuiCol_TabActive,      ImVec4(0.14f, 0.22f, 0.42f, 1.0f));
    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 4.0f);

    ImGui::Begin("Task Manager", &isVisible,
        ImGuiWindowFlags_NoCollapse);

    // Tab bar
    if (ImGui::BeginTabBar("TMTabs")) {
        if (ImGui::BeginTabItem("Processes")) {
            m_activeTab = 0;
            drawProcessesTab();
            ImGui::EndTabItem();
        }
        if (ImGui::BeginTabItem("Performance")) {
            m_activeTab = 1;
            drawPerformanceTab();
            ImGui::EndTabItem();
        }
        // Placeholder tabs to match Windows TM look
        if (ImGui::BeginTabItem("App history"))  { ImGui::TextDisabled("(no data)"); ImGui::EndTabItem(); }
        if (ImGui::BeginTabItem("Startup"))       { ImGui::TextDisabled("(no data)"); ImGui::EndTabItem(); }
        if (ImGui::BeginTabItem("Users"))         { ImGui::TextDisabled("(no data)"); ImGui::EndTabItem(); }
        if (ImGui::BeginTabItem("Details"))       { ImGui::TextDisabled("(no data)"); ImGui::EndTabItem(); }
        ImGui::EndTabBar();
    }

    // Status bar
    ImGui::Separator();
    ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.55f, 0.55f, 0.65f, 1.0f));
    ImGui::Text("Processes: %d", (int)m_processes.size());
    ImGui::SameLine(160.0f);
    ImGui::Text("CPU Usage: %.1f%%", 4.2f + sinf((float)ImGui::GetTime() * 0.3f) * 1.5f);
    ImGui::SameLine(330.0f);
    ImGui::Text("Memory: 4.2 / 16.0 GB");
    ImGui::PopStyleColor();

    ImGui::End();
    ImGui::PopStyleVar();
    ImGui::PopStyleColor(6);
}

void TaskManager::drawProcessesTab() {
    // Search filter
    ImGui::SetNextItemWidth(220.0f);
    ImGui::InputTextWithHint("##search", "Search processes...", m_searchBuf, sizeof(m_searchBuf));
    ImGui::SameLine();
    ImGui::TextDisabled("  %d processes", (int)m_processes.size());

    ImGui::Spacing();

    static ImGuiTableFlags tableFlags =
        ImGuiTableFlags_SizingStretchProp   |
        ImGuiTableFlags_Resizable           |
        ImGuiTableFlags_BordersOuter        |
        ImGuiTableFlags_BordersInnerV       |
        ImGuiTableFlags_RowBg               |
        ImGuiTableFlags_ScrollY             |
        ImGuiTableFlags_Sortable;

    float tableH = ImGui::GetContentRegionAvail().y - 36.0f;

    ImGui::PushStyleColor(ImGuiCol_TableHeaderBg,  ImVec4(0.14f, 0.14f, 0.22f, 1.0f));
    ImGui::PushStyleColor(ImGuiCol_TableRowBg,     ImVec4(0.09f, 0.09f, 0.14f, 1.0f));
    ImGui::PushStyleColor(ImGuiCol_TableRowBgAlt,  ImVec4(0.11f, 0.11f, 0.17f, 1.0f));

    if (ImGui::BeginTable("ProcTable", 5, tableFlags, ImVec2(0.0f, tableH))) {
        ImGui::TableSetupScrollFreeze(0, 1);
        ImGui::TableSetupColumn("Name",        ImGuiTableColumnFlags_WidthStretch);
        ImGui::TableSetupColumn("PID",         ImGuiTableColumnFlags_WidthFixed,  55.0f);
        ImGui::TableSetupColumn("Status",      ImGuiTableColumnFlags_WidthFixed,  75.0f);
        ImGui::TableSetupColumn("CPU",         ImGuiTableColumnFlags_WidthFixed,  70.0f);
        ImGui::TableSetupColumn("Memory (MB)", ImGuiTableColumnFlags_WidthFixed, 105.0f);
        ImGui::TableHeadersRow();

        float t = (float)ImGui::GetTime();

        for (int i = 0; i < (int)m_processes.size(); i++) {
            const ProcessEntry& proc = m_processes[i];

            // Filter by search string
            if (m_searchBuf[0] != '\0') {
                bool found = false;
                const char* haystack = proc.name.c_str();
                const char* needle   = m_searchBuf;
                // case-insensitive substring check
                for (int h = 0; haystack[h]; h++) {
                    bool match = true;
                    for (int n = 0; needle[n]; n++) {
                        char hc = (haystack[h+n] >= 'A' && haystack[h+n] <= 'Z') ? haystack[h+n]+32 : haystack[h+n];
                        char nc = (needle[n]    >= 'A' && needle[n]    <= 'Z') ? needle[n]+32    : needle[n];
                        if (hc != nc) { match = false; break; }
                    }
                    if (match) { found = true; break; }
                }
                if (!found) continue;
            }

            // Animate CPU around base value
            float phase = (float)i * 1.618f;
            float cpu   = proc.baseCpu + sinf(t * 0.4f + phase) * proc.baseCpu * 0.12f;
            if (cpu < 0.0f) cpu = 0.0f;

            ImGui::TableNextRow();

            // Row background tinting by CPU level
            if (cpu > 50.0f) {
                ImGui::TableSetBgColor(ImGuiTableBgTarget_RowBg0, IM_COL32(90, 18, 18, 100));
                ImGui::TableSetBgColor(ImGuiTableBgTarget_RowBg1, IM_COL32(90, 18, 18, 120));
            } else if (cpu > 5.0f) {
                ImGui::TableSetBgColor(ImGuiTableBgTarget_RowBg0, IM_COL32(70, 60, 10, 80));
                ImGui::TableSetBgColor(ImGuiTableBgTarget_RowBg1, IM_COL32(70, 60, 10, 100));
            }

            // Name column (selectable spanning all columns)
            ImGui::TableSetColumnIndex(0);
            char selectId[32];
            snprintf(selectId, sizeof(selectId), "##sel%d", i);
            bool selected = (m_selectedRow == i);
            if (ImGui::Selectable(selectId, selected,
                    ImGuiSelectableFlags_SpanAllColumns,
                    ImVec2(0.0f, 0.0f))) {
                m_selectedRow = i;
            }
            ImGui::SameLine();
            ImGui::TextUnformatted(proc.name.c_str());
            if (ImGui::IsItemHovered() && proc.description[0]) {
                ImGui::SetTooltip("%s", proc.description.c_str());
            }

            // PID
            ImGui::TableSetColumnIndex(1);
            ImGui::Text("%d", proc.pid);

            // Status
            ImGui::TableSetColumnIndex(2);
            ImGui::TextColored(ImVec4(0.30f, 0.88f, 0.30f, 1.0f), "Running");

            // CPU %
            ImGui::TableSetColumnIndex(3);
            if (cpu > 50.0f)
                ImGui::TextColored(ImVec4(1.00f, 0.28f, 0.28f, 1.0f), "%.1f%%", cpu);
            else if (cpu > 5.0f)
                ImGui::TextColored(ImVec4(1.00f, 0.80f, 0.10f, 1.0f), "%.1f%%", cpu);
            else
                ImGui::Text("%.1f%%", cpu);

            // Memory
            ImGui::TableSetColumnIndex(4);
            if (proc.memoryMB >= 200.0f)
                ImGui::TextColored(ImVec4(1.00f, 0.75f, 0.30f, 1.0f), "%.1f MB", proc.memoryMB);
            else
                ImGui::Text("%.1f MB", proc.memoryMB);
        }

        ImGui::EndTable();
    }
    ImGui::PopStyleColor(3);

    // End Task button
    ImGui::Spacing();
    bool hasSelection = (m_selectedRow >= 0);
    if (!hasSelection) {
        ImGui::PushStyleVar(ImGuiStyleVar_Alpha, 0.35f);
    }
    ImGui::PushStyleColor(ImGuiCol_Button,        ImVec4(0.18f, 0.28f, 0.52f, 1.0f));
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.26f, 0.40f, 0.70f, 1.0f));
    ImGui::PushStyleColor(ImGuiCol_ButtonActive,  ImVec4(0.14f, 0.22f, 0.42f, 1.0f));
    if (ImGui::Button("End Task", ImVec2(100.0f, 26.0f)) && hasSelection) {
        // placeholder: remove process from list
        m_processes.erase(m_processes.begin() + m_selectedRow);
        m_selectedRow = -1;
    }
    ImGui::PopStyleColor(3);
    if (!hasSelection) ImGui::PopStyleVar();
    ImGui::SameLine();
    ImGui::TextDisabled("  Select a process to end it");
}

void TaskManager::drawPerformanceTab() {
    float t = (float)ImGui::GetTime();

    // CPU section
    ImGui::Text("CPU");
    ImGui::Separator();

    // Build a fake CPU usage history using sin
    static float cpuHistory[120] = {};
    static int   cpuOffset       = 0;
    static double lastUpdate     = 0.0;
    if (ImGui::GetTime() - lastUpdate > 0.5) {
        float base = 6.5f + sinf(t * 0.25f) * 3.0f + sinf(t * 0.7f) * 1.5f;
        cpuHistory[cpuOffset] = base;
        cpuOffset = (cpuOffset + 1) % 120;
        lastUpdate = ImGui::GetTime();
    }

    char overlay[32];
    float currentCpu = 4.2f + sinf(t * 0.3f) * 1.5f;
    snprintf(overlay, sizeof(overlay), "%.1f%%", currentCpu);
    ImGui::PlotLines("##cpu", cpuHistory, 120, cpuOffset,
        overlay, 0.0f, 100.0f, ImVec2(ImGui::GetContentRegionAvail().x, 120.0f));

    ImGui::Spacing();
    ImGui::Columns(3, "cpucols", false);
    ImGui::Text("Utilization");
    ImGui::TextColored(ImVec4(0.4f, 0.8f, 1.0f, 1.0f), "%.1f%%", currentCpu);
    ImGui::NextColumn();
    ImGui::Text("Speed");
    ImGui::TextColored(ImVec4(0.4f, 0.8f, 1.0f, 1.0f), "3.20 GHz");
    ImGui::NextColumn();
    ImGui::Text("Processes");
    ImGui::TextColored(ImVec4(0.4f, 0.8f, 1.0f, 1.0f), "%d", (int)m_processes.size());
    ImGui::Columns(1);

    ImGui::Spacing();
    ImGui::Separator();
    ImGui::Spacing();

    // Memory section
    ImGui::Text("Memory");
    ImGui::Separator();

    float memUsed  = 4.2f + sinf(t * 0.15f) * 0.3f;
    float memTotal = 16.0f;
    float memFrac  = memUsed / memTotal;

    char memLabel[64];
    snprintf(memLabel, sizeof(memLabel), "%.1f / %.0f GB  (%.0f%%)", memUsed, memTotal, memFrac * 100.0f);
    ImGui::ProgressBar(memFrac, ImVec2(ImGui::GetContentRegionAvail().x, 24.0f), memLabel);

    ImGui::Spacing();
    ImGui::Columns(3, "memcols", false);
    ImGui::Text("In use");
    ImGui::TextColored(ImVec4(0.4f, 0.8f, 1.0f, 1.0f), "%.1f GB", memUsed);
    ImGui::NextColumn();
    ImGui::Text("Available");
    ImGui::TextColored(ImVec4(0.4f, 0.8f, 1.0f, 1.0f), "%.1f GB", memTotal - memUsed);
    ImGui::NextColumn();
    ImGui::Text("Slots used");
    ImGui::TextColored(ImVec4(0.4f, 0.8f, 1.0f, 1.0f), "2 of 4");
    ImGui::Columns(1);
}

} // namespace csopesy
