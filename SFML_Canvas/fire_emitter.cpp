#include "fire_emitter.h"

FireEmitter::FireEmitter(size_t particles_count) : BaseEmitter(particles_count)
{
}

void FireEmitter::process_emition(sf::Vector2f position)
{
    auto indices = get_particles_indexes();
    int spawn_count = 0;
    int max_spawn_per_frame = 2;
    for (size_t i = 0; i < indices.size(); i++)
    {
        int idx = indices[i];
        auto* p = ParticlePool::get_instance().get(idx);
        if (!p) continue;

        if (!p->active && spawn_count < max_spawn_per_frame)
        {
            p->transform_data.position = position;
            p->velocity = { random<float>(-30.f, 30.f), random<float>(-180.f, -100.f) };
            p->maxLifetime = random<float>(0.5f, 0.9f);
            p->lifetime = p->maxLifetime;
            float size = random<float>(4.f, 8.f);
            p->transform_data.size = { size, size };
            p->color = sf::Color(255, random<int>(50, 160), 0);

            p->active = true;
            spawn_count++;
        }
    }
}
