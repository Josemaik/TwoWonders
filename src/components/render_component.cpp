#include "render_component.hpp"

void RenderComponent::destroyMesh()
{
    //UnloadModel(model);
    meshLoaded = false;
    if (node) {
        for (auto& child : node->getChildren()) {
            if (auto pLight = child->getEntity<DarkMoon::PointLight>())
                pLight->enabled = false;
            //if(auto sLight = child->getEntity<DarkMoon::SpotLight>())
            //    sLight->enabled = false;
        }
        //node->destroy();
        node = nullptr;
    }
}

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

void RenderComponent::setOrientation(double o)
{
    orientation = o;
}

void RenderComponent::setRotationVec(vec3d r)
{
    rotationVec.setX(r.x());
    rotationVec.setY(r.y());
    rotationVec.setZ(r.z());
}