#pragma once
#include <list>
#include "SFML/Graphics.hpp"
#include "verletObject.hpp"


struct Link
{
    VerletObject *v1, *v2;
    float  length;

    Link(VerletObject *_v1, VerletObject *_v2, float _length)
    : v1{_v1},
      v2{_v2},
      length{_length}
    {
    }

    void apply()
    {
        sf::Vector2f axis = v2->pos_current - v1->pos_current;
        float diff = axis.length() - length;
        axis = axis.normalized();
        
        if(!v1->fixed) v1->pos_current += 0.5f * axis * diff;
        if(!v2->fixed) v2->pos_current -= 0.5f * axis * diff;
    }

};