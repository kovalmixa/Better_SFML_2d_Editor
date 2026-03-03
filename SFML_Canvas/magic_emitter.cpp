#include "magic_emitter.h"

MagicEmitter::MagicEmitter(size_t particles_count) : BaseEmitter(particles_count)
{
}

void MagicEmitter::process_emition(sf::Vector2f position)
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
            spawn_count++;
        }
    }
}