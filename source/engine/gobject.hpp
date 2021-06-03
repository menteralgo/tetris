#pragma once

#include <cstdint>
#include <vector>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "gmath.hpp"

namespace gn
{
    static uint32_t genId = 0;

    class Actor;
    class Bounds;

    class gobject
    {
        uint32_t id;

    public:
        gobject() : id(genId++) { }

        uint32_t getId() { return id; }
    };

    class Component : public gobject
    {
    protected:
        Actor *actor;
        
    public:
        void setActor(Actor *actor) { this->actor = actor; }
        virtual void update(float deltaTime) { };
    };

    class RenderComponent : public Component
    {
    public:
        uint16_t order;
        bool isVisible = true;

        RenderComponent();

        virtual void render() = 0;
    };

    class Bounds : public Component
    {
        vector2 _size;
        vector2 _pivot;
        vector2 _point1;
        vector2 _point2;

    public:
        Bounds(vector2 size);
        Bounds(vector2 size, vector2 pivot);

        vector2 size() { return _size; }
        vector2 point1() { return _point1; }
        vector2 point2() { return _point2; }

        void setSize(vector2 size);
    };

    class Actor : public gobject
    {
        std::vector<Component*> components;

    public:
        Actor();
        ~Actor();

        vector2 position;

        void addComponent(Component *component);
        void removeComponent(Component *component);
        
        template <typename T>
        T * getComponent();

        virtual void update(float deltaTime);
    };
}