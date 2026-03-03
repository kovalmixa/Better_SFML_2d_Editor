#include "smoke_emitter.h"

SmokeEmitter::SmokeEmitter(size_t particles_count) : BaseEmitter(particles_count)
{
}

void SmokeEmitter::process_emition(sf::Vector2f position)
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
            p->velocity = { random<float>(-10,10), random<float>(-50,-20) };

            p->lifetime = p->maxLifetime = 3.0f;
            float size = random<float>(5, 10);
            p->transform_data.size = { size, size };
            p->color = sf::Color(150, 150, 150, 200);

            p->active = true;
            spawn_count++;
        }
    }
}