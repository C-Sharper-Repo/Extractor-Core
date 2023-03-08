#include <ExCore/EntryPoint.h>
#include <ExampleApp.h>

using namespace ExCore;

Application* ExCore::createApplication(AppArgs args) {
	AppSpecs specs;
	specs.name = "Extractor Gui Example";
	specs.args = args;
	return new ExampleApp(specs);
}