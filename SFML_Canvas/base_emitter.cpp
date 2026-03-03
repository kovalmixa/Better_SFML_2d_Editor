#include "base_emitter.h"

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

void BaseEmitter::stop()
{
	for (auto idx : active_indices_)
		ParticlePool::get_instance().release(idx);
}
