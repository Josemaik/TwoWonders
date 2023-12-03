#include "render_component.hpp"

void RenderComponent::setAll(vec3f p, vec3f s, Color c)
{
    position.setX(p.x());
    position.setY(p.y());
    position.setZ(p.z());

    scale.setX(s.x());
    scale.setY(s.y());
    scale.setZ(s.z());

    color = c;
}

void RenderComponent::setPosition(vec3f p)
{
    position.setX(p.x());
    position.setY(p.y());
    position.setZ(p.z());
}

void RenderComponent::setScale(vec3f s)
{
    scale.setX(s.x());
    scale.setY(s.y());
    scale.setZ(s.z());
}

void RenderComponent::setColor(Color c)
{
    color = c;
}