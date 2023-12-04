#include "input.hpp"

namespace waza3d {
	bool Input::m_keys_pressed[static_cast<size_t>(KeyCode::KEY_LAST) + 1] = {};
	bool Input::m_mouse_buttons_pressed[static_cast<size_t>(MouseButtonCode::MOUSE_BUTTON_LAST) + 1] = {};


	bool Input::isKeyPressed(const KeyCode key_code)
	{
		return m_keys_pressed[static_cast<size_t>(key_code)];
	}
	void Input::PressKey(const KeyCode key_code)
	{
		m_keys_pressed[static_cast<size_t>(key_code)] = true;
	}
	void Input::ReleaseKey(const KeyCode key_code)
	{
		m_keys_pressed[static_cast<size_t>(key_code)] = false;
	}
	bool Input::isMouseButtonPressed(const MouseButtonCode mouse_botton_code)
	{
		return m_mouse_buttons_pressed[static_cast<size_t>(mouse_botton_code)];
	}
	void Input::PressMouseButton(const MouseButtonCode mouse_botton_code)
	{
		m_mouse_buttons_pressed[static_cast<size_t>(mouse_botton_code)] = true;
	}
	void Input::ReleaseMouseButton(const MouseButtonCode mouse_botton_code)
	{
		m_mouse_buttons_pressed[static_cast<size_t>(mouse_botton_code)] = false;
	}
}