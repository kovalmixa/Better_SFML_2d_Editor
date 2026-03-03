#include "fire_emitter.h"

FireEmitter::FireEmitter(size_t particles_count) : BaseEmitter(particles_count)
{
}

void FireEmitter::process_emition(sf::Vector2f position)
{
    auto indices = ParticlePool::get_instance().acquire(particles_count_);
    for (int idx : indices) {
        auto* p = ParticlePool::get_instance().get(idx);
        if (!p) return;

        p->transform_data.position = position;
        p->velocity = { random<float>(-20,20), random<float>(-150,-80) };

        p->lifetime = p->maxLifetime = 0.8f;
        float size = random<float>(3, 6);
        p->transform_data.size = { size, size };
        p->color = sf::Color(255, random<int>(50, 150), 0);
        p->active = true;
        active_indices_.push_back(idx);
    }
}
