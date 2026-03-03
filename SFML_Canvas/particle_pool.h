#pragma once
#include <vector>
#include <mutex>
#include <optional>
#include "particle_system.h"

class ParticlePool {
private:
    ParticlePool(size_t maxCount);
    std::vector<ParticleData> pool_;
    std::vector<int> free_list_;
    std::mutex lock_;
public:
    static ParticlePool& get_instance(size_t maxCount = 10000);
    std::vector<int> acquire(size_t count);
    int acquire_one();
    void release(int index);
    ParticleData* get(int index);
    size_t capacity() const;
};