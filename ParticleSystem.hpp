#pragma once

#include "Particle.hpp"

#include <SFML/Graphics/Drawable.hpp>
#include <vector>

// TODO: tests

namespace sfext
{
	template <typename ParticleType, typename Factory>
	class ParticleSystem final : public sf::Drawable
	{
	private:
		std::vector<ParticleType *> m_particles;
		Factory m_factory;
	public:
		// Constructors
		ParticleSystem(const Factory & factory) : m_factory(factory)
		{
		}
		// Destructor
		~ParticleSystem()
		{
			// Need to clear the memory for each particle
			// We know that each particle was allocated with new (because we're the ones that [indirectly] allocate them)
			for (Particle * particle : m_particles)
				if (particle != nullptr)
					delete particle;
			// Vector destructor will handle the rest
		}
		// Utilities
		void move(sf::Time elapsed)
		{
			// Update the position of each particle in the particle system
			for (Particle * particle : m_particles)
				if (particle != nullptr)
					particle->move(elapsed);
		}
		void age(sf::Time elapsed)
		{
			// Update the age of each particle in the particle system
			for (Particle * particle : m_particles)
				if (particle != nullptr)
					particle->age(elapsed);
		}
		void accelerate(const sf::Vector2f & direction, sf::Time elapsed)
		{
			// Update the velocity of each particle in the particle system
			// Updates based on the current velocity and a force vector
			for (Particle * particle : m_particles)
				if (particle != nullptr)
					particle->accelerate(direction, elapsed);
		}
		void deleteDeadParticles()
		{
			// Remove particles that have somehow been deallocated prematurely
			// or have ages that are greater than the particle's lifespan
			unsigned int index = 0;
			unsigned int size = m_particles.size();
			while (index < size)
			{
				Particle * temp = m_particles.at(index);
				if (temp != nullptr)
				{
					if (temp->isDead())
					{
						delete temp;
						m_particles.erase(m_particles.begin() + index);
						--size;
					}
					else
					{
						++index;
					}
				}
				else
				{
					m_particles.erase(m_particles.begin() + index);
					--size;
				}
			}
		}
		void draw(sf::RenderTarget & target, sf::RenderStates states = sf::RenderStates::Default) const
		{
			// Draw the particles to the desired RenderTarget
			// Does not use the particle's draw method
			// Instead uses the addToBatch method to draw all of the particles with a single call to draw
			sf::VertexArray vertices(ParticleType::getPrimitiveType(), 0U);
			for (ParticleType * particle : m_particles)
				if (particle != nullptr)
					particle->addToBatch(vertices);
			target.draw(vertices);
		}
		// Factory functions
		void add(unsigned int n)
		{
			// Add n new particles to the particle system
			for (unsigned int i = 0; i < n; ++i)
				m_particles.push_back(m_factory.create());
		}
	};
}