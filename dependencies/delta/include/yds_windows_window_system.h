#ifndef YDS_WINDOWS_WINDOW_SYSTEM_H
#define YDS_WINDOWS_WINDOW_SYSTEM_H

#include "yds_window_system.h"

#include <Windows.h>

class ysWindowsWindowSystem : public ysWindowSystem {
public:
    ysWindowsWindowSystem();
    ~ysWindowsWindowSystem();

    virtual ysError NewWindow(ysWindow **newWindow);

    virtual ysMonitor *NewMonitor();
    virtual void SurveyMonitors();

    virtual void ProcessMessages();

    ysWindow *FindWindowFromHandle(HWND handle);

    virtual void ConnectInstance(void *genericInstanceConnection);

    static LRESULT WINAPI WinProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

protected:
    HINSTANCE m_instance;
};

#endif /* YDS_WINDOWS_WINDOW_SYSTEM_H */
