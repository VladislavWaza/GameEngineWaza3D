#include <iostream>

#include <application.hpp>

class Editor : public waza3d::Application
{
	int frame = 0;
public:
	virtual void on_update() override
	{
		std::cout << "Frame: " << frame++ << std::endl;
	}
};


int main()
{
	auto editor = std::make_unique<Editor>();

	int returnCode = editor->start(1024, 576, "Editor");

	std::cin.get();

	return returnCode;
}