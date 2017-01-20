#pragma once

#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/Rect.hpp>


// TODO: tests

// The Collidable class provides a basic interface for objects that can physically interact with each other.
// The object represented by the Collidable class is an axis-aligned bounding box, or an AABB.

// The functionalities that are provided include detecting intersections with other AABBs, movement, and resizing.

// Intersection handling is beyond the scope of this class and is instead handled in the Entity class.

namespace sfext
{
	class Collidable
	{
	private:
		sf::FloatRect m_bounds;
	public:
		// Constructors
		explicit Collidable(const sf::FloatRect & rect = sf::FloatRect()) : m_bounds(rect)
		{
		}
		Collidable         (const sf::Vector2f & position, const sf::Vector2f & size) : m_bounds(position, size)
		{
		}
		Collidable         (float x, float y, float width, float height) : m_bounds(x, y, width, height)
		{
		}
		// Virtual Destructor
		virtual ~Collidable()
		{
		}
		// Accessors
		sf::Vector2f  getPosition   () const
		{
			return sf::Vector2f(m_bounds.left, m_bounds.top);
		}
		sf::Vector2f  getDimensions () const
		{
			return sf::Vector2f(m_bounds.width, m_bounds.height);
		}
		sf::Vector2f  getCenter     () const
		{
			return getPosition() + getDimensions() * .5f;
		}
		sf::FloatRect getBoundingBox() const
		{
			return m_bounds;
		}
		// Mutators
		void setPosition  (float x, float y)
		{
			m_bounds.left = x;
			m_bounds.top = y;
		}
		void setPosition  (const sf::Vector2f & position)
		{
			m_bounds.left = position.x;
			m_bounds.top = position.y;
		}
		void setDimensions(float x, float y)
		{
			m_bounds.width = x;
			m_bounds.height = y;
		}
		void setDimensions(const sf::Vector2f & dimensions)
		{
			m_bounds.width = dimensions.x;
			m_bounds.height = dimensions.y;
		}
		void move         (float x, float y)
		{
			m_bounds.left += x;
			m_bounds.top += y;
		}
		void move         (const sf::Vector2f & distance)
		{
			m_bounds.left += distance.x;
			m_bounds.top += distance.y;
		}
		void scale        (float factor)
		{
			m_bounds.width *= factor;
			m_bounds.height *= factor;
		}
		void scale        (float x, float y)
		{
			m_bounds.width *= x;
			m_bounds.height *= y;
		}
		void scale        (const sf::Vector2f & factor)
		{
			m_bounds.width *= factor.x;
			m_bounds.height *= factor.y;
		}
		void scaleFromCenter(float factor)
		{
			sf::Vector2f center = getCenter();
			float halfWidth = m_bounds.width * .5f * factor;
			float halfHeight = m_bounds.height * .5f * factor;
			m_bounds.left = center.x - halfWidth;
			m_bounds.top = center.y - halfHeight;
			m_bounds.width = halfWidth * 2.f;
			m_bounds.height = halfHeight * 2.f;
		}
		void scaleFromCenter(float x, float y)
		{
			sf::Vector2f center = getCenter();
			float halfWidth = m_bounds.width * .5f * x;
			float halfHeight = m_bounds.height * .5f * y;
			m_bounds.left = center.x - halfWidth;
			m_bounds.top = center.y - halfHeight;
			m_bounds.width = halfWidth * 2.f;
			m_bounds.height = halfHeight * 2.f;
		}
		void scaleFromCenter(const sf::Vector2f & factor)
		{
			sf::Vector2f center = getCenter();
			float halfWidth = m_bounds.width * .5f * factor.x;
			float halfHeight = m_bounds.height * .5f * factor.y;
			m_bounds.left = center.x - halfWidth;
			m_bounds.top = center.y - halfHeight;
			m_bounds.width = halfWidth * 2.f;
			m_bounds.height = halfHeight * 2.f;
		}
		// Utilities
		bool intersects   (const Collidable & other) const
		{
			return m_bounds.intersects(other.getBoundingBox());
		}
		bool intersects   (const Collidable & other, sf::FloatRect & intersection) const
		{
			return m_bounds.intersects(other.getBoundingBox(), intersection);
		}
		bool containsPoint(float x, float y) const
		{
			return m_bounds.contains(x, y);
		}
		bool containsPoint(const sf::Vector2f & point) const
		{
			return m_bounds.contains(point);
		}
	};
}