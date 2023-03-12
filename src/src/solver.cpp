#include "solver.hpp"
#include <iostream>

VerletObject* Solver::addObject(sf::Vector2f position, float radius, float mass, bool fixed)
{
    auto& ref = m_objects.emplace_back(VerletObject(position, radius, mass, fixed));
    return std::addressof(ref);
}

void Solver::addLink(VerletObject *v1, VerletObject *v2, float length)
{
    m_links.push_back(Link(v1, v2, length));
}

void Solver::setConstraint(sf::Vector2f position, float radius)
{
    m_constraint_center = position;
    m_constraint_radius = radius;
}

void Solver::setConstraint(sf::Vector2f size, sf::Vector2f position)
{
    m_constrain.setSize(size);
    m_constrain.setPosition(position);
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

void Solver::addRectangle(float w, float h, float x, float y, float radius, float mass, bool fixed)
{
    float v_mass = mass / 4;
    auto v1 = addObject(sf::Vector2f(x,     y    ), radius, v_mass, fixed);
    auto v2 = addObject(sf::Vector2f(x + w, y    ), radius, v_mass, fixed);
    auto v3 = addObject(sf::Vector2f(x,     y + h), radius, v_mass, fixed);
    auto v4 = addObject(sf::Vector2f(x + w, y + h), radius, v_mass, fixed);

    addLink(v1, v2, v1->distance(v2));
    addLink(v2, v3, v2->distance(v3));
    addLink(v3, v4, v3->distance(v4));
    addLink(v4, v1, v4->distance(v1));
    addLink(v1, v3, v1->distance(v3));
    addLink(v2, v4, v2->distance(v4));
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
    for (auto& obj : m_objects) {
        const sf::Vector2f v    = m_constraint_center - obj.pos_current;
        const float        dist = v.length();
        if (dist > (m_constraint_radius - obj.radius)) {
            const sf::Vector2f n = v / dist;
            obj.pos_current = m_constraint_center - n * (m_constraint_radius - obj.radius);
        }
    }
}

void Solver::solveCollision()
{
    for(auto obj1 = m_objects.begin(); obj1 != m_objects.end(); obj1++)
    {
        auto temp = obj1;
        for(auto obj2 = ++temp; obj2 != m_objects.end(); obj2++)
        {
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