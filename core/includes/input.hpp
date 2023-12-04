#pragma once

#include "keys.hpp"

namespace waza3d {
	class Input {
		static bool m_keys_pressed[static_cast<size_t>(KeyCode::KEY_LAST)];
	public:
		static bool isKeyPressed(const KeyCode key_code);
		static void PressKey(const KeyCode key_code);
		static void ReleaseKey(const KeyCode key_code);
	};

}