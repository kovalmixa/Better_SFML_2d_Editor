#pragma once
#include <SFML/Graphics.hpp>

#include "transform_data.h"

struct ParticleData {
    TransformData transform_data;
    sf::Vector2f velocity;
    float lifetime;
    float maxLifetime;
    sf::Color color;
    bool active = false;

    void update(float dt);
};