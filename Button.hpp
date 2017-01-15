#pragma once

#include "GuiBase.hpp"

namespace sfext
{
	class Button : public GuiBase, public ClickableComponent, public DimensionalComponent, public ActionComponent
	{
	protected:
	public:
		// Constructors
		Button(const sf::Vector2f & position, const sf::Vector2f & dimensions, sf::Mouse::Button button) : ClickableComponent(button), DimensionalComponent(position, dimensions)
		{

		}
		// Virtual Destructor
		virtual ~Button()
		{
		}
		// Utilities
		virtual void handleClick(const sf::Vector2f & mousePosition)
		{
			if (clicked(mousePosition))
			{
				call();
			}
		}
		virtual void draw(sf::RenderTarget & target, sf::RenderStates states = sf::RenderStates::Default) const
		{
			static sf::VertexArray vertices = sf::VertexArray(sf::PrimitiveType::Quads, 4U);
			vertices[0].position = m_position;
			vertices[1].position = sf::Vector2f(m_position.x + m_dimensions.x, m_position.y);
			vertices[2].position = m_position + m_dimensions;
			vertices[3].position = sf::Vector2f(m_position.x, m_position.y + m_dimensions.y);
			target.draw(vertices, states);
		}
		virtual bool mousedOver(const sf::Vector2f & mousePosition) const
		{
			return sf::FloatRect(m_position, m_dimensions).contains(mousePosition);
		}
		virtual bool clicked(const sf::Vector2f & mousePosition) const
		{
			return mousedOver(mousePosition) && sf::Mouse::isButtonPressed(m_mouseButton);
		}
	};

	class TexturedButton : public Button, public TextureComponent
	{
	protected:
	public:
		// Constructors
		TexturedButton(const sf::Vector2f & position, const sf::Vector2f & dimensions, sf::Mouse::Button button) : Button(position, dimensions, button)
		{
		}
		// Virtual Destructor
		virtual ~TexturedButton()
		{
		}
		// Utilities
		virtual void updateVertices() const
		{
			if (m_texture != nullptr)
			{
				m_vertices[0] = sf::Vertex(m_position, m_color, sf::Vector2f(0.f, 0.f));
				m_vertices[1] = sf::Vertex(sf::Vector2f(m_position.x + m_dimensions.x, m_position.y), m_color, sf::Vector2f(static_cast<float>(m_texture->getSize().x), 0.f));
				m_vertices[2] = sf::Vertex(m_position + m_dimensions, m_color, sf::Vector2f(m_texture->getSize()));
				m_vertices[3] = sf::Vertex(sf::Vector2f(m_position.x, m_position.y + m_dimensions.y), m_color, sf::Vector2f(0.f, static_cast<float>(m_texture->getSize().y)));
			}
			else
			{
				m_vertices[0] = sf::Vertex(m_position, m_color);
				m_vertices[1] = sf::Vertex(sf::Vector2f(m_position.x + m_dimensions.x, m_position.y), m_color);
				m_vertices[2] = sf::Vertex(m_position + m_dimensions, m_color);
				m_vertices[3] = sf::Vertex(sf::Vector2f(m_position.x, m_position.y + m_dimensions.y), m_color);
			}
		}
		virtual void draw(sf::RenderTarget & target, sf::RenderStates states = sf::RenderStates::Default) const
		{
			updateVertices();
			if (m_texture != nullptr)
				states.texture = m_texture.get();
			target.draw(m_vertices, states);
		}
	};
}