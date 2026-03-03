#include "basic_functions.h"
#include "base_shape.h"
#include "particle_system.h"
#include "fire_emitter.h"
#include "explosion_emitter.h"
#include "smoke_emitter.h"
#include "magic_emitter.h"

std::atomic<bool> is_emittion_active{ true };

ParticleSystem::ParticleSystem(BaseShape* host_shape)
{
    host_shape_ = host_shape;

    emition_map = {
        { EmitterMode::Explosion, new ExplosionEmitter(100) },
        { EmitterMode::Fire, new FireEmitter(20) },
        { EmitterMode::Smoke, new SmokeEmitter(10) },
        { EmitterMode::Magic, new MagicEmitter(50) },
    };
}

ParticleSystem::~ParticleSystem()
{
    is_emittion_active = false;
    if (update_thread.joinable()) {
        update_thread.join();
    }
}

void ParticleSystem::emit(const EmitterMode mode)
{
    stop_emition();
    if (mode == EmitterMode::None) return;
    is_emittion_active = true;
    current_emitter = emition_map[mode];
    if (current_emitter == nullptr) return;
    update_thread = std::thread(&ParticleSystem::update, this);
}

void ParticleSystem::stop_emition()
{
    if (update_thread.joinable()) {
        current_emitter->stop();
        is_emittion_active = false;
        update_thread.join();
    }
}

void ParticleSystem::update()
{
    sf::Clock clock;
    while (is_emittion_active) {
        float dt = clock.restart().asSeconds();
        auto transform = host_shape_->get_transform();
        auto position = transform.position;
        current_emitter->process_emition(position);
        current_emitter->for_each_active([dt](ParticleData& p) {
            p.update(dt);
        });
        std::this_thread::sleep_for(std::chrono::milliseconds(15));
    }
}

void ParticleSystem::draw(sf::RenderWindow& window)
{
    if (current_emitter == nullptr) return;

    current_emitter->for_each_active([&window, this](ParticleData& particle) {
        if (!particle.active) return;

        auto& p_transform = particle.transform_data;
        particle_shape_.setSize(p_transform.size);
        particle_shape_.setPosition(p_transform.position);
        particle_shape_.setRotation(p_transform.rotation);
        particle_shape_.setFillColor(particle.color);

        window.draw(particle_shape_);
    });
}