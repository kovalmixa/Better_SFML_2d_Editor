#pragma once

#include "base_emitter.h"

class SmokeEmitter : public BaseEmitter {
public:
	SmokeEmitter(size_t particles_count);
	~SmokeEmitter() = default;

	void process_emition(sf::Vector2f position) override;
};
