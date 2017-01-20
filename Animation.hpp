#pragma once

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/System/Vector2.hpp>

#include "FlexibleClock.hpp"

// The Animation class provides access to an animated sprite. Various properties can be changed to give
// the programmer exact control over the animation process.

// Animations are drawable, just like any other SFML drawable entity.

// TODO: documentation
// TODO: testing

namespace sfext
{
	class Animation final : public sf::Drawable
	{
	private:
		sf::Sprite         m_spriteSheet;
		sf::Vector2f       m_start;
		sf::Vector2f       m_dimensions;
		sf::Vector2f       m_offset;
		sf::Vector2u       m_frameDistribution;
		FlexibleClock      m_timer;
		float              m_fps;
	public:
		// Constructors
		Animation() : m_start(0.f, 0.f), m_dimensions(0.f, 0.f), m_offset(0.f, 0.f), m_frameDistribution(1, 1), m_fps(24.f)
		{
		}
		Animation(const sf::Sprite & sheet, const sf::Vector2f & start, const sf::Vector2f & dimensions, const sf::Vector2f & offset, const sf::Vector2u & frameDistribution, float fps = 24.f) : m_spriteSheet(sheet), m_start(start), m_dimensions(dimensions), m_offset(offset), m_frameDistribution(frameDistribution), m_fps(fps)
		{
		}
		Animation(const Animation & rhs) : m_spriteSheet(rhs.m_spriteSheet), m_start(rhs.m_start), m_dimensions(rhs.m_dimensions), m_offset(rhs.m_offset), m_frameDistribution(rhs.m_frameDistribution), m_timer(rhs.m_timer), m_fps(rhs.m_fps)
		{
		}
		// Destructor
		~Animation()
		{
		}
		// Accessors
		sf::Vector2f        getPosition      () const
		{
			return m_spriteSheet.getPosition();
		}
		const sf::Sprite &  getSpriteSheet   () const
		{
			return m_spriteSheet;
		}
		const sf::Texture * getTexture       () const
		{
			return m_spriteSheet.getTexture();
		}
		sf::Vector2f        getStart         () const
		{
			return m_start;
		}
		sf::Vector2f        getDimensions    () const
		{
			return m_dimensions;
		}
		sf::Vector2f        getOffset        () const
		{
			return m_offset;
		}
		sf::Vector2u        getRowsAndColumns() const
		{
			return m_frameDistribution;
		}
		float               getFPS           () const
		{
			return m_fps;
		}
		std::size_t         getFrameCount    () const
		{
			return m_frameDistribution.x * m_frameDistribution.y;
		}
		// Mutators
		void setStart         (const sf::Vector2f & position)
		{
			m_start = position;
		}
		void setStart         (float x, float y)
		{
			m_start.x = x;
			m_start.y = y;
		}
		void setFPS           (float fps)
		{
			m_fps = fps;
		}
		void setPosition      (const sf::Vector2f & position)
		{
			m_spriteSheet.setPosition(position);
		}
		void setPosition      (float x, float y)
		{
			m_spriteSheet.setPosition(x, y);
		}
		void setDimensions    (const sf::Vector2f & dimensions)
		{
			m_dimensions = dimensions;
		}
		void setDimensions    (float x, float y)
		{
			m_dimensions.x = x;
			m_dimensions.y = y;
		}
		void setOffset        (const sf::Vector2f & offset)
		{
			m_offset = offset;
		}
		void setOffset        (float x, float y)
		{
			m_offset.x = x;
			m_offset.y = y;
		}
		void setRowsAndColumns(const sf::Vector2u & frameDistribution)
		{
			m_frameDistribution = frameDistribution;
		}
		void setRowsAndColumns(std::size_t x, std::size_t y)
		{
			m_frameDistribution.x = x;
			m_frameDistribution.y = y;
		}
		void setSpriteSheet   (const sf::Sprite & sprite)
		{
			m_spriteSheet = sprite;
		}
		void setRotation      (float angle)
		{
			m_spriteSheet.setRotation(angle);
		}
		void setColor         (const sf::Color & color)
		{
			m_spriteSheet.setColor(color);
		}
		// Utilities
		std::size_t currentFrame      () const
		{
			return static_cast<std::size_t>((m_timer.getElapsedTime().asSeconds()) * m_fps) % getFrameCount();
		}
		std::size_t currentFrame      (sf::Time time) const
		{
			return static_cast<std::size_t>((time.asSeconds() * m_fps)) % getFrameCount();
		}
		sf::IntRect currentTextureRect() const
		{
			std::size_t frame = currentFrame();
			std::size_t x = frame % m_frameDistribution.x;
			std::size_t y = (frame - x) / m_frameDistribution.x;
			return sf::IntRect(static_cast<int>(m_start.x + x * m_dimensions.x + x * m_offset.x), static_cast<int>(m_start.y + y * m_dimensions.y + y * m_offset.y), static_cast<int>(m_dimensions.x), static_cast<int>(m_dimensions.y));
		}
		sf::IntRect currentTextureRect(sf::Time time) const
		{
			std::size_t frame = currentFrame(time);
			std::size_t x = frame % m_frameDistribution.x;
			std::size_t y = (frame - x) / m_frameDistribution.x;
			return sf::IntRect(static_cast<int>(m_start.x + x * m_dimensions.x + x * m_offset.x), static_cast<int>(m_start.y + y * m_dimensions.y + y * m_offset.y), static_cast<int>(m_dimensions.x), static_cast<int>(m_dimensions.y));
		}
		sf::IntRect currentTextureRect(std::size_t frame) const
		{
			frame %= getFrameCount();
			std::size_t x = frame % m_frameDistribution.x;
			std::size_t y = (frame - x) / m_frameDistribution.x;
			return sf::IntRect(static_cast<int>(m_start.x + x * m_dimensions.x + x * m_offset.x), static_cast<int>(m_start.y + y * m_dimensions.y + y * m_offset.y), static_cast<int>(m_dimensions.x), static_cast<int>(m_dimensions.y));
		}
		void        draw              (sf::RenderTarget & target, sf::RenderStates states = sf::RenderStates::Default) const
		{
			sf::Sprite sprite(m_spriteSheet);
			sprite.setTextureRect(currentTextureRect());
			target.draw(sprite, states);
		}
		void        draw              (sf::RenderTarget & target, std::size_t frame, sf::RenderStates states = sf::RenderStates::Default) const
		{
			sf::Sprite sprite(m_spriteSheet);
			sprite.setTextureRect(currentTextureRect(frame));
			target.draw(sprite, states);
		}
		void        draw              (sf::RenderTarget & target, sf::Time time, sf::RenderStates states = sf::RenderStates::Default) const
		{
			sf::Sprite sprite(m_spriteSheet);
			sprite.setTextureRect(currentTextureRect(time));
			target.draw(sprite, states);
		}
		void        draw              (sf::RenderTarget & target, const sf::Vector2f & position, sf::RenderStates states = sf::RenderStates::Default) const
		{
			sf::Sprite sprite(m_spriteSheet);
			sprite.setPosition(position);
			sprite.setTextureRect(currentTextureRect());
			target.draw(sprite, states);
		}
		void        draw              (sf::RenderTarget & target, const sf::Vector2f & position, sf::Time time, sf::RenderStates states = sf::RenderStates::Default) const
		{
			sf::Sprite sprite(m_spriteSheet);
			sprite.setPosition(position);
			sprite.setTextureRect(currentTextureRect(time));
			target.draw(sprite, states);
		}
		void        draw              (sf::RenderTarget & target, const sf::Vector2f & position, std::size_t frame, sf::RenderStates states = sf::RenderStates::Default) const
		{
			sf::Sprite sprite(m_spriteSheet);
			sprite.setPosition(position);
			sprite.setTextureRect(currentTextureRect(frame));
			target.draw(sprite, states);
		}
	};
}