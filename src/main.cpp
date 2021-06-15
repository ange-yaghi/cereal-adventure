#include "../include/cereal_adventure_app.h"

#include <iostream>

int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nCmdShow) {
    (void)nCmdShow;
    (void)lpCmdLine;
    (void)hPrevInstance;
    
    c_adv::CerealAdventureApp app;
    app.Initialize((void *)&hInstance, ysContextObject::DeviceAPI::OpenGL4_0);
    app.Run(); 

    return 0;
}
