#pragma once

#include <SFML/Graphics/Drawable.hpp>
#include <SFML/System/Time.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Sprite.hpp>

#include "Entity.hpp"
#include "VectorMath.hpp"

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
		TexturedEntity(const Collidable & coll, const sf::Texture & txt, const sf::Vector2f & vel = sf::Vector2f()) : Entity(coll, vel), texture(txt)
		{
		}
		TexturedEntity(const Collidable & coll, const sf::Sprite & sprite, const sf::Vector2f & vel = sf::Vector2f()) : Entity(coll, vel)
		{
			setTexture(sprite);
		}
		TexturedEntity(const Collidable & coll, const sf::Image & image, const sf::Vector2f & vel = sf::Vector2f()) : Entity(coll, vel)
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
		virtual void draw(sf::RenderTarget & target, sf::RenderStates states = sf::RenderStates::Default) const
		{
			static sf::VertexArray vertices = sf::VertexArray(sf::Quads, 4);
			states.texture = &texture;
			vertices[0] = sf::Vertex(collidable.getPosition(), sf::Vector2f(0.f, 0.f));
			vertices[1] = sf::Vertex(collidable.getPosition() + sf::Vector2f(collidable.getDimensions().x, 0.f), sf::Vector2f(collidable.getDimensions().x, 0.f));
			vertices[2] = sf::Vertex(collidable.getPosition() + collidable.getDimensions(), collidable.getDimensions());
			vertices[3] = sf::Vertex(collidable.getPosition() + sf::Vector2f(0.f, collidable.getDimensions().y), sf::Vector2f(0.f, collidable.getDimensions().y));
			target.draw(vertices, states);
		}
	};
}