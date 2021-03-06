#include "Win32Window.h"

#include "IRenderer.h"

namespace voxl
{
    LONG Window_Win32::m_WindowStyle = WS_OVERLAPPEDWINDOW | WS_BORDER;

    Window_Win32::Window_Win32(IRenderer& a_Renderer) : m_Dimensions(0, 0), m_MousePos(0, 0), m_Closed(false), m_Hwnd(nullptr), m_HasFocus(false), m_CursorHidden(false), m_Renderer(a_Renderer), m_IsFullScreen(false)
    {

    }


    HWND Window_Win32::GetHwnd()
    {
        return m_Hwnd;
    }

    LRESULT CALLBACK Window_Win32::WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
    {
        using namespace utilities;
        Window_Win32* windowPtr = reinterpret_cast<Window_Win32*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));

        switch (message)
        {
        case WM_CREATE:
        {
            // Save the DXSample* passed in to CreateWindow.
            LPCREATESTRUCT pCreateStruct = reinterpret_cast<LPCREATESTRUCT>(lParam);
            SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pCreateStruct->lpCreateParams));

            //Finally calculate the screen dimensions and possibly update the swapchain size.
            if (windowPtr) windowPtr->OnMoveResize();
        }
        return 0;
        case WM_SYSKEYDOWN:
        case WM_KEYDOWN:
            if (windowPtr)
            {
                windowPtr->m_InputQueue.addKeyboardEvent(KeyboardEvent(KeyboardAction::KEY_PRESSED, static_cast<std::uint16_t>(wParam)));
            }
            return 0;
        case WM_SYSKEYUP:
        case WM_KEYUP:
            if (windowPtr)
            {
                windowPtr->m_InputQueue.addKeyboardEvent(KeyboardEvent(KeyboardAction::KEY_RELEASED, static_cast<std::uint16_t>(wParam)));
            }
            return 0;
        case WM_MOUSEMOVE:
        {
            if (windowPtr)
            {
                //Retrieve the current cursor position in screen space.
                POINT cursorPos;
                GetCursorPos(&cursorPos);

                //Non relative old position of the mouse.
                const glm::vec2 oldPos = windowPtr->m_MousePos;

                windowPtr->m_MousePos = { cursorPos.x, cursorPos.y };

                float dX = cursorPos.x - oldPos.x;
                float dY = cursorPos.y - oldPos.y;

                if (dX != 0)
                {
                    windowPtr->m_InputQueue.addMouseEvent(MouseEvent(MouseAction::MOVE_X, dX, MouseButton::NONE));
                }
                if (dY != 0)
                {
                    windowPtr->m_InputQueue.addMouseEvent(MouseEvent(MouseAction::MOVE_Y, dY, MouseButton::NONE));
                }

                //Finally set the cursor back to the center if it is captured.
                if (windowPtr->m_Settings.captureCursor && windowPtr->m_HasFocus)
                {
                    auto clientRect = windowPtr->GetClientCoordinates();

                    //Set the cursor to the center of the screen when focused.
                    windowPtr->CenterCursor(clientRect);
                }
            }
        }
        return 0;
        case WM_LBUTTONDOWN:
        {
            if (windowPtr)
            {
                //Set focus if the window is clicked and the mouse is within the window.
                if (!windowPtr->m_HasFocus)
                {
                    const auto clientRect = windowPtr->GetClientCoordinates();
                    POINT cursorPos;
                    GetCursorPos(&cursorPos);

                    if (cursorPos.x >= clientRect.left && cursorPos.x <= clientRect.right && cursorPos.y <= clientRect.bottom && cursorPos.y >= clientRect.top)
                    {
                        windowPtr->ObtainFocus();
                    }
                }
                windowPtr->m_InputQueue.addMouseEvent(MouseEvent(MouseAction::CLICK, 0, MouseButton::LMB));
            }
        }
        return 0;
        case WM_RBUTTONDOWN:
        {
            if (windowPtr)
            {
                //Set focus if the window is clicked and the mouse is within the window.
                if (!windowPtr->m_HasFocus)
                {
                    const auto clientRect = windowPtr->GetClientCoordinates();
                    POINT cursorPos;
                    GetCursorPos(&cursorPos);

                    if (cursorPos.x >= clientRect.left && cursorPos.x <= clientRect.right && cursorPos.y <= clientRect.bottom && cursorPos.y >= clientRect.top)
                    {
                        windowPtr->ObtainFocus();
                    }
                }
                windowPtr->m_InputQueue.addMouseEvent(MouseEvent(MouseAction::CLICK, 0, MouseButton::RMB));
            }
        }
        return 0;
        case WM_MBUTTONDOWN:
        {
            if (windowPtr)
            {
                //Set focus if the window is clicked and the mouse is within the window.
                if (!windowPtr->m_HasFocus)
                {
                    const auto clientRect = windowPtr->GetClientCoordinates();
                    POINT cursorPos;
                    GetCursorPos(&cursorPos);

                    if (cursorPos.x >= clientRect.left && cursorPos.x <= clientRect.right && cursorPos.y <= clientRect.bottom && cursorPos.y >= clientRect.top)
                    {
                        windowPtr->ObtainFocus();
                    }
                }
                windowPtr->m_InputQueue.addMouseEvent(MouseEvent(MouseAction::CLICK, 0, MouseButton::MMB));
            }
        }
        return 0;
        case WM_LBUTTONUP:
        {
            if (windowPtr)
            {
                windowPtr->m_InputQueue.addMouseEvent(MouseEvent(MouseAction::RELEASE, 0, MouseButton::LMB));
            }
        }
        return 0;
        case WM_RBUTTONUP:
        {
            if (windowPtr)
            {
                windowPtr->m_InputQueue.addMouseEvent(MouseEvent(MouseAction::RELEASE, 0, MouseButton::RMB));
            }
        }
        return 0;
        case WM_MBUTTONUP:
        {
            if (windowPtr)
            {
                windowPtr->m_InputQueue.addMouseEvent(MouseEvent(MouseAction::RELEASE, 0, MouseButton::MMB));
            }
        }
        return 0;
        case WM_MOUSEWHEEL:
        {
            if (windowPtr)
            {
                float moved = static_cast<float>(static_cast<short>(HIWORD(wParam))) / static_cast<float>(WHEEL_DELTA);
                windowPtr->m_InputQueue.addMouseEvent(MouseEvent(MouseAction::SCROLL, moved, MouseButton::MMB));
            }
        }
        return 0;
        case WM_EXITSIZEMOVE:
            if (windowPtr)
            {
                windowPtr->OnMoveResize();
            }
            return 0;
        case WM_SETCURSOR:
            //Hide the cursor if it's within the screen and it is focused.
            if (windowPtr)
            {
                if (windowPtr->m_Settings.hideCursor && windowPtr->m_HasFocus)
                {
                    if (!windowPtr->m_CursorHidden)
                    {

                        if (LOWORD(lParam) == HTCLIENT)
                        {
                            windowPtr->m_CursorHidden = true;
                            windowPtr->m_OldCursor = SetCursor(NULL);
                        }
                    }
                }
                else if (windowPtr->m_CursorHidden)
                {
                    SetCursor(windowPtr->m_OldCursor);
                    windowPtr->m_CursorHidden = false;
                }
            }
            return 0;
        case WM_SETFOCUS:
            return 0;
        case WM_KILLFOCUS:
            if (windowPtr)
            {
                windowPtr->m_HasFocus = false;
                if (windowPtr->m_Settings.captureCursor)
                {
                    ClipCursor(NULL);
                }
            }
            return 0;
        case WM_DESTROY:
            PostQuitMessage(0);
            return 0;
        }

        // Handle any messages the switch statement didn't.
        return DefWindowProc(hWnd, message, wParam, lParam);
    }

    void Window_Win32::Resize(glm::vec2 a_Dimensions)
    {
        SetWindowPos(m_Hwnd, HWND_TOP, 0, 0, static_cast<int>(a_Dimensions.x), static_cast<int>(a_Dimensions.y), SWP_NOMOVE);

        //Make sure the swapchain and cursor capturing updates.
        OnMoveResize();
    }

    glm::vec2 Window_Win32::GetDimensions()
    {
        return m_Dimensions;
    }

    void Window_Win32::SetFullScreen(bool a_FullScreen)
    {
        if (m_IsFullScreen != a_FullScreen)
        {
            m_IsFullScreen = a_FullScreen;

            //To windowed mode.
            if (!m_IsFullScreen)
            {
                //Set the Window back to normal mode with the stored sizes.
                SetWindowLong(m_Hwnd, GWL_STYLE, m_WindowStyle);

                SetWindowPos(
                    m_Hwnd,
                    HWND_NOTOPMOST,
                    m_WindowedRect.left,
                    m_WindowedRect.top,
                    m_WindowedRect.right - m_WindowedRect.left,
                    m_WindowedRect.bottom - m_WindowedRect.top,
                    SWP_FRAMECHANGED | SWP_NOACTIVATE);

                ShowWindow(m_Hwnd, SW_NORMAL);
            }
            //To full screen mode.
            else
            {
                //Store the current window rect to be set again when exiting full screen mode.
                GetWindowRect(m_Hwnd, &m_WindowedRect);

                // Make the window borderless so that the client area can fill the screen.
                SetWindowLong(m_Hwnd, GWL_STYLE, m_WindowStyle & ~(WS_CAPTION | WS_MAXIMIZEBOX | WS_MINIMIZEBOX | WS_SYSMENU | WS_THICKFRAME));

                // Get the settings of the primary display
                DEVMODE devMode = {};
                devMode.dmSize = sizeof(DEVMODE);
                EnumDisplaySettings(nullptr, ENUM_CURRENT_SETTINGS, &devMode);

                RECT fullscreenWindowRect = {
                    devMode.dmPosition.x,
                    devMode.dmPosition.y,
                    devMode.dmPosition.x + static_cast<LONG>(devMode.dmPelsWidth),
                    devMode.dmPosition.y + static_cast<LONG>(devMode.dmPelsHeight)
                };

                SetWindowPos(
                    m_Hwnd,
                    HWND_TOPMOST,
                    fullscreenWindowRect.left,
                    fullscreenWindowRect.top,
                    fullscreenWindowRect.right,
                    fullscreenWindowRect.bottom,
                    SWP_FRAMECHANGED | SWP_NOACTIVATE);


                ShowWindow(m_Hwnd, SW_MAXIMIZE);
            }

            //Resize the buffers and clip the cursor if needed.
            OnMoveResize();

            //Finally set the focus of the window.
            SetFocus(m_Hwnd);

        }
    }

    bool Window_Win32::IsFullScreen()
    {
        return m_Settings.captureCursor;
    }

    void Window_Win32::SetNativeMousePosition(const glm::vec2& a_MousePosition)
    {
        m_MousePos = a_MousePosition;
        SetCursorPos(static_cast<int>(m_MousePos.x), static_cast<int>(m_MousePos.y));
    }

    glm::vec2 Window_Win32::GetNativeMousePosition() const
    {
        return m_MousePos;
    }

    utilities::InputData Window_Win32::PollInput()
    {
        // Main sample loop.
        MSG msg = { 0 };

        //Loop over messages till none are left.
        while (PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);

            //Paint command issued. Stop processing shit and draw damn it!
            if (msg.message == WM_PAINT)
            {
                return m_InputQueue.getQueuedEvents();
            }

            //Quit called, quit the game.
            if (msg.message == WM_QUIT)
            {
                m_Closed = true;
                return m_InputQueue.getQueuedEvents();
            }
        }

        return m_InputQueue.getQueuedEvents();
    }

    bool Window_Win32::IsClosed() const
    {
        return m_Closed;
    }

    void Window_Win32::Close()
    {
        DestroyWindow(m_Hwnd);
    }

    RECT Window_Win32::GetClientCoordinates() const
    {
        RECT rect;
        GetClientRect(m_Hwnd, &rect);

        ClientToScreen(m_Hwnd, reinterpret_cast<POINT*>(&rect.left)); // convert top-left
        ClientToScreen(m_Hwnd, reinterpret_cast<POINT*>(&rect.right)); // convert bottom-right

        return rect;
    }

    RECT Window_Win32::GetWindowCoordinates() const
    {
        RECT rect;
        GetWindowRect(m_Hwnd, &rect);
        return rect;
    }

    void Window_Win32::CenterCursor(RECT a_ClientRect)
    {
        //Set the cursor to the center of the screen when focused.
        int centerX = a_ClientRect.left + ((a_ClientRect.right - a_ClientRect.left) / 2);
        int centerY = a_ClientRect.top + ((a_ClientRect.bottom - a_ClientRect.top) / 2);
        m_MousePos = { centerX, centerY };
        SetCursorPos(centerX, centerY);
    }

    void Window_Win32::OnMoveResize()
    {
        //Size
        const auto windowRect = GetWindowCoordinates();

        const int width = windowRect.right - windowRect.left;
        const int height = windowRect.bottom - windowRect.top;

        //Size has changed.
        if (width != static_cast<int>(m_Dimensions.x) || height != static_cast<int>(m_Dimensions.y))
        {
            const glm::vec2 newDims = { width, height };
            m_Dimensions = newDims;

            //Notify the rendering system.
            m_Renderer.ResizeSwapChain(newDims);

            //Notify the callback.
            if(m_ResizeCallback != nullptr)
            {
                m_ResizeCallback(static_cast<int>(m_Dimensions.x), static_cast<int>(m_Dimensions.y));
            }
        }

        //Clip the cursor to the window if focus is still active.
        if (m_HasFocus)
        {
            if (m_Settings.captureCursor)
            {
                auto clientRect = GetClientCoordinates();

                //Set the cursor to the center of the screen when focused.
                CenterCursor(clientRect);

                ClipCursor(&clientRect);
            }
        }
    }

    void Window_Win32::ObtainFocus()
    {
        m_HasFocus = true;
        SetFocus(m_Hwnd);

        //Set the cursor to clip with the current area.
        if (m_Settings.captureCursor)
        {
            //Get the screen rectangle.
            auto clientRect = GetClientCoordinates();

            //Set the cursor to the center of the screen when focussed.
            CenterCursor(clientRect);

            //Clip the cursor in the screen.
            ClipCursor(&clientRect);
        }
    }

    void Window_Win32::SetResizeCallback(std::function<void(int a_Width, int a_Height)> a_Function)
    {
        m_ResizeCallback = std::move(a_Function);
    }

    bool Window_Win32::Initialize(const WindowSettings& a_Settings)
    {
        m_Settings = a_Settings;

        //Retrieve the current instance.
        const auto hInstance = GetModuleHandle(NULL);

        WNDCLASSEX windowClass = { 0 };
        windowClass.cbSize = sizeof(WNDCLASSEX);
        windowClass.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
        windowClass.lpfnWndProc = WindowProc;
        windowClass.hInstance = hInstance;
        windowClass.hCursor = LoadCursor(NULL, IDC_ARROW);
        windowClass.lpszClassName = L"Win32WindowClass";
        RegisterClassEx(&windowClass);

        RECT windowRect = { 0, 0, static_cast<LONG>(a_Settings.dimensions.x), static_cast<LONG>(a_Settings.dimensions.y) };
        AdjustWindowRect(&windowRect, m_WindowStyle, FALSE);

        auto title = std::wstring(a_Settings.name.begin(), a_Settings.name.end());

        // Create the window and store a handle to it.
        m_Hwnd = CreateWindow(
            windowClass.lpszClassName,
            title.c_str(),
            m_WindowStyle,
            CW_USEDEFAULT,
            CW_USEDEFAULT,
            windowRect.right - windowRect.left,
            windowRect.bottom - windowRect.top,
            nullptr,
            nullptr,
            hInstance,
            this);

        //Display the window.
        ShowWindow(m_Hwnd, SW_SHOW);

        //If the window should start in fullscreen mode, do that now.
        if (a_Settings.fullScreen)
        {
            SetFullScreen(true);
        }

        //Finally set the window focus.
        //First set to null to ensure it resets the focus state.
        SetFocus(NULL);
        SetFocus(m_Hwnd);
        ObtainFocus();

        return true;
    }
}
