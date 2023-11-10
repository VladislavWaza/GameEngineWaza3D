#pragma once

namespace waza3d{

	/* ƒл€ использовни€ класса нужно переопределить функцию void on_update()
	*/

	class Application 
	{
	public:
		Application();
		virtual ~Application();


		// ласс должен быть в одном экземпл€ре и не может копироватьс€
		Application(const Application&) = delete;
		Application(Application&&) = delete;
		Application& operator=(const Application&) = delete;
		Application& operator=(Application&&) = delete;

		virtual int start(unsigned int width, unsigned int height, const char* title);

		virtual void on_update() {};
	};
}