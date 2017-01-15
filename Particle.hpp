#pragma once

#include <SFML/Graphics/Drawable.hpp>
#include <SFML/System/Time.hpp>

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
		}
		// Destructor
		virtual ~Particle()
		{
			// There isn't really anything to do
		}
		// Accessors
		sf::Vector2f getPosition() const
		{
			return m_position;
		}
		sf::Vector2f getVelocity() const
		{
			return m_velocity;
		}
		sf::Time getAge() const
		{
			return m_age;
		}
		sf::Time getLifespan() const
		{
			return m_lifespan;
		}
		// Mutators
		virtual void setPosition(const sf::Vector2f & position)
		{
			m_position = position;
		}
		virtual void setVelocity(const sf::Vector2f & velocity)
		{
			m_velocity = velocity;
		}
		virtual void setAge(sf::Time age)
		{
			m_age = age;
		}
		virtual void setLifespan(sf::Time lifespan)
		{
			m_lifespan = lifespan;
		}
		// Utilities
		virtual void move(sf::Time elapsed)
		{
			m_position += m_velocity * elapsed.asSeconds();
		}
		virtual void accelerate(sf::Vector2f direction, sf::Time elapsed)
		{
			m_velocity += direction * elapsed.asSeconds();
		}
		virtual void age(sf::Time elapsed)
		{
			m_age += elapsed;
		}
		virtual bool isDead() const
		{
			return m_age > m_lifespan;
		}
		virtual void draw(sf::RenderTarget & target, sf::RenderStates states = sf::RenderStates::Default)
		{
			sf::VertexArray vertices(m_primitiveType, 1U);
			addToBatch(vertices);
			target.draw(vertices, states);
		}
		// Batching stuff
		virtual void addToBatch(sf::VertexArray & vertices) const
		{
			// Overwrite this with the rendering code for Particle subclasses
			vertices.append(sf::Vertex(m_position, sf::Color::White));
		}
		// Static Functions
		static sf::PrimitiveType getPrimitiveType()
		{
			return m_primitiveType;
		}
		static unsigned int getPointCount()
		{
			return m_pointCount;
		}
	};

	sf::PrimitiveType Particle::m_primitiveType = sf::PrimitiveType::Points;
	unsigned int Particle::m_pointCount = 1U;
}