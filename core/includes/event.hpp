#pragma once
#include <array>
#include <functional>

#include "keys.hpp"

namespace waza3d {

	enum class EventType
	{
		WindowResize = 0,
		WindowClose,

		KeyPressed,
		KeyReleased,

		MouseButtonPressed,
		MouseButtonReleased,
		MouseMoved,

		EventsCount
	};

	/*Базовый ивент, от которого наследуются остальные*/
	struct BaseEvent {
		virtual ~BaseEvent() = default;
		virtual EventType getType() const = 0;
	};

	/*Диспетчер ивентов хранит функции обработки всех ивентов и контролирует их вызовы*/
	class EventDispatcher
	{
		std::array<std::function<void(BaseEvent&)>, static_cast<size_t>(EventType::EventsCount)> m_event_callbacks;
	public:
		/*Задает функцию-обработчик конкретного ивента*/
		template<typename T>
		void addEventListener(std::function<void(T&)> callback)
		{
			/*Создаем лямбду принимающую базовый тип, который внтури будет кастовать базовый тип в конкретный*/
			auto base_callback = [moved_callback = std::move(callback)](BaseEvent& event)
			{
				if (event.getType() == T::m_type)
				{
					moved_callback(static_cast<T&>(event));
				}
			};
			m_event_callbacks[static_cast<size_t>(T::m_type)] = std::move(base_callback);
		}

		/*Вызывает callback соответсвующий ивенту*/
		void dispatch(BaseEvent& event)
		{
			auto& callback = m_event_callbacks[static_cast<size_t>(event.getType())];
			if (callback)
			{
				callback(event);
			}
		}
	};

	/*Изменение размеров окна*/
	struct EventWindowResize : public BaseEvent {
		unsigned int m_width = 0;
		unsigned int m_height = 0;
		static const EventType m_type = EventType::WindowResize;

		EventWindowResize(unsigned int width, unsigned int height)
			:m_width(width), m_height(height) {}

		virtual EventType getType() const override
		{
			return m_type;
		}
	};

	/*Закрытие окна*/
	struct EventWindowClose : public BaseEvent {
		static const EventType m_type = EventType::WindowClose;

		virtual EventType getType() const override
		{
			return m_type;
		}
	};

	/*Перемещение мыши*/
	struct EventMouseMoved : public BaseEvent {
		double m_x = 0.0;
		double m_y = 0.0;
		static const EventType m_type = EventType::MouseMoved;

		EventMouseMoved(double new_x, double new_y)
			:m_x(new_x), m_y(new_y) {}

		virtual EventType getType() const override
		{
			return m_type;
		}
	};

	/*Нажатие клавиши*/
	struct EventKeyPressed : public BaseEvent {
		static const EventType m_type = EventType::KeyPressed;
		KeyCode m_key_code;
		bool m_repeat;


		EventKeyPressed(const KeyCode key_code, const bool repeat)
			:m_key_code(key_code), m_repeat(repeat) {}

		virtual EventType getType() const override
		{
			return m_type;
		}
	};

	/*Отжатие клавиши клавиши*/
	struct EventKeyReleased : public BaseEvent {
		static const EventType m_type = EventType::KeyReleased;
		KeyCode m_key_code;

		EventKeyReleased(const KeyCode key_code)
			:m_key_code(key_code){}

		virtual EventType getType() const override
		{
			return m_type;
		}
	};
	
	/*Нажатие кнопки мыши*/
	struct EventMouseButtonPressed : public BaseEvent {
		static const EventType m_type = EventType::MouseButtonPressed;
		MouseButtonCode m_mouse_button_code;
		double m_x;
		double m_y;


		EventMouseButtonPressed(const MouseButtonCode mouse_button_code, const double pos_x, const double pos_y)
			:m_mouse_button_code(mouse_button_code), m_x(pos_x), m_y(pos_y) {}

		virtual EventType getType() const override
		{
			return m_type;
		}
	};

	/*Отжатие кнопки мыши*/
	struct EventMouseButtonReleased : public BaseEvent {
		static const EventType m_type = EventType::MouseButtonReleased;
		MouseButtonCode m_mouse_button_code;
		double m_x;
		double m_y;


		EventMouseButtonReleased(const MouseButtonCode mouse_button_code, const double pos_x, const double pos_y)
			:m_mouse_button_code(mouse_button_code), m_x(pos_x), m_y(pos_y) {}

		virtual EventType getType() const override
		{
			return m_type;
		}
	};
}