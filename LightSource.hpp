#pragma once

#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/VertexArray.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/System/Vector2.hpp>

namespace sfext
{
	class LightSource : public sf::Drawable
	{
	protected:
		mutable sf::VertexArray vertices;
		sf::Vector2f            position;
	public:
		// Constructors
		LightSource(sf::PrimitiveType primitive, std::size_t vertexCount, const sf::Vector2f & pos) : vertices(primitive, vertexCount), position(pos)
		{
		}
		// Virtual Destructor
		virtual ~LightSource()
		{
		}
		// Accessors
		sf::Vector2f      getPosition     () const
		{
			return position;
		}
		sf::VertexArray   getVertices     () const
		{
			return vertices;
		}
		sf::PrimitiveType getPrimitiveType() const
		{
			return vertices.getPrimitiveType();
		}
		std::size_t       getVertexCount  () const
		{
			return vertices.getVertexCount();
		}
		// Mutators
		void setPosition(const sf::Vector2f & pos)
		{
			position = pos;
		}
		void setPosition(float x, float y)
		{
			position.x = x;
			position.y = y;
		}
		// Utilities
		virtual void updateVertices() const = 0;
		virtual void updateVertices(const sf::View & view) const
		{
			updateVertices();
			sf::Vector2f offset = view.getCenter() - view.getSize() * .5f;
			for (unsigned int i = 0; i < vertices.getVertexCount(); ++i)
			{
				vertices[i].position -= offset;
				vertices[i].texCoords -= offset;
			}
		}
		virtual void draw          (sf::RenderTarget & target, sf::RenderStates states = sf::RenderStates::Default) const
		{
			updateVertices();
			target.draw(vertices, states);
		}
		virtual void draw          (sf::RenderTarget & target, const sf::View & view, sf::RenderStates states = sf::RenderStates::Default) const
		{
			updateVertices(view);
			target.draw(vertices, states);
		}
	};
}