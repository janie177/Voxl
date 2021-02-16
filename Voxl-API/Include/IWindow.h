#pragma once
#include <functional>
#include <glm/glm.hpp>

namespace voxl
{
    /*
     * Window related settings.
     */
    struct WindowSettings
    {
        glm::ivec2 dimensions = { 400, 600 };
        bool fullScreen = false;
        bool hideCursor = false;
        bool lockCursor = false;
        bool allowManualResize = true;
    };

    /*
     * Window interface with basic interactions.
     */
    class IWindow
    {
    public:
        virtual ~IWindow() = default;

        /*
         * Initialize the window.
         */
        virtual void Init(const WindowSettings& a_Settings) = 0;

        /*
         * Set the window settings.
         */
        virtual void SetSettings(const WindowSettings& a_Settings) = 0;

        /*
         * Set the callback that is invoked when the windows dimensions change due to user input.
         */
        virtual void SetResizeCallback(std::function<void(int a_X, int a_Y, bool a_FullScreen)>& a_Function) = 0;

        /*
         * Get the dimensions of the window.
         */
        virtual glm::ivec2 GetDimensions() = 0;

        /*
         * Resize the window to new dimensions.
         */
        virtual void Resize(const glm::ivec2& a_Dimensions) = 0;

        /*
         * When true, the window will be made full-screen resolution.
         */
        virtual void SetFullScreen(bool a_FullScreen) = 0;

        /*
         * When true, the cursor will be hidden.
         */
        virtual void SetHideCursor(bool a_HideCursor) = 0;

        /*
         * When true, the cursor will be locked to the center of the window.
         */
        virtual void SetLockCursor(bool a_CaptureCursor) = 0;

        /*
         * When true, the user is allowed to manually resize the window.
         */
        virtual void SetAllowManualResize(bool a_AllowResize) = 0;
    };
}
