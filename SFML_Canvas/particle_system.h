#pragma once

#include <vector>
#include <SFML/Graphics.hpp>

#include "particle_data.h"

enum class ParticleMode
{
    Explosion,
    Smoke,
    Fire,
    Magic
};

class ParticleSystem {
private:
    std::vector<ParticleData> particles_;
    sf::RectangleShape particle_shape;

    ParticleData* get_free_particle();

    void emit_explosion(sf::Vector2f pos);
    void emit_fire(sf::Vector2f pos);
    void emit_smoke(sf::Vector2f pos);
    void emit_magic(sf::Vector2f pos);
public:
    ParticleSystem(size_t maxCount);

    void emit(sf::Vector2f position, int count);
    void update(float dt);
    void draw(sf::RenderWindow& window);
};
