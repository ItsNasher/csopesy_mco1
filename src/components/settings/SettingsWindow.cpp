#include "SettingsWindow.h"
#include "imgui.h"

namespace csopesy {

SettingsWindow::SettingsWindow() : AWindow("Settings") {}

void SettingsWindow::draw() {
    if (!isVisible) return;

    ImVec2 displaySize = ImGui::GetIO().DisplaySize;
    ImGui::SetNextWindowSize(ImVec2(520.0f, 440.0f), ImGuiCond_Once);
    ImGui::SetNextWindowPos(
        ImVec2((displaySize.x - 520.0f) * 0.5f, (displaySize.y - 440.0f) * 0.5f),
        ImGuiCond_Once);

    ImGui::PushStyleColor(ImGuiCol_TitleBg,       ImVec4(0.10f, 0.10f, 0.16f, 1.0f));
    ImGui::PushStyleColor(ImGuiCol_TitleBgActive,  ImVec4(0.12f, 0.20f, 0.38f, 1.0f));
    ImGui::PushStyleColor(ImGuiCol_WindowBg,       ImVec4(0.08f, 0.08f, 0.12f, 0.97f));
    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 4.0f);

    ImGui::Begin("Settings", &isVisible, ImGuiWindowFlags_NoCollapse);

    drawDisplaySection();
    ImGui::Spacing();
    drawPersonalizationSection();
    ImGui::Spacing();
    drawSystemSection();

    ImGui::End();
    ImGui::PopStyleVar();
    ImGui::PopStyleColor(3);
}

void SettingsWindow::drawDisplaySection() {
    ImGui::TextColored(ImVec4(0.40f, 0.75f, 1.00f, 1.0f), "Display");
    ImGui::Separator();
    ImGui::Spacing();

    static const char* resolutions[] = {
        "1280 x 720",
        "1600 x 900",
        "1920 x 1080",
        "2560 x 1440",
        "3840 x 2160",
    };
    ImGui::Text("Resolution");
    ImGui::SameLine(160.0f);
    ImGui::SetNextItemWidth(180.0f);
    ImGui::Combo("##res", &m_resolutionIdx, resolutions, 5);

    ImGui::Text("UI Scale");
    ImGui::SameLine(160.0f);
    ImGui::SetNextItemWidth(180.0f);
    ImGui::SliderFloat("##scale", &m_uiScale, 0.75f, 2.0f, "%.2fx");

    ImGui::Text("VSync");
    ImGui::SameLine(160.0f);
    ImGui::Checkbox("##vsync", &m_vsync);

    ImGui::Text("Fullscreen");
    ImGui::SameLine(160.0f);
    ImGui::Checkbox("##fs", &m_fullscreen);
}

void SettingsWindow::drawPersonalizationSection() {
    ImGui::TextColored(ImVec4(0.40f, 0.75f, 1.00f, 1.0f), "Personalization");
    ImGui::Separator();
    ImGui::Spacing();

    ImGui::Text("Dark Mode");
    ImGui::SameLine(160.0f);
    ImGui::Checkbox("##dark", &m_darkMode);

    static const char* accents[] = {
        "Blue", "Purple", "Green", "Teal", "Orange", "Red",
    };
    ImGui::Text("Accent Color");
    ImGui::SameLine(160.0f);
    ImGui::SetNextItemWidth(120.0f);
    ImGui::Combo("##accent", &m_accentColor, accents, 6);

    // Swatch preview
    ImGui::SameLine(0, 10);
    static const ImVec4 swatches[] = {
        {0.18f, 0.42f, 0.88f, 1.0f},
        {0.55f, 0.18f, 0.88f, 1.0f},
        {0.18f, 0.72f, 0.28f, 1.0f},
        {0.12f, 0.72f, 0.68f, 1.0f},
        {0.90f, 0.52f, 0.10f, 1.0f},
        {0.85f, 0.18f, 0.18f, 1.0f},
    };
    ImGui::ColorButton("##swatch", swatches[m_accentColor],
        ImGuiColorEditFlags_NoTooltip | ImGuiColorEditFlags_NoPicker,
        ImVec2(20.0f, 20.0f));
}

void SettingsWindow::drawSystemSection() {
    ImGui::TextColored(ImVec4(0.40f, 0.75f, 1.00f, 1.0f), "System");
    ImGui::Separator();
    ImGui::Spacing();

    ImGui::Text("Volume");
    ImGui::SameLine(160.0f);
    ImGui::SetNextItemWidth(180.0f);
    ImGui::SliderFloat("##vol", &m_volume, 0.0f, 1.0f, "%.0f%%",
        ImGuiSliderFlags_AlwaysClamp);

    ImGui::Text("Notifications");
    ImGui::SameLine(160.0f);
    ImGui::Checkbox("##notif", &m_notifications);

    ImGui::Spacing();
    ImGui::Separator();
    ImGui::Spacing();

    // System info
    ImGui::TextDisabled("OS:        CSOPESY v1.0 (Simulator)");
    ImGui::TextDisabled("Kernel:    ehrv-semi-mo build");
    ImGui::TextDisabled("Renderer:  OpenGL 3.3 + Dear ImGui");
    ImGui::TextDisabled("Build:     C++17 / CMake");

    ImGui::Spacing();
    ImGui::PushStyleColor(ImGuiCol_Button,        ImVec4(0.18f, 0.28f, 0.52f, 1.0f));
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.26f, 0.40f, 0.70f, 1.0f));
    ImGui::PushStyleColor(ImGuiCol_ButtonActive,  ImVec4(0.14f, 0.22f, 0.42f, 1.0f));
    ImGui::Button("Save Changes", ImVec2(120.0f, 26.0f));
    ImGui::SameLine(0, 8);
    ImGui::Button("Restore Defaults", ImVec2(140.0f, 26.0f));
    ImGui::PopStyleColor(3);
}

} // namespace csopesy
