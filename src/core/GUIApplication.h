#pragma once

#include <memory>

struct GLFWwindow;

namespace csopesy {

class Desktop;
class Taskbar;
class TaskManager;
class SettingsWindow;
class FilesWindow;

class GUIApplication {
public:
    GUIApplication();
    ~GUIApplication();

    bool initialize();
    void run();

private:
    void bootstrap();
    void kernelInit();
    void startSystemServices();
    void mainLoop();
    void shutdown();

    GLFWwindow* m_window = nullptr;

    std::shared_ptr<Desktop>        m_desktop;
    std::shared_ptr<Taskbar>        m_taskbar;
    std::shared_ptr<TaskManager>    m_taskManager;
    std::shared_ptr<SettingsWindow> m_settingsWindow;
    std::shared_ptr<FilesWindow>    m_filesWindow;
};

} // namespace csopesy
