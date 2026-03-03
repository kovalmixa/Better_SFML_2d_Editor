#pragma once

#include "base_emitter.h"

class FireEmitter : public BaseEmitter {
public:
	FireEmitter(size_t particles_count);
	~FireEmitter() = default;

	void process_emition(sf::Vector2f position) override;
};