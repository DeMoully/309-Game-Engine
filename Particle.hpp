#pragma once

#include <SFML/Graphics/Drawable.hpp>
#include <SFML/System/Time.hpp>

// TODO: tests

namespace sfext
{
	class Particle : public sf::Drawable
	{
	protected:
		sf::Vector2f m_position;
		sf::Vector2f m_velocity;
		sf::Time m_age;
		sf::Time m_lifespan;
		// Static Stuff
		static sf::PrimitiveType m_primitiveType;
		static unsigned int m_pointCount;
	public:
		// Constructors
		Particle(const sf::Vector2f & position, const sf::Vector2f & velocity, sf::Time lifespan) : m_position(position), m_velocity(velocity), m_age(sf::seconds(0.f)), m_lifespan(lifespan)
		{
			// Construct a particle with a position, velocity, and lifespan.
			// The initial age of the particle is 0 seconds.
		}
		// Destructor
		virtual ~Particle()
		{
			// There isn't really anything to do yet.
			// Classes that inherit from this need a virtual destructor.
		}
		// Accessors
		sf::Vector2f getPosition() const
		{
			// Returns the current position of the particle
			return m_position;
		}
		sf::Vector2f getVelocity() const
		{
			// Returns the current velocity of the particle
			return m_velocity;
		}
		sf::Time getAge() const
		{
			// Returns the current age of the particle
			return m_age;
		}
		sf::Time getLifespan() const
		{
			// Returns the maximum lifespan of the particle
			return m_lifespan;
		}
		// Mutators
		virtual void setPosition(const sf::Vector2f & position)
		{
			// Overwrite the current position of the particle
			m_position = position;
		}
		virtual void setVelocity(const sf::Vector2f & velocity)
		{
			// Overwrite the current velocity of the particle
			m_velocity = velocity;
		}
		virtual void setAge(sf::Time age)
		{
			// Overwrite the current age of the particle
			m_age = age;
		}
		virtual void setLifespan(sf::Time lifespan)
		{
			// Overwrite the maximum lifespan of the particle
			m_lifespan = lifespan;
		}
		// Utilities
		virtual void move(sf::Time elapsed)
		{
			// Update the position of the particle based on its velocity and position
			m_position += m_velocity * elapsed.asSeconds();
		}
		virtual void accelerate(sf::Vector2f direction, sf::Time elapsed)
		{
			// Update the velocity of the particle based on its current velocity and a force vector
			m_velocity += direction * elapsed.asSeconds();
		}
		virtual void age(sf::Time elapsed)
		{
			// Update the age of the particle based on its current age
			m_age += elapsed;
		}
		virtual bool isDead() const
		{
			// Returns true if the particle is dead, else returns false
			return m_age > m_lifespan;
		}
		virtual void draw(sf::RenderTarget & target, sf::RenderStates states = sf::RenderStates::Default)
		{
			// Draw the particle to the specified RenderTarget
			sf::VertexArray vertices(m_primitiveType, 1U);
			addToBatch(vertices);
			target.draw(vertices, states);
		}
		// Batching stuff
		virtual void addToBatch(sf::VertexArray & vertices) const
		{
			// Add the particle's batching vertices to an already existing vertex batch
			// Overwrite this with the rendering code for Particle subclasses
			vertices.append(sf::Vertex(m_position, sf::Color::White));
		}
		// Static Functions
		static sf::PrimitiveType getPrimitiveType()
		{
			// Returns the type of primitive that is used for the particle
			// Will generally be either Points or Quads
			return m_primitiveType;
		}
		static unsigned int getPointCount()
		{
			// Returns the number of points that a single particle requires to be drawn
			return m_pointCount;
		}
	};

	sf::PrimitiveType Particle::m_primitiveType = sf::PrimitiveType::Points;
	unsigned int Particle::m_pointCount = 1U;
}