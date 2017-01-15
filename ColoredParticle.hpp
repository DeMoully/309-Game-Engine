// NOTE: this isn't really part of this framework, it's just an example (perhaps this functionality should just be put in the Particle class?)

#pragma once

#include "Particle.hpp"
#include "Random.hpp"
#include "VectorMath.hpp"

#include <SFML\Graphics\Vertex.hpp>
#include <SFML\Graphics\VertexArray.hpp>
#include <SFML\Graphics\Drawable.hpp>
#include <SFML\Graphics\RenderTarget.hpp>

namespace sfext
{
	class ColoredParticle : public Particle
	{
	protected:
		sf::Vertex m_vertex;
	public:
		// Constructors
		ColoredParticle(const sf::Vector2f & position, const sf::Vector2f & velocity, sf::Time lifespan, const sf::Color & color) : Particle(position, velocity, lifespan), m_vertex(position, color)
		{
		}
		// Destructor
		virtual ~ColoredParticle()
		{
		}
		// Mutators
		virtual void setPosition(sf::Vector2f & position)
		{
			m_position = position;
			m_vertex.position = position;
		}
		virtual void setColor(const sf::Color & color)
		{
			m_vertex.color = color;
		}
		// Utilities
		virtual void move(sf::Time elapsed)
		{
			m_position += m_velocity * elapsed.asSeconds();
			m_vertex.position = m_position;
		}
		virtual void draw(sf::RenderTarget & target, sf::RenderStates states = sf::RenderStates::Default) const
		{
			static sf::VertexArray vertex(sf::PrimitiveType::Points, 1U);
			vertex[0] = m_vertex;
			target.draw(vertex, states);
		}
		// Batching stuff
		virtual void addToBatch(sf::VertexArray & vertices) const
		{
			vertices.append(sf::Vertex(m_vertex.position, m_vertex.color));
		}
	};


	// Need to make a base ParticleFactory or something
	class ColoredParticleFactory
	{
	private:
		sf::Color m_color;
		sf::Vector2f m_position;
		sf::Vector2f m_velocity;
		sf::Time m_lifespan;
		float m_radius;
	public:
		// Constructor
		ColoredParticleFactory(const sf::Vector2f & position, const sf::Vector2f & velocity, sf::Time lifespan, const sf::Color & color, float radius) : m_position(position), m_velocity(velocity), m_lifespan(lifespan), m_color(color), m_radius(radius)
		{
		}
		// Creator Function
		ColoredParticle * create() const
		{
			// Creates a random particle within m_radius units of m_position
			// Velocity is proportional to the distance and direction of the random position
			sf::Vector2f position = randomVector2fWithinCircle(m_position, m_radius);
			sf::Vector2f velocity = distance(sf::Vector2f(0.f, 0.f), m_velocity) * unitVector(angle(position - m_position));
			ColoredParticle * particle = new ColoredParticle(position, velocity, m_lifespan, m_color);
			return particle;
		}
	};
}