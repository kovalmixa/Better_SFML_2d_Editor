#pragma once
#include <vector>
#include <thread>
#include <atomic>
#include <SFML/Graphics.hpp>
#include <map>

#include "particle_data.h"

class BaseShape;
class BaseEmitter;

enum class EmitterMode
{
    None,
    Explosion,
    Smoke,
    Fire,
    Magic
};

class ParticleSystem {
private:
    BaseShape* host_shape_ = nullptr;
    std::map<EmitterMode, BaseEmitter*> emition_map;
    sf::RectangleShape particle_shape_;
    BaseEmitter* current_emitter = nullptr;
public:
    std::thread update_thread;

    ParticleSystem(BaseShape* host_shape);
    ~ParticleSystem();

    void emit(const EmitterMode mode);
    void stop_emition();
    void update();
    void draw(sf::RenderWindow& window);
};
