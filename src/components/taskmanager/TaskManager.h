#pragma once

#include "core/AWindow.h"
#include <string>
#include <vector>

namespace csopesy {

struct ProcessEntry {
    std::string name;
    int         pid;
    float       baseCpu;    // base CPU % (animated around this value each frame)
    float       memoryMB;
    std::string description;
};

class TaskManager : public AWindow {
public:
    TaskManager();
    ~TaskManager() = default;

    void draw() override;

private:
    void drawProcessesTab();
    void drawPerformanceTab();

    std::vector<ProcessEntry> m_processes;
    int   m_selectedRow = -1;
    int   m_activeTab   = 0;   // 0=Processes  1=Performance
    char  m_searchBuf[128]  = {};
};

} // namespace csopesy
