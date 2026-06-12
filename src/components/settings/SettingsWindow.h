#pragma once

#include "core/AWindow.h"

namespace csopesy {

class SettingsWindow : public AWindow {
public:
    SettingsWindow();
    ~SettingsWindow() = default;

    void draw() override;

private:
    void drawDisplaySection();
    void drawPersonalizationSection();
    void drawSystemSection();

    // placeholder state
    bool  m_vsync         = true;
    bool  m_fullscreen    = false;
    bool  m_notifications = true;
    bool  m_darkMode      = true;
    int   m_resolutionIdx = 2;
    float m_uiScale       = 1.0f;
    float m_volume        = 0.75f;
    int   m_accentColor   = 3;
};

} // namespace csopesy
