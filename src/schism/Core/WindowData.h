#pragma once
#include <functional>
#include <array>
#include <glm/vec2.hpp>


#include "Events/Event.h"

namespace Schism
{
	using EventCallback = std::function<void(Event&& e)>;

	struct InputData
	{
		std::array<bool, 400> PressedKeys;
		glm::vec2 MousePosition;
		struct
		{
			bool Left;
			bool Right;
			bool Middle;
		} MouseButton;
	};
	
	struct WindowData
	{
		EventCallback OnEvent;
		InputData Input;
		int Width;
		int Height;
	};
}
