#include "explosion_emitter.h"

ExplosionEmitter::ExplosionEmitter(size_t particles_count) : BaseEmitter(particles_count)
{
}

void ExplosionEmitter::process_emition(sf::Vector2f position)
{
    auto indices = ParticlePool::get_instance().acquire(particles_count_);
    for (int idx : indices) {
        auto* p = ParticlePool::get_instance().get(idx);
        if (!p) continue;
        float angle = random<float>(0.f, 360.f);
        float speed = random<float>(100.f, 300.f);
        p->transform_data.position = position;
        float rad = deg_to_rad(angle);
        p->velocity = { std::cos(rad) * speed, std::sin(rad) * speed };
        p->lifetime = p->maxLifetime = 1.0f;
        p->transform_data.size = { random<float>(2.f, 5.f), random<float>(2.f, 5.f) };
        p->color = sf::Color(255, random<int>(2, 5), 0);
        p->active = true;
        active_indices_.push_back(idx);
    }
}