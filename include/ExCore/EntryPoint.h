#pragma once 
#include <ExCore/Application.h>

extern ExCore::Application* ExCore::createApplication(AppArgs args);

int main(int argc, char** argv)
{
	ExCore::Log::init();
	EX_CORE_TRACE("Initializing Extractor Core application");

	auto app = ExCore::createApplication({ argc, argv });
	if (app->initialize()) {
		app->run();
	}

	EX_CORE_TRACE("Destroying Extractor Core");
	delete app;
}
