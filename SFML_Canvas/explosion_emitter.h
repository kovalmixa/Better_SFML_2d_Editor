#pragma once

#include "base_emitter.h"

class ExplosionEmitter : public BaseEmitter {
public:
	ExplosionEmitter(size_t particles_count);
	~ExplosionEmitter() = default;

	void process_emition(sf::Vector2f position) override;
};