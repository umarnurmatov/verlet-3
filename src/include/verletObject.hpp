#pragma once
#include "SFML/System/Vector2.hpp"
#include <cmath>

struct VerletObject
{
    sf::Vector2f pos_current;
    sf::Vector2f pos_prev;
    sf::Vector2f acceleration;

    float radius;
    float mass;

    bool fixed;

    VerletObject(sf::Vector2f _position, float _radius, float _mass, bool _fixed = false)
        : pos_current{_position},
          pos_prev{_position},
          radius{_radius},
          mass{_mass},
          fixed{_fixed}
    {
    }

    void updatePosition(float dt)
    {
        if(fixed) return;
        const sf::Vector2f vel = pos_current - pos_prev;
        pos_prev = pos_current;
        // Verlet integration
        pos_current = pos_current + vel + acceleration * dt * dt;
        // reset acceleration
        acceleration = sf::Vector2f();
    }

    void accelerate(sf::Vector2f da)
    {
        acceleration += da;
    }

    float distance(VerletObject *v2)
    {
        return sqrt(pow(pos_current.x - v2->pos_current.x, 2) + pow(pos_current.y - v2->pos_current.y, 2));
    }

};