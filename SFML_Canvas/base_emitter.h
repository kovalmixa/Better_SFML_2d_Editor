#ifndef BASE_EMITTER_H
#define BASE_EMITTER_H
#pragma once

#include <SFML/Graphics.hpp>
#include <vector>
#include <functional>

#include "particle_data.h"
#include "basic_functions.h"
#include "particle_pool.h"

class BaseEmitter {
protected:
	std::vector<int> active_indices_;
	size_t particles_count_;

	virtual std::vector<int> get_particles_indexes();
	virtual void  swap_pop_indice(int idx, int i);
public:
	BaseEmitter(size_t particles_count);
	~BaseEmitter();

	void for_each_active(std::function<void(ParticleData&)> func);
	virtual void process_emition(sf::Vector2f position) = 0;
	virtual void update_particles(float dt);
	virtual void stop();

	virtual void draw(sf::RenderWindow& window, sf::RectangleShape* particle_shape);
};
#endif