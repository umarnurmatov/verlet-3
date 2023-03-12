#include "link.hpp"

void Link::apply()
{
    sf::Vector2f axis = v2->pos_current - v1->pos_current;
    float diff = axis.length() - length;
    axis = axis.normalized();
    
    if(!v1->fixed) v1->pos_current += 0.5f * axis * diff;
    if(!v2->fixed) v2->pos_current -= 0.5f * axis * diff;
}

Link::Link(VerletObject *_v1, VerletObject *_v2, float _length)
    : v1{_v1},
      v2{_v2},
      length{_length}
{
}
