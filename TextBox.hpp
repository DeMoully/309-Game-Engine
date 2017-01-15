#pragma once

#include "GuiBase.hpp"

namespace sfext
{
	class TextBox : public TextComponent, public GuiBase
	{
	protected:
		sf::Color m_backgroundColor;
		sf::Color m_outlineColor;
		float     m_outlineThickness;
	public:
		// Virtual Destructor
		virtual ~TextBox()
		{
		}
		// Accessors
		sf::Color getBackgroundColor() const
		{
			return m_backgroundColor;
		}
		// Mutators
		void setBackgroundColor(const sf::Color & color)
		{
			m_backgroundColor = color;
		}
		void setOutlineColor(const sf::Color & color)
		{
			m_outlineColor = color;
		}
		void setOutlineThickness(float thickness)
		{
			m_outlineThickness = thickness;
		}
		// Utilities
		virtual void draw(sf::RenderTarget & target, sf::RenderStates states = sf::RenderStates::Default) const
		{
			sf::VertexArray backdrop(sf::PrimitiveType::Quads, 4);
			sf::VertexArray outline(sf::PrimitiveType::Quads, 4);
			backdrop[0] = sf::Vertex(m_position, m_backgroundColor);
			backdrop[1] = sf::Vertex(sf::Vector2f(m_position.x + m_dimensions.x, m_position.y), m_backgroundColor);
			backdrop[2] = sf::Vertex(m_position + m_dimensions, m_backgroundColor);
			backdrop[3] = sf::Vertex(sf::Vector2f(m_position.x, m_position.y + m_dimensions.y), m_backgroundColor);

			outline[0] = sf::Vertex(m_position - sf::Vector2f(m_outlineThickness, m_outlineThickness), m_outlineColor);
			outline[1] = sf::Vertex(m_position + sf::Vector2f(m_dimensions.x + m_outlineThickness, -m_outlineThickness), m_outlineColor);
			outline[2] = sf::Vertex(m_position + m_dimensions + sf::Vector2f(m_outlineThickness, m_outlineThickness), m_outlineColor);
			outline[3] = sf::Vertex(m_position + sf::Vector2f(- m_outlineThickness, m_dimensions.y + m_outlineThickness), m_outlineColor);

			target.draw(outline, states);
			target.draw(backdrop, states);

			if (m_font != nullptr)
			{
				states.texture = &m_font->getTexture(m_characterSize);
				target.draw(m_textVertices, states);
			}
		}
	};
}