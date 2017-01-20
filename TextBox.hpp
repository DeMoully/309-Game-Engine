#pragma once

#include "GuiBase.hpp"

// TODO: tests
// TODO: documentation

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
			// TODO: have update geometry handle the vertices so that things are not being recomputed every frame - it's very wasteful

			// if we have a font, then we have 8 more vertices than the base text component would have
			// otherwise, we just have 8 vertices
			sf::VertexArray vertices(sf::PrimitiveType::Quads, 8 + (m_font != nullptr ? m_textVertices.getVertexCount() : 0));

			vertices[0] = sf::Vertex(m_position, m_backgroundColor);
			vertices[1] = sf::Vertex(sf::Vector2f(m_position.x + m_dimensions.x, m_position.y), m_backgroundColor);
			vertices[2] = sf::Vertex(m_position + m_dimensions, m_backgroundColor);
			vertices[3] = sf::Vertex(sf::Vector2f(m_position.x, m_position.y + m_dimensions.y), m_backgroundColor);

			vertices[4] = sf::Vertex(m_position - sf::Vector2f(m_outlineThickness, m_outlineThickness), m_outlineColor);
			vertices[5] = sf::Vertex(m_position + sf::Vector2f(m_dimensions.x + m_outlineThickness, -m_outlineThickness), m_outlineColor);
			vertices[6] = sf::Vertex(m_position + m_dimensions + sf::Vector2f(m_outlineThickness, m_outlineThickness), m_outlineColor);
			vertices[7] = sf::Vertex(m_position + sf::Vector2f(-m_outlineThickness, m_dimensions.y + m_outlineThickness), m_outlineColor);
			
			if (m_font != nullptr)
			{
				for (unsigned int i = 0; i < m_textVertices.getVertexCount(); ++i)
					vertices[8 + i] = m_textVertices[i];
				states.texture = &m_font->getTexture(m_characterSize);
			}
			target.draw(vertices, states);
		}
	};
}