#pragma once

#include <SFML/Graphics/Drawable.hpp>
#include <SFML/System/Time.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Sprite.hpp>

#include "Entity.hpp"
#include "VectorMath.hpp"

// TODO: tests
// TODO: documentation

namespace sfext
{
	class TexturedEntity : public Entity
	{
	protected:
		sf::Texture texture;
	public:
		// Constructors
		TexturedEntity() : Entity()
		{
		}
		TexturedEntity(const Collidable & collidable, const sf::Texture & txt, const sf::Vector2f & velocity = sf::Vector2f()) : Entity(collidable, velocity), texture(txt)
		{
		}
		TexturedEntity(const Collidable & collidable, const sf::Sprite & sprite, const sf::Vector2f & velocity = sf::Vector2f()) : Entity(collidable, velocity)
		{
			setTexture(sprite);
		}
		TexturedEntity(const Collidable & collidable, const sf::Image & image, const sf::Vector2f & velocity = sf::Vector2f()) : Entity(collidable, velocity)
		{
			setTexture(image);
		}
		// Virtual Destructor
		~TexturedEntity()
		{
		}
		// Accessors
		const sf::Texture & getTexture() const
		{
			return texture;
		}
		// Mutators
		void setTexture (const sf::Texture & txt)
		{
			texture = txt;
		}
		void setTexture (const sf::Image & image)
		{
			texture.loadFromImage(image);
		}
		void setTexture (const sf::Sprite & sprite)
		{
			if (sprite.getTexture())
			{
				texture = *(sprite.getTexture());
			}
		}
		void setRepeated(bool repeated)
		{
			texture.setRepeated(repeated);
		}
		// Utilities
		virtual void updateVertices()
		{
			m_vertices[0] = sf::Vertex(m_collidable.getPosition(), sf::Vector2f(0.f, 0.f));
			m_vertices[1] = sf::Vertex(m_collidable.getPosition() + sf::Vector2f(m_collidable.getDimensions().x, 0.f), sf::Vector2f(m_collidable.getDimensions().x, 0.f));
			m_vertices[2] = sf::Vertex(m_collidable.getPosition() + m_collidable.getDimensions(), m_collidable.getDimensions());
			m_vertices[3] = sf::Vertex(m_collidable.getPosition() + sf::Vector2f(0.f, m_collidable.getDimensions().y), sf::Vector2f(0.f, m_collidable.getDimensions().y));
		}
		virtual void draw(sf::RenderTarget & target, sf::RenderStates states = sf::RenderStates::Default) const
		{
			states.texture = &texture;
			target.draw(m_vertices, states);
		}
	};
}