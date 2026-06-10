#include "core/GUIApplication.h"

int main() {
    csopesy::GUIApplication app;

    if (!app.initialize()) {
        return 1;
    }

    app.run();

    return 0;
}