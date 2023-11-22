#include "render_component.hpp"

void RenderComponent::setAll(Vector3 v, Vector3 s, Color c)
{
    position.x = v.x;
    position.y = v.y;
    position.z = v.z;

    scale.x = s.x;
    scale.y = s.y;
    scale.z = s.z;

    color = c;
}

void RenderComponent::setPosition(Vector3 v)
{
    position.x = v.x;
    position.y = v.y;
    position.z = v.z;
}

void RenderComponent::setScale(Vector3 v)
{
    scale.x = v.x;
    scale.y = v.y;
    scale.z = v.z;
}

void RenderComponent::setColor(Color c)
{
    color = c;
}

BoundingBox RenderComponent::getBoundingBox()
{
    Vector3 min = { position.x - scale.x / 2, position.y - scale.y / 2, position.z - scale.z / 2 };
    Vector3 max = { position.x + scale.x / 2, position.y + scale.y / 2, position.z + scale.z / 2 };

    return BoundingBox{ min, max };
}