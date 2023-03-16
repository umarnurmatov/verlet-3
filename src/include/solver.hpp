#pragma once
#include <list>

#include "SFML/System/Vector2.hpp"
#include "SFML/Graphics/RectangleShape.hpp"

#include "verletObject.hpp"
#include "link.hpp"

typedef unsigned int uint;

class Solver
{
    sf::Vector2f                m_gravity;
    uint                        m_sub_steps;   
    float                       m_response_coef;  
    float                       m_GHeight, m_GWidth;
    sf::RectangleShape          m_constrain;
    std::vector<VerletObject>   m_objects;
    std::vector<Link>           m_links;

public:

    Solver()
        : m_gravity            {0.0f, 1000.0f},
          m_sub_steps          {8},
          m_response_coef      {0.75f}
    {
        m_objects.reserve(1000);
    }

    VerletObject* addObject(sf::Vector2f position, float radius, float mass, bool fixed = false);
    VerletObject* addObject(VerletObject v);

    void addLink(VerletObject *v1, VerletObject *v2, float length);

    void setConstraint(float w, float h) { m_GWidth = w; m_GHeight = h; }
    void setSubstepsCount(uint substeps) { m_sub_steps = substeps; }

    void update(float dt);

    [[nodiscard]] const std::vector<VerletObject>& getObjects() const;

    /// @brief 
    /// @param w_count amount of verlet object on x-axis
    /// @param h_count amount of verlet object on y-axis
    /// @param r       radius of each verlet object
    void addRectangle(uint w_count, uint h_count, float r, float x, float y, float mass, bool fixed);


private:
    void applyGravity();

    void updateObjects(float dt);

    void applyLinks();

    void applyConstraint();

    void solveCollision();

};