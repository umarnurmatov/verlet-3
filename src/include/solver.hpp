#pragma once
#include <list>

#include "SFML/System/Vector2.hpp"
#include "SFML/Graphics/RectangleShape.hpp"

#include "verletObject.hpp"
#include "link.hpp"

class Solver
{
    sf::Vector2f              m_gravity;
    uint32_t                  m_sub_steps;   
    float                     m_response_coef;  
    sf::Vector2f              m_constraint_center;
    float                     m_constraint_radius;
    sf::RectangleShape        m_constrain;
    std::list<VerletObject>   m_objects;
    std::list<Link>           m_links;

public:

    Solver()
        : m_gravity            {0.0f, 1000.0f},
          m_sub_steps          {8},
          m_response_coef      {0.75f},
          m_constraint_radius  {100.0f}
    {
    }

    VerletObject* addObject(sf::Vector2f position, float radius, float mass, bool fixed = false);

    void addLink(VerletObject *v1, VerletObject *v2, float length);

    void setConstraint(sf::Vector2f position, float radius);
    void setConstraint(sf::Vector2f size, sf::Vector2f position);

    void update(float dt);

    [[nodiscard]] const std::list<VerletObject>& getObjects() const;

    void addRectangle(float w, float h, float x, float y, float radius, float mass, bool fixed);


private:
    void applyGravity();

    void updateObjects(float dt);

    void applyLinks();

    void applyConstraint();

    void solveCollision();

};