#include "../include/cereal_adventure_app.h"

#include <iostream>

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    (void)nCmdShow;
    (void)lpCmdLine;
    (void)hPrevInstance;

    c_adv::CerealAdventureApp app;
    app.Initialize((void *)&hInstance, ysContextObject::OPENGL4_0);
    app.Run();

    return 0;
}
