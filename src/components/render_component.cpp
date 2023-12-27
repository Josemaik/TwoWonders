#include "render_component.hpp"

void RenderComponent::setAll(vec3d p, vec3d s, Color c)
{
    position.setX(p.x());
    position.setY(p.y());
    position.setZ(p.z());

    scale.setX(s.x());
    scale.setY(s.y());
    scale.setZ(s.z());

    color = c;
}

void RenderComponent::setPosition(vec3d p)
{
    position.setX(p.x());
    position.setY(p.y());
    position.setZ(p.z());
}

void RenderComponent::setScale(vec3d s)
{
    scale.setX(s.x());
    scale.setY(s.y());
    scale.setZ(s.z());
}

void RenderComponent::setColor(Color c)
{
    color = c;
}