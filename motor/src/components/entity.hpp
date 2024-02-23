#pragma once

struct Entity : public std::enable_shared_from_this<Entity>{
    virtual void draw(glm::mat4) const {};
};

struct Light : Entity{
    void draw(glm::mat4) const override {};
};

struct Camera : Entity{
    void draw(glm::mat4) const override {};
};