#include "base_emitter.h"

std::vector<int> BaseEmitter::get_particles_indexes()
{
    size_t missing = particles_count_ - active_indices_.size();
    if (missing > 0) {
        auto new_indices = ParticlePool::get_instance().acquire(missing);
        active_indices_.insert(active_indices_.end(), new_indices.begin(), new_indices.end());
    }
    return active_indices_;
}

void BaseEmitter::swap_pop_indice(int idx, int i)
{
    ParticlePool::get_instance().release(idx);
    active_indices_[i] = active_indices_.back();
    active_indices_.pop_back();
}

BaseEmitter::BaseEmitter(size_t particles_count)
{
	particles_count_ = particles_count;
}

BaseEmitter::~BaseEmitter()
{
	stop();
}

void BaseEmitter::for_each_active(std::function<void(ParticleData&)> func) {
    auto& pool = ParticlePool::get_instance();
    for (int idx : active_indices_) {
        ParticleData* p = pool.get(idx);
        if (p) func(*p);
    }
}

void BaseEmitter::update_particles(float dt)
{
    auto& pool = ParticlePool::get_instance();
    for (size_t i = 0; i < active_indices_.size(); ) {
        int idx = active_indices_[i];
        ParticleData* p = pool.get(idx);
        if (!p) { i++; continue; }

        p->update(dt);

        if (!p->active) swap_pop_indice(idx, i);
        else i++;
    }
}

void BaseEmitter::stop()
{
	for (auto idx : active_indices_)
		ParticlePool::get_instance().release(idx);
}

void BaseEmitter::draw(sf::RenderWindow& window, sf::RectangleShape* particle_shape) {
    auto& pool = ParticlePool::get_instance();
    for (auto idx : active_indices_)
    {
        auto* p_ptr = pool.get(idx);
        if (!p_ptr || !p_ptr->active) continue;

        auto& particle = *p_ptr;
        auto& p_transform = particle.transform_data;

        particle_shape->setSize(p_transform.size);
        particle_shape->setPosition(p_transform.position);
        particle_shape->setRotation(p_transform.rotation);
        particle_shape->setFillColor(particle.color);

        window.draw(*particle_shape);
    }
}