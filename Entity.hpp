#pragma once

#include <SFML/Graphics/Drawable.hpp>
#include <SFML/System/Time.hpp>
#include <SFML/Graphics/VertexArray.hpp>
#include <SFML/Graphics/RenderTarget.hpp>

#include "Collidable.hpp"
#include "VectorMath.hpp"

// TODO: tests
// TODO: documentation

namespace sfext
{
	class Entity : public sf::Drawable
	{
	protected:
		Collidable m_collidable = Collidable();
		sf::Vector2f m_velocity = sf::Vector2f();
		sf::VertexArray m_vertices = sf::VertexArray(sf::PrimitiveType::Quads, 4U);
	public:
		// Constructors
		Entity         ()
		{
			updateVertices();
		}
		explicit Entity(const Collidable & collidable, const sf::Vector2f & velocity = sf::Vector2f()) : m_collidable(collidable), m_velocity(velocity)
		{
			updateVertices();
		}
		Entity         (const sf::Vector2f & position, const sf::Vector2f & dimensions, const sf::Vector2f & velocity = sf::Vector2f()) : m_collidable(position, dimensions), m_velocity(velocity)
		{
			updateVertices();
		}
		Entity         (float x, float y, float width, float height, sf::Vector2f & velocity = sf::Vector2f()) : m_collidable(x, y, width, height), m_velocity(velocity)
		{
			updateVertices();
		}
		// Virtual Destructor
		virtual ~Entity()
		{
		}
		// Accessors
		Collidable    getCollidable () const
		{
			return m_collidable;
		}
		sf::Vector2f  getVelocity   () const
		{
			return m_velocity;
		}
		sf::Vector2f  getPosition   () const
		{
			return m_collidable.getPosition();
		}
		sf::Vector2f  getDimensions () const
		{
			return m_collidable.getDimensions();
		}
		sf::Vector2f  getCenter     () const
		{
			return m_collidable.getCenter();
		}
		sf::FloatRect getBoundingBox() const
		{
			return m_collidable.getBoundingBox();
		}
		// Mutators
		void setVelocity  (const sf::Vector2f & vel)
		{
			m_velocity = vel;
		}
		void setVelocity  (float x, float y)
		{
			m_velocity.x = x;
			m_velocity.y = y;
		}
		void setPosition  (const sf::Vector2f & position)
		{
			m_collidable.setPosition(position);
			updateVertices();
		}
		void setPosition  (float x, float y)
		{
			m_collidable.setPosition(x, y);
			updateVertices();
		}
		void setDimensions(const sf::Vector2f & dimensions)
		{
			m_collidable.setDimensions(dimensions);
			updateVertices();
		}
		void setDimensions(float x, float y)
		{
			m_collidable.setDimensions(x, y);
			updateVertices();
		}
		void scale        (float factor)
		{
			m_collidable.scale(factor);
			updateVertices();
		}
		void scale        (float x, float y)
		{
			m_collidable.scale(x, y);
			updateVertices();
		}
		void scale        (const sf::Vector2f & factor)
		{
			m_collidable.scale(factor);
			updateVertices();
		}
		// Utilities
		virtual bool intersects        (const Entity & other) const
		{
			return m_collidable.intersects(other.getCollidable());
		}
		virtual void handleIntersection(const Entity & other)
		{
			// TODO: rewrite this with SAT

			// Assumes a static Entity for 'other'. This should be used for intersections with terrain, boundaries, etc.
			// This intersection method is not to be used with two moving entities, as the results are undefined and will often not be what is expected. Use with caution.
			Collidable otherCollidable = other.getCollidable();
			sf::Vector2f center = m_collidable.getCenter();
			if (intersects(other))
			{
				if (m_velocity.x == 0.f)
				{
					if (m_velocity.y < 0.f) // Object is moving straight up
					{
						m_collidable.move(0.f, otherCollidable.getPosition().y + otherCollidable.getDimensions().y - getPosition().y);
					}
					else if (m_velocity.y > 0.f) // Object is moving straight down
					{
						m_collidable.move(0.f, otherCollidable.getPosition().y - getPosition().y - getDimensions().y);
					}
					m_velocity.y = 0.f;
					return;
				}
				else if (m_velocity.y == 0.f)
				{
					if (m_velocity.x < 0.f) // Object is moving straight to the left
					{
						m_collidable.move(otherCollidable.getPosition().x + otherCollidable.getDimensions().x - getPosition().x, 0.f);
					}
					else if (m_velocity.x > 0.f) // Object is moving straight to the right
					{
						m_collidable.move(otherCollidable.getPosition().x - getPosition().x - getDimensions().x, 0.f);
					}
					m_velocity.x = 0.f;
					return;
				}
				sf::Vector2f offset(0.f, 0.f);
				sf::Vector2f ratios(0.f, 0.f);
				if (m_velocity.x < 0.f && m_velocity.y < 0.f)
				{
					offset = getPosition() - otherCollidable.getPosition() - otherCollidable.getDimensions();
					ratios = sf::Vector2f(offset.x / m_velocity.x, offset.y / m_velocity.y);
				}
				else if (m_velocity.x < 0.f && m_velocity.y > 0.f)
				{
					offset.x = getPosition().x - otherCollidable.getPosition().x - otherCollidable.getDimensions().x;
					offset.y = getPosition().y + getDimensions().y - otherCollidable.getPosition().y;
					ratios = sf::Vector2f(offset.x / m_velocity.x, offset.y / m_velocity.y);
				}
				else if (m_velocity.x > 0.f && m_velocity.y < 0.f)
				{
					offset.x = getPosition().x + getDimensions().x - otherCollidable.getPosition().x;
					offset.y = getPosition().y - otherCollidable.getPosition().y - otherCollidable.getDimensions().y;
					ratios = sf::Vector2f(offset.x / m_velocity.x, offset.y / m_velocity.y);
				}
				else if (m_velocity.x > 0.f && m_velocity.y > 0.f)
				{
					offset = getPosition() + getDimensions() - otherCollidable.getPosition();
					ratios = sf::Vector2f(offset.x / m_velocity.x, offset.y / m_velocity.y);
				}
				if (ratios.y < ratios.x)
				{
					m_collidable.move(0.f, -ratios.y * m_velocity.y);
					m_velocity.y = 0.f;
				}
				else if (ratios.x < ratios.y)
				{
					m_collidable.move(-ratios.x * m_velocity.x, 0.f);
					m_velocity.x = 0.f;
				}
				else
				{
					m_collidable.move(-ratios.x * m_velocity.x, -ratios.y * m_velocity.y);
					m_velocity = sf::Vector2f(0.f, 0.f);
				}
			}
			updateVertices();
		}
		virtual bool containsPoint     (const sf::Vector2f & point) const
		{
			return m_collidable.containsPoint(point);
		}
		virtual bool containsPoint     (float x, float y) const
		{
			return containsPoint(sf::Vector2f(x, y));
		}
		virtual void accelerate        (sf::Time elapsed, const sf::Vector2f & force)
		{
			m_velocity += force * elapsed.asSeconds();
		}
		virtual void move              (sf::Time elapsed)
		{
			m_collidable.move(m_velocity * elapsed.asSeconds());
			updateVertices();
		}
		virtual void update            (sf::Time elapsed)
		{
			move(elapsed);
		}
		virtual void updateVertices()
		{
			sf::Vector2f position = getPosition();
			sf::Vector2f dimensions = getDimensions();
			m_vertices[0] = sf::Vertex(position, sf::Color::White);
			m_vertices[1] = sf::Vertex(position + sf::Vector2f(dimensions.x, 0.f), sf::Color::White);
			m_vertices[2] = sf::Vertex(position + dimensions, sf::Color::White);
			m_vertices[3] = sf::Vertex(position + sf::Vector2f(0.f, dimensions.y), sf::Color::White);
		}
		virtual void draw              (sf::RenderTarget & target, sf::RenderStates states = sf::RenderStates::Default) const
		{
			target.draw(m_vertices, states);
		}
	};

	bool operator < (const Entity & left, const Entity & right)
	{
		// Order the entities based on their y values and then on their x values
		if (left.getPosition().y < right.getPosition().y)
		{
			return true;
		}
		else if (left.getPosition().y > right.getPosition().y)
		{
			return false;
		}
		else // Y values are equal, check x values
		{
			return left.getPosition().x < right.getPosition().x;
		}
	}
}