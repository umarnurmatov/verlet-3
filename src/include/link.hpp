#pragma once
#include <list>
#include "SFML/Graphics.hpp"
#include "verletObject.hpp"


struct Link
{
    VerletObject *v1, *v2;
    float  length;

    void apply();

    Link(VerletObject *_v1, VerletObject *_v2, float _length);

};