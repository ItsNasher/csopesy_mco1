#pragma once

#include "core/AWindow.h"

namespace csopesy {

class Desktop : public AWindow {
public:
    Desktop();
    ~Desktop() override;

    void draw() override;

private:
    void loadWallpaper();
    void drawWallpaper();
    void drawClock();

    unsigned int wallpaperTexture = 0;
    int wallpaperWidth = 0;
    int wallpaperHeight = 0;
    bool wallpaperLoaded = false;
};

} // namespace csopesy