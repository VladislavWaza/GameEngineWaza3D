#pragma once

namespace waza3d{

	/* ��� ������������ ������ ����� �������������� ������� void on_update()
	*/

	class Application 
	{
	public:
		Application();
		virtual ~Application();


		//����� ������ ���� � ����� ���������� � �� ����� ������������
		Application(const Application&) = delete;
		Application(Application&&) = delete;
		Application& operator=(const Application&) = delete;
		Application& operator=(Application&&) = delete;

		virtual int start(unsigned int width, unsigned int height, const char* title);

		virtual void on_update() {};
	};
}