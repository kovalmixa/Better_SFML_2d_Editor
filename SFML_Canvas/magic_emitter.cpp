#include "magic_emitter.h"

MagicEmitter::MagicEmitter(size_t particles_count) : BaseEmitter(particles_count)
{
}

void MagicEmitter::process_emition(sf::Vector2f position)
{
    auto indices = ParticlePool::get_instance().acquire(particles_count_);
    for (int idx : indices) {
        auto* p = ParticlePool::get_instance().get(idx);
        if (!p) return;

        float angle = random<float>(0, 360);
        float radius = random<float>(20, 60);
        float rad = deg_to_rad(angle);
        p->transform_data.position = position +
            sf::Vector2f(cos(rad) * radius, sin(rad) * radius);

        p->velocity = { random<float>(-30,30), random<float>(-30,30) };

        p->lifetime = p->maxLifetime = 2.0f;
        float size = random<float>(2, 4);
        p->transform_data.size = { size, size };
        p->color = sf::Color(150, 0, 255);
        p->active = true;
        active_indices_.push_back(idx);
    }
}