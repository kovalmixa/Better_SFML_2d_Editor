#include "explosion_emitter.h"

ExplosionEmitter::ExplosionEmitter(size_t particles_count) : BaseEmitter(particles_count)
{
}

void ExplosionEmitter::process_emition(sf::Vector2f position)
{
    auto indices = get_particles_indexes();
    int spawn_count = 0;
    int max_spawn_per_frame = 5;
    for (size_t i = 0; i < indices.size(); i++)
    {
        int idx = indices[i];
        auto* p = ParticlePool::get_instance().get(idx);
        if (!p) continue;

        if (!p->active && spawn_count < max_spawn_per_frame)
        {
            float angle = random<float>(0.f, 360.f);
            float speed = random<float>(100.f, 300.f);
            p->transform_data.position = position;
            float rad = deg_to_rad(angle);
            p->velocity = { std::cos(rad) * speed, std::sin(rad) * speed };
            p->lifetime = p->maxLifetime = 1.0f;
            p->transform_data.size = { random<float>(2.f, 5.f), random<float>(2.f, 5.f) };
            p->color = sf::Color(255, random<int>(2, 5), 0);

            p->active = true;
            spawn_count++;
        }
    }
}