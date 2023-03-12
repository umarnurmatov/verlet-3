#include "solver.hpp"
#include <iostream>

VerletObject* Solver::addObject(sf::Vector2f position, float radius, float mass, bool fixed)
{
    return &m_objects.emplace_back(VerletObject(position, radius, mass, fixed));
}

VerletObject* Solver::addObject(VerletObject v)
{
    return &m_objects.emplace_back(v);
}

void Solver::addLink(VerletObject *v1, VerletObject *v2, float length)
{
    m_links.emplace_back(Link(v1, v2, length));
}

void Solver::update(float dt)
{
    const float sub_dt = dt / m_sub_steps;
    for(uint32_t i = 0; i < m_sub_steps; i++)
    {
        applyGravity();
        applyLinks();
        applyConstraint();
        solveCollision();
        updateObjects(sub_dt);
    }
}

[[nodiscard]] const std::list<VerletObject>& Solver::getObjects() const
{
    return m_objects;
}



void Solver::addRectangle(uint w_count, uint h_count, float r, float x, float y, float mass, bool fixed)
{
    float v_mass = mass / (w_count * h_count);

    std::vector<std::vector<int>> tr {{1, 0}, {0, 1}, {1, 1}};

    std::vector<std::vector<VerletObject*>> v;

    for (size_t i = 0; i < h_count; i++)
    {
        v.push_back(std::vector<VerletObject*>());
        for (size_t j = 0; j < w_count; j++)
        {
            v[i].push_back(addObject(sf::Vector2f(x + 2.f*j*r, y + 2.f*i*r), r, v_mass, fixed));
        }
    }

    for (size_t i = 0; i < h_count; i++)
    {
        for (size_t j = 0; j < w_count; j++)
        {
            for(auto t : tr)
            {
                if(i + t[0] < v.size() && j + t[1] < v[0].size())
                {
                    auto* vo1 = v[i][j];
                    auto* vo2 = v[i + t[0]][j + t[1]]; 
                    addLink(vo1, vo2, vo1->distance(vo2));
                }
            }
        }
    }
}


void Solver::applyGravity()
{
    for(auto& obj : m_objects)
    {
        obj.accelerate(m_gravity);
    }
}

void Solver::updateObjects(float dt)
{
    for(auto& obj : m_objects)
    {
        obj.updatePosition(dt);
    }
}

void Solver::applyLinks()
{
    for(auto& link : m_links)
    {
        link.apply();
    }
}

void Solver::applyConstraint()
{
    for(auto& v : m_objects) {
        v.pos_current.x = std::max( std::min( v.pos_current.x, m_GWidth  ), 0.0f );
        v.pos_current.y = std::max( std::min( v.pos_current.y, m_GHeight ), 0.0f );
    }

}

void Solver::solveCollision()
{
    for(auto obj1 = m_objects.begin(); obj1 != m_objects.end(); obj1++)
    {
        auto temp = obj1;
        for(auto obj2 = ++temp; obj2 != m_objects.end(); obj2++)
        {
            // TODO fix this mess
            if(obj1->pos_current == obj2->pos_current) continue;

            sf::Vector2f axis = obj1->pos_current - obj2->pos_current;
            float min_dist = obj1->radius + obj2->radius;

            if(axis.lengthSq() < min_dist * min_dist)
            {
                const sf::Vector2f normal = axis.normalized();
                const float mass_ratio_1 = obj1->mass / (obj1->mass + obj2->mass);
                const float mass_ratio_2 = obj2->mass / (obj1->mass + obj2->mass);
                const float delta = 0.5f * m_response_coef * (axis.length() - min_dist);
                // Update positions
                if(!obj1->fixed) obj1->pos_current -= normal * (mass_ratio_2 * delta);
                if(!obj2->fixed) obj2->pos_current += normal * (mass_ratio_1 * delta);
            }
        }
    }
}