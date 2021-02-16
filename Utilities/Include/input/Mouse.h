#pragma once
#include <cinttypes>

namespace utilities
{
	/*
	 * Small helper class to store a mouse position. This is essentially a 2D vector with limited functionality.
	 */
	class MousePos
	{
	public:
		MousePos(float x, float y) : x(x), y(y) {}
		MousePos() : x(0.f), y(0.f) {}

	public:
		MousePos operator +(const MousePos& rhs) const
		{
			return MousePos(rhs.x + x, rhs.y + y);
		}

		MousePos operator -(const MousePos& rhs) const
		{
			return (*this + MousePos(-rhs.x, -rhs.y));
		}

	public:
		float x, y;
	};

	/*
	 * Mouse class which keeps track of where the cursor is.
	 * The cursor position can be set and moved.
	 * The cursor position is always checked to be within the bounds given by width and height.
	 *
	 * It is not guaranteed that the cursor stays within bounds if the bounds are resized.
	 * A manual call to setPosition is then required.
	 */
	class Mouse
	{
	public:
		/*
		 * Init the mouse with the given screen bounds.
		 */
		Mouse(std::uint32_t width, std::uint32_t height, std::uint32_t startX, std::uint32_t startY);

		/*
		 * Get the dimensions of the mouse.
		 * This is the size in pixels which should match the screen size.
		 */
		MousePos getDimensions() const;

		/*
		 * Get the current position of the mouse.
		 */
		MousePos getPosition() const;

		/*
		 * Set the position of the mouse.
		 */
		void setPosition(const MousePos& position);

		/*
		 * Add to the position of the mouse.
		 */
		void addPosition(const MousePos& added);

		/*
		 * Set the dimensions of the screen.
		 */
		void setDimensions(const MousePos& dimensions);

		/*
		 * Get the current mouse X coordinate.
		 */
		std::uint32_t getPositionX() const;

		/*
		 * Get the current mouse Y coordinate.
		 */
		std::uint32_t getPositionY() const;

		/*
		 * Get the size in pixels on the horizontal axis that this mouse can move on.
		 */
		std::uint32_t getDimensionX() const;

		/*
		 * Get the size in pixels on the vertical axis that this mouse can move on.
		 */
		std::uint32_t getDimensionY() const;

	private:
		/*
		 * Cap the given position to be within the bounds of 0-width and 0-height.
		 * The value cap is given by the dimensions MousePos.
		 */
		MousePos capBounds(const MousePos& position) const;

	private:
		MousePos dimensions;
		MousePos position;
	};
}