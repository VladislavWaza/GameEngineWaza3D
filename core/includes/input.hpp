#pragma once

#include "keys.hpp"

namespace waza3d {
	class Input {
		static bool m_keys_pressed[];
		static bool m_mouse_buttons_pressed[];
	public:
		static bool isKeyPressed(const KeyCode key_code);
		static void PressKey(const KeyCode key_code);
		static void ReleaseKey(const KeyCode key_code);

		static bool isMouseButtonPressed(const MouseButtonCode mouse_botton_code);
		static void PressMouseButton(const MouseButtonCode mouse_botton_code);
		static void ReleaseMouseButton(const MouseButtonCode mouse_botton_code);
	};

}