#pragma once

struct GLFWwindow;
namespace waza3d {

	class UIModule {

	public:
		/*Иницализация интерфейса и подключение обработчика событий к интерфейсу
		должна вызываться после создания окна*/
		static void init(GLFWwindow* window);

		/*Уничтожаем контекст
		должна вызываться при уничтожении окна*/
		static void shutdown();

		/*Начинает новый кадр интерфейса*/
		static void newFrame();

		/*Рисует кадр интерфейса*/
		static void draw();
	};

}