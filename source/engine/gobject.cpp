#include "gobject.hpp"
#include "engine.hpp"
#include <algorithm>

namespace gn
{
    Bounds::Bounds(vector2 size)
    {
        _pivot = vector2(0.5f, 0.5f);
        setSize(size);
    }

    Bounds::Bounds(vector2 size, vector2 pivot)
    {
        _pivot = pivot;
        setSize(size);
    }

    void Bounds::setSize(const vector2 size)
    {
        _size = size;
        _point1 = vector2(-_size.x * _pivot.x, _size.y * _pivot.y);
        _point2 = vector2(_size.x * (1.f - _pivot.x), -_size.y * (1.f - _pivot.y));
    }

    RenderComponent::RenderComponent()
    {
        App::instance().addRenderer(this);
    }

    Actor::Actor()
    {
        App::instance().addActor(this);
    }

    Actor::~Actor()
    {
        for (Component *comp : components)
        {
            RenderComponent *rc = dynamic_cast<RenderComponent*>(comp);
            if (rc != nullptr)
                App::instance().removeRenderer(rc);

            delete comp;
        }
    }

    void Actor::addComponent(Component *component)
    {
        components.push_back(component);
        component->setActor(this);
    }

    void Actor::removeComponent(Component *component)
    {
        uint32_t id = component->getId();
        std::remove_if(components.begin(), components.end(), 
            [id](Component *comp) { return comp->getId() == id; });
    }

    template <class T>
    T * Actor::getComponent()
    {
        for (const Component &comp : components)
        {
            T target = dynamic_cast<T>(comp);
            if (target != nullptr)
                return target;
        }

        return nullptr;
    }

    void Actor::update(float deltaTime)
    {
        for (Component *comp : components)
            comp->update(deltaTime);
    }
}