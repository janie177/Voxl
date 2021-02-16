#include "input/Mouse.h"
#include <cmath>

namespace utilities
{
	Mouse::Mouse(std::uint32_t width, std::uint32_t height, std::uint32_t startX, std::uint32_t startY) : dimensions(width, height), position(0.f, 0.f)
	{
		setPosition(capBounds(MousePos(startX, startY)));
	}

	MousePos Mouse::getDimensions() const
	{
		return dimensions;
	}

	MousePos Mouse::getPosition() const
	{
		return position;
	}

	void Mouse::setPosition(const MousePos& position)
	{
		this->position = capBounds(position);
	}

	void Mouse::addPosition(const MousePos& added)
	{
		this->position = capBounds(position + added);
	}

	void Mouse::setDimensions(const MousePos& dimensions)
	{
		this->dimensions = dimensions;
	}

	std::uint32_t Mouse::getPositionX() const
	{
		return static_cast<std::uint32_t>(position.x);
	}

	std::uint32_t Mouse::getPositionY() const
	{
		return static_cast<std::uint32_t>(position.y);
	}

	std::uint32_t Mouse::getDimensionX() const
	{
		return static_cast<std::uint32_t>(dimensions.x);
	}

	std::uint32_t Mouse::getDimensionY() const
	{
		return static_cast<std::uint32_t>(dimensions.y);
	}

	MousePos Mouse::capBounds(const MousePos& position) const
	{
		return MousePos(std::fmin(std::fmax(0.f, position.x), dimensions.x), std::fmin(std::fmax(0.f, position.y), dimensions.y));
	}
}