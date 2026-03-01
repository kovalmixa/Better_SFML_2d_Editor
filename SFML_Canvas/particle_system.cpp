#define _USE_MATH_DEFINES
#include <cmath>
#include <random>

#include "particle_system.h"

ParticleData* ParticleSystem::get_free_particle()
{
	for (auto& p : particles_)
		if (!p.active)
			return &p;
	return nullptr;
}

void ParticleSystem::emit_explosion(sf::Vector2f pos)
{
    static std::random_device rd;
    static std::mt19937 gen(rd());

    std::uniform_real_distribution<float> dist_angle(0.0f, 360.0f);
    std::uniform_real_distribution<float> dist_speed(100.0f, 300.0f);
    std::uniform_real_distribution<float> dist_size(2.0f, 5.0f);
    std::uniform_int_distribution<int> dist_color(100, 200);
    for (int i = 0; i < 100; i++)
    {
        auto* p = get_free_particle();
        if (!p) return;

        float angle = dist_angle(gen);
        float speed = dist_speed(gen);

        p->transform_data.position = pos;
        float rad = angle * M_PI / 180.0f;
        p->velocity = { std::cos(rad) * speed, std::sin(rad) * speed };

        p->lifetime = p->maxLifetime = 1.0f;
        p->transform_data.size = { dist_size(gen), dist_size(gen)};
        p->color = sf::Color(255, dist_color(gen), 0);
        p->active = true;
    }
}

void ParticleSystem::emit_fire(sf::Vector2f pos)
{
    for (int i = 0; i < 20; i++)
    {
        auto* p = get_free_particle();
        if (!p) return;

        p->transform_data.position = pos;
        p->velocity = { std::random(-20,20), random(-150,-80) };

        p->lifetime = p->maxLifetime = 0.8f;
        p->transform_data.size = random(3, 6);
        p->color = sf::Color(255, random(50, 150), 0);
        p->active = true;
    }
}

void ParticleSystem::emit_smoke(sf::Vector2f pos)
{
    for (int i = 0; i < 10; i++)
    {
        auto* p = get_free_particle();
        if (!p) return;

        p->transform_data.position = pos;
        p->velocity = { std::random(-10,10), random(-50,-20) };

        p->lifetime = p->maxLifetime = 3.0f;
        p->size = random(5, 10);
        p->color = sf::Color(150, 150, 150, 200);
        p->active = true;
    }
}

void ParticleSystem::emit_magic(sf::Vector2f pos)
{
    for (int i = 0; i < 50; i++)
    {
        auto* p = get_free_particle();
        if (!p) return;

        float angle = random(0, 360);
        float radius = random(20, 60);

        p->position = pos + sf::Vector2f(cos(rad(angle)) * radius, sin(rad(angle)) * radius);

        p->velocity = { random(-30,30), random(-30,30) };

        p->lifetime = p->maxLifetime = 2.0f;
        p->size = random(2, 4);
        p->color = sf::Color(150, 0, 255);
        p->active = true;
    }
}

ParticleSystem::ParticleSystem(size_t maxCount)
{
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> distrib(1, 100);
    particles_.resize(maxCount);
}

void ParticleSystem::emit(sf::Vector2f position, int count)
{
}

void ParticleSystem::update(float dt)
{
    for (auto& particle : particles_) particle.update(dt);
}

void ParticleSystem::draw(sf::RenderWindow& window)
{
    for (auto& particle : particles_)
    {
        if (!particle.active) continue;

        auto p_transform = particle.transform_data;
        particle_shape.setSize(p_transform.size);
        particle_shape.setPosition(p_transform.position);
        particle_shape.setRotation(p_transform.rotation);
        particle_shape.setFillColor(particle.color);

        window.draw(particle_shape);
    }
}