#pragma once
#include <functional>
#include <glm/glm.hpp>
#include <input/InputQueue.h>

namespace voxl
{
    /*
     * Window related settings.
     */
    struct WindowSettings
    {
        std::string name = "Window";
        glm::ivec2 dimensions = { 400, 600 };
        bool fullScreen = false;
        bool hideCursor = false;
        bool captureCursor = false;
        bool allowManualResize = true;
    };

    struct WindowSettings;
    class SwapChain;
    class RenderTarget;

    class IWindow
    {
    public:
        virtual ~IWindow() = default;

        /*
         * Set a callback that is called when the window resizes.
         *
         * Example format: SetResizeCallback([](int w, int h){ //CODE });
         */
        virtual void SetResizeCallback(std::function<void(int a_Width, int a_Height)> a_Function) = 0;

        /*
         * Close the window.
         */
        virtual void Close() = 0;

        /*
         * Resize this window to the specified dimensions.
         */
        virtual void Resize(glm::vec2 a_Dimensions) = 0;

        /*
         * Get the current dimensions of this window.
         */
        virtual glm::vec2 GetDimensions() = 0;

        /*
         * Returns whether this window is currently full screen mode.
         */
        virtual bool IsFullScreen() = 0;

        /*
         * Toggle fullscreen state on or off.
         */
        virtual void SetFullScreen(bool a_FullScreen) = 0;

        /*
         * Set the mouse position for this window.
         * This caps to be within 0,0 and width,height.
         *
         * This regards the native OS mouse.
         */
        virtual void SetNativeMousePosition(const glm::vec2& a_MousePosition) = 0;

        /*
         * Get the mouse position of the native OS mouse.
         */
        virtual glm::vec2 GetNativeMousePosition() const = 0;

        /*
         * Get a queue of all input events that happened since this was last called.
         */
        virtual utilities::InputData PollInput() = 0;

        /*
         * Returns whether this window has closed or not.
         */
        virtual bool IsClosed() const = 0;

        /*
         * Initialize the window with the given settings.
         */
        virtual bool Initialize(const WindowSettings& a_Settings) = 0;

        /*
         * Called when the window resizes.
         */
        virtual void OnResize(const glm::ivec2& a_NewSize) = 0;
    };
}
