#pragma once

#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/System/String.hpp>
#include <SFML/Graphics/Text.hpp>

#include "Animation.hpp"

#include <memory>

namespace sfext
{
	class GuiBase : public sf::Drawable
	{
	protected:
	public:
		// Constructors
		GuiBase()
		{
		}
		// Virtual Destructor
		virtual ~GuiBase()
		{
		}
		// Utilities
		virtual void draw(sf::RenderTarget & target, sf::RenderStates states = sf::RenderStates::Default) const = 0;
		virtual void update()
		{
		}
	};

	class DimensionalComponent
	{
	protected:
		sf::Vector2f m_position;
		sf::Vector2f m_dimensions;
	public:
		// Constructors
		DimensionalComponent() : m_position(0.f, 0.f), m_dimensions(0.f, 0.f)
		{
		}
		DimensionalComponent(const sf::Vector2f & position, const sf::Vector2f & dimensions) : m_position(position), m_dimensions(dimensions)
		{
		}
		// Virtual Destructor
		virtual ~DimensionalComponent()
		{
		}
		// Accessors
		sf::Vector2f getPosition() const
		{
			return m_position;
		}
		sf::Vector2f getDimensions() const
		{
			return m_dimensions;
		}
		// Mutators
		void setPosition(const sf::Vector2f & position)
		{
			m_position = position;
		}
		void setPosition(float x, float y)
		{
			m_position.x = x;
			m_position.y = y;
		}
		void setDimensions(const sf::Vector2f & dimensions)
		{
			m_dimensions = dimensions;
		}
		void setDimensions(float x, float y)
		{
			m_dimensions.x = x;
			m_dimensions.y = y;
		}
	};

	class TextureComponent
	{
	protected:
		std::shared_ptr<sf::Texture> m_texture;
		mutable sf::VertexArray      m_vertices;
		sf::Color                    m_color;
	public:
		// Constructors
		TextureComponent() : m_texture(nullptr), m_vertices(sf::PrimitiveType::Quads, 4U), m_color(sf::Color::White)
		{
		}
		TextureComponent(const std::shared_ptr<sf::Texture> & texture) : m_texture(texture), m_vertices(sf::PrimitiveType::Quads, 4U), m_color(sf::Color::White)
		{
		}
		TextureComponent(const sf::Texture & texture) : m_texture(std::make_shared<sf::Texture>(texture)), m_vertices(sf::PrimitiveType::Quads, 4U), m_color(sf::Color::White)
		{
		}
		// Virtual Destructor
		virtual ~TextureComponent()
		{
		}
		// Accessors
		std::shared_ptr<const sf::Texture> getTexture() const
		{
			return m_texture;
		}
		sf::Color getColor() const
		{
			return m_color;
		}
		// Mutators
		void setTexture(const std::shared_ptr<sf::Texture> & texture)
		{
			m_texture = texture;
		}
		void setTexture(const sf::Texture & texture)
		{
			m_texture = std::make_shared<sf::Texture>(texture);
		}
		void setColor(const sf::Color & color)
		{
			m_color = color;
		}
		// Utilities
		virtual void updateVertices() const = 0;
	};

	class AnimationComponent
	{
	protected:
		Animation m_animation;
	public:
		// Constructors
		AnimationComponent() : m_animation()
		{
		}
		AnimationComponent(const sf::Sprite & sprite, const sf::Vector2f & startPosition, const sf::Vector2f & dimensions, const sf::Vector2f & offset, const sf::Vector2u & rowsAndColumns, float frameRate = 24.f) : m_animation(sprite, startPosition, dimensions, offset, rowsAndColumns, frameRate)
		{
		}
		// Virtual Destructor
		virtual ~AnimationComponent()
		{
		}
		// Accessors
		Animation getAnimation() const
		{
			return m_animation;
		}
		// Mutators
		void setSpriteSheet(const sf::Sprite & sprite)
		{
			m_animation.setSpriteSheet(sprite);
		}
	};

	class ActionComponent
	{
	protected:
		std::function<void ()> m_action;
		sf::Clock              m_timer;
		sf::Time               m_delay;
	public:
		// Constructors
		ActionComponent() : m_action([]() -> void {}), m_delay(sf::seconds(0.f)), m_timer()
		{
		}
		ActionComponent(const std::function<void ()> & action, sf::Time delay = sf::seconds(0.f)) : m_action(action), m_delay(delay), m_timer()
		{
		}
		// Virtual Destructor
		virtual ~ActionComponent()
		{
		}
		// Accessors
		std::function<void ()> getAction() const
		{
			return m_action;
		}
		sf::Time getDelay() const
		{
			return m_delay;
		}
		// Mutators
		void setAction(const std::function<void ()> & action)
		{
			m_action = action;
		}
		void setDelay(sf::Time delay)
		{
			m_delay = delay;
		}
		// Utilities
		virtual void call()
		{
			if (canCall())
			{
				m_action();
				m_timer.restart();
			}
		}
		virtual bool canCall() const
		{
			return m_delay <= m_timer.getElapsedTime();
		}
	};

	class ClickableComponent
	{
	protected:
		sf::Mouse::Button m_mouseButton;
	public:
		// Constructors
		ClickableComponent() : m_mouseButton(sf::Mouse::Left)
		{
		}
		ClickableComponent(sf::Mouse::Button button) : m_mouseButton(button)
		{
		}
		// Virtual Destructor
		virtual ~ClickableComponent()
		{
		}
		// Accessors
		sf::Mouse::Button getMouseButton() const
		{
			return m_mouseButton;
		}
		// Mutators
		void setMouseButton(sf::Mouse::Button button)
		{
			m_mouseButton = button;
		}
		// Utilities
		virtual bool clicked(const sf::Vector2f & mousePosition) const = 0;
		virtual bool mousedOver(const sf::Vector2f & mousePosition) const = 0;
	};

	class TextComponent : public DimensionalComponent
	{
	protected:
		sf::String                m_text;
		sf::Color                 m_textColor;
		unsigned int              m_characterSize;
		sf::VertexArray           m_textVertices;
		std::shared_ptr<sf::Font> m_font;
	public:
		// Constructors
		TextComponent(sf::String text = "", sf::Color textColor = sf::Color::White, unsigned int characterSize = 30U) : m_text(text), m_textColor(textColor), m_characterSize(characterSize), m_textVertices(sf::PrimitiveType::Quads), m_font(nullptr)
		{
			updateGeometry();
		}
		// Virtual Destructor
		virtual ~TextComponent()
		{
		}
		// Accessors
		// Mutators
		virtual void setPosition(const sf::Vector2f & position)
		{
			sf::Vector2f offset = position - m_position;
			m_position = position;
			for (unsigned int i = 0; i < m_textVertices.getVertexCount(); ++i)
			{
				m_textVertices[i].position += offset;
			}
		}
		virtual void setDimensions(const sf::Vector2f & dimensions)
		{
			m_dimensions = dimensions;
			updateGeometry();
		}
		void setString(const sf::String & string)
		{
			m_text = string;
			updateGeometry();
		}
		void setTextColor(const sf::Color & color)
		{
			m_textColor = color;
			for (unsigned int i = 0; i < m_textVertices.getVertexCount(); ++i)
			{
				m_textVertices[i].color = m_textColor;
			}
		}
		void setCharacterSize(unsigned int characterSize)
		{
			m_characterSize = characterSize;
			updateGeometry();
		}
		void setFont(const sf::Font & font)
		{
			m_font = std::make_shared<sf::Font>(font);
			updateGeometry();
		}
		// Utilities
		virtual void updateGeometry()
		{
			// ===============================================================================================================================
			// |This function is based off of the updateGeometry function of the sf::Text class contained in SFML (written by Laurent Gomila)|
			// ===============================================================================================================================
			m_textVertices.clear();
			
			if (m_font == nullptr || m_text.isEmpty())
			{
				return;
			}

			float horizontal = static_cast<float>(m_font->getGlyph(' ', m_characterSize, false).advance);
			float vertical = static_cast<float>(m_font->getLineSpacing(m_characterSize));
			float x = m_position.x;
			float y = m_position.y + static_cast<float>(m_characterSize);

			sf::Uint32 previous = 0;
			for (unsigned int i = 0; i < m_text.getSize(); ++i)
			{
				sf::Uint32 current = m_text[i];

				// Find out how many characters there are until the next space and if the next word will fit
				float totalRemainingSpace = 0.f;
				sf::Uint32 prev = previous;
				for (unsigned int k = i; k < m_text.getSize() && m_text[k] != ' ' && m_text[k] != '\t' && m_text[k] != '\n' && m_text[k] != '\v'; ++k)
				{
					sf::Uint32 curr = m_text[k];
					totalRemainingSpace += static_cast<float>(m_font->getKerning(prev, curr, m_characterSize)) + m_font->getGlyph(curr, m_characterSize, false).advance;
					prev = curr;
				}
				if (x + totalRemainingSpace > m_position.x + m_dimensions.x)
				{
					x = m_position.x;
					y += vertical;
				}

				// Check for whitespace
				if (current == ' ')
				{
					x += horizontal;
					if (x > m_position.x + m_dimensions.x)
					{
						x = m_position.x;
						y += vertical;
					}
					continue;
				}
				else if (current == '\t')
				{
					x += horizontal * 4;
					if (x > m_position.x + m_dimensions.x)
					{
						x = m_position.x;
						y += vertical;
					}
					continue;
				}
				else if (current == '\n')
				{
					y += vertical;
					x = m_position.x;
					continue;
				}
				else if (current == '\v')
				{
					y += vertical * 4;
					continue;
				}

				// Append the quad to the vertex array, then move on to the next character
				const sf::Glyph & glyph = m_font->getGlyph(current, m_characterSize, false);

				int left   = static_cast<int>(glyph.bounds.left);
				int top    = static_cast<int>(glyph.bounds.top);
				int right  = static_cast<int>(glyph.bounds.left + glyph.bounds.width);
				int bottom = static_cast<int>(glyph.bounds.top + glyph.bounds.height);

				float u1 = static_cast<float>(glyph.textureRect.left);
				float v1 = static_cast<float>(glyph.textureRect.top);
				float u2 = static_cast<float>(glyph.textureRect.left + glyph.textureRect.width);
				float v2 = static_cast<float>(glyph.textureRect.top + glyph.textureRect.height);

				m_textVertices.append(sf::Vertex(sf::Vector2f(x + left, y + top), m_textColor, sf::Vector2f(u1, v1)));
				m_textVertices.append(sf::Vertex(sf::Vector2f(x + right, y + top), m_textColor, sf::Vector2f(u2, v1)));
				m_textVertices.append(sf::Vertex(sf::Vector2f(x + right, y + bottom), m_textColor, sf::Vector2f(u2, v2)));
				m_textVertices.append(sf::Vertex(sf::Vector2f(x + left, y + bottom), m_textColor, sf::Vector2f(u1, v2)));

				x += glyph.advance;
			}
		}
	};
}