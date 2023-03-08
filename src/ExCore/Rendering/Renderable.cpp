#include <ExCore/Rendering/Renderable.h>
#include <ExCore/Rendering/Renderer.h>

namespace ExCore {
    Renderable::Renderable() : _layer(), _color(Color32::White) {
        Renderer::addRenderable(this);
    } 
    
    Renderable::~Renderable() { Renderer::removeRenderable(this); }
}