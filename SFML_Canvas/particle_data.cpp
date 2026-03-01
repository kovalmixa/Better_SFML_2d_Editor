#include "particle_data.h"

void ParticleData::update(float dt)
{
    if (!active) return;
    lifetime -= dt;
    if (lifetime <= 0)
    {
        active = false;
        return;
    }
    transform_data.position += velocity * dt;

    float lifeRatio = lifetime / maxLifetime;
    color.a = static_cast<uint8_t>(255 * lifeRatio);
}