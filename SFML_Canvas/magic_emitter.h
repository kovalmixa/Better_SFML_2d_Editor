#pragma once

#include "base_emitter.h"

class MagicEmitter : public BaseEmitter {
public:
	MagicEmitter(size_t particles_count);
	~MagicEmitter() = default;

	void process_emition(sf::Vector2f position) override;
};