#pragma once
#include <ExCore/Application.h>
#include <ExCore/Rendering/Texture.h>
#include <ExCore/Rendering/Sprite.h>

class ExampleApp : public ExCore::Application {
public:
	ExampleApp(const ExCore::AppSpecs& specs);

protected:
	void start() override;
	void doGui() override;

private:


	std::shared_ptr<ExCore::Texture> tex;
	std::shared_ptr<ExCore::Sprite> sprite;
};