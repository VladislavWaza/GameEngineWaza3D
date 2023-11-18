#pragma once
#include <array>
#include <functional>

namespace waza3d {

	enum class EventType
	{
		WindowResize = 0,
		WindowClose,

		//KeyPressed,
		//KeyReleased,

		//MouseButtonPressed,
		//MouseButtonReleased,
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

}