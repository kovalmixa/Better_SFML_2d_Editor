#include "smoke_emitter.h"

SmokeEmitter::SmokeEmitter(size_t particles_count) : BaseEmitter(particles_count)
{
}

void SmokeEmitter::process_emition(sf::Vector2f position)
{
    auto indices = ParticlePool::get_instance().acquire(particles_count_);
    for (int idx : indices) {
        auto* p = ParticlePool::get_instance().get(idx);
        if (!p) return;

        p->transform_data.position = position;
        p->velocity = { random<float>(-10,10), random<float>(-50,-20) };

        p->lifetime = p->maxLifetime = 3.0f;
        float size = random<float>(5, 10);
        p->transform_data.size = { size, size };
        p->color = sf::Color(150, 150, 150, 200);
        p->active = true;
        active_indices_.push_back(idx);
    }
}