#include "particle_pool.h"
#include <algorithm>

ParticlePool& ParticlePool::get_instance(size_t maxCount) {
    static ParticlePool pool(maxCount);
    return pool;
}

ParticlePool::ParticlePool(size_t maxCount) {
    pool_.resize(maxCount);
    free_list_.reserve(maxCount);
    for (int i = static_cast<int>(maxCount) - 1; i >= 0; --i)
        free_list_.push_back(i);
}

std::vector<int> ParticlePool::acquire(size_t count) {
    std::lock_guard<std::mutex> g(lock_);
    std::vector<int> out;
    out.reserve(count);
    while (count-- > 0 && !free_list_.empty()) {
        out.push_back(free_list_.back());
        free_list_.pop_back();
    }
    return out;
}

int ParticlePool::acquire_one() {
    std::lock_guard<std::mutex> g(lock_);
    if (free_list_.empty()) return -1;
    int idx = free_list_.back();
    free_list_.pop_back();
    return idx;
}

void ParticlePool::release(int index) {
    std::lock_guard<std::mutex> g(lock_);
    if (index < 0 || static_cast<size_t>(index) >= pool_.size()) return;
    pool_[index] = ParticleData{};
    free_list_.push_back(index);
}

ParticleData* ParticlePool::get(int index) {
    if (index < 0 || static_cast<size_t>(index) >= pool_.size()) return nullptr;
    return &pool_[index];
}

size_t ParticlePool::capacity() const { return pool_.size(); }