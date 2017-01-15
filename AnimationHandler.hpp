#pragma once

// TODO: write more detailed documentation

#include <map>
#include <string>

#include <SFML/System/String.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/VertexArray.hpp>

#include "SpriteHandler.hpp"
#include "Animation.hpp"

namespace sfext
{
	typedef std::map<sf::String, Animation>::iterator               AnimationIterator;
	typedef std::map<sf::String, Animation>::const_iterator         ConstAnimationIterator;
	typedef std::map<sf::String, Animation>::reverse_iterator       ReverseAnimationIterator;
	typedef std::map<sf::String, Animation>::const_reverse_iterator ConstReverseAnimationIterator;

	class AnimationHandler final
	{
	private:
		SpriteHandler m_sprites;
		std::map<sf::String, Animation> m_animations;
	public:
		// Constructors
		AnimationHandler()
		{
		}
		AnimationHandler(const AnimationHandler & rhs) : m_sprites(rhs.m_sprites), m_animations(rhs.m_animations)
		{
		}
		// Destructor
		~AnimationHandler()
		{
		}
		// Accessors
		sf::Vector2f        getPosition      (const sf::String & alias) const
		{
			if (hasAnimation(alias))
				return m_animations.at(alias).getPosition();
			else
				throw std::invalid_argument("The animation <" + alias + "> does not exist.");
		}
		float               getFPS           (const sf::String & alias) const
		{
			if (hasAnimation(alias))
				return m_animations.at(alias).getFPS();
			else
				throw std::invalid_argument("The animation <" + alias + "> does not exist.");
		}
		sf::Vector2f        getStart         (const sf::String & alias) const
		{
			if (hasAnimation(alias))
				return m_animations.at(alias).getStart();
			else
				throw std::invalid_argument("The animation <" + alias + "> does not exist.");
		}
		sf::Vector2f        getOffset        (const sf::String & alias) const
		{
			if (hasAnimation(alias))
				return m_animations.at(alias).getOffset();
			else
				throw std::invalid_argument("The animation <" + alias + "> does not exist.");
		}
		sf::Vector2f        getDimensions    (const sf::String & alias) const
		{
			if (hasAnimation(alias))
				return m_animations.at(alias).getDimensions();
			else
				throw std::invalid_argument("The animation <" + alias + "> does not exist.");
		}
		sf::Vector2u        getRowsAndColumns(const sf::String & alias) const
		{
			if (hasAnimation(alias))
				return m_animations.at(alias).getRowsAndColumns();
			else
				throw std::invalid_argument("The animation <" + alias + "> does not exist.");
		}
		std::size_t         getFrameCount    (const sf::String & alias) const
		{
			if (hasAnimation(alias))
				return m_animations.at(alias).getFrameCount();
			else
				throw std::invalid_argument("The animation <" + alias + "> does not exist.");
		}
		const sf::Texture * getTexture       (const sf::String & alias) const
		{
			if (hasAnimation(alias))
				return m_animations.at(alias).getTexture();
			else
				throw std::invalid_argument("The animation <" + alias + "> does not exist.");
		}
		const sf::Sprite &  getSpriteSheet   (const sf::String & alias) const
		{
			if (hasAnimation(alias))
				return m_animations.at(alias).getSpriteSheet();
			else
				throw std::invalid_argument("The animation <" + alias + "> does not exist.");
		}
		// Mutators
		void setFPS     (const sf::String & alias, float fps)
		{
			if (hasAnimation(alias))
				m_animations.at(alias).setFPS(fps);
			else
				throw std::invalid_argument("The animation <" + alias + "> does not exist.");
		}
		void setStart   (const sf::String & alias, const sf::Vector2f & start)
		{
			if (hasAnimation(alias))
				m_animations.at(alias).setStart(start);
			else
				throw std::invalid_argument("The animation <" + alias + "> does not exist.");
		}
		void setStart   (const sf::String & alias, float x, float y)
		{
			if (hasAnimation(alias))
				m_animations.at(alias).setStart(x, y);
			else
				throw std::invalid_argument("The animation <" + alias + "> does not exist.");
		}
		void setPosition(const sf::String & alias, const sf::Vector2f & pos)
		{
			if (hasAnimation(alias))
				m_animations.at(alias).setPosition(pos);
			else
				throw std::invalid_argument("The animation <" + alias + "> does not exist.");
		}
		void setPosition(const sf::String & alias, float x, float y)
		{
			if (hasAnimation(alias))
				m_animations.at(alias).setPosition(x, y);
			else
				throw std::invalid_argument("The animation <" + alias + "> does not exist.");
		}
		// Utilities
		bool addAnimation   (const sf::String & filePath, const sf::String & alias, const sf::Vector2f & start, const sf::Vector2f & dimensions, const sf::Vector2f & offset, const sf::Vector2u & rowsAndColumns, float fps = 24.f)
		{
			if (m_sprites.addTexture(filePath, alias))
			{
				m_animations[alias] = Animation(m_sprites.find(alias)->second, start, dimensions, offset, rowsAndColumns, fps);
				return true;
			}
			return false;
		}
		bool addAnimation   (const sf::Image & image, const sf::String & alias, const sf::Vector2f & start, const sf::Vector2f & dimensions, const sf::Vector2f & offset, const sf::Vector2u & rowsAndColumns, float fps = 24.f)
		{
			if (m_sprites.addTexture(image, alias))
			{
				m_animations[alias] = Animation(m_sprites.find(alias)->second, start, dimensions, offset, rowsAndColumns, fps);
				return true;
			}
			return false;
		}
		bool hasAnimation   (const sf::String & alias) const
		{
			return m_animations.find(alias) != m_animations.cend();
		}
		bool removeAnimation(const sf::String & alias)
		{
			if (m_sprites.removeTexture(alias))
			{
				ConstAnimationIterator animation = m_animations.find(alias);
				m_animations.erase(animation);
				return true;
			}
			return false;
		}
		void draw           (sf::RenderTarget & target, sf::String & alias, sf::RenderStates states = sf::RenderStates::Default) const
		{
			if (hasAnimation(alias))
				m_animations.at(alias).draw(target);
			else
				throw std::invalid_argument("The animation <" + alias + "> does not exist.");
		}
		void draw           (sf::RenderTarget & target, const sf::String & alias, std::size_t frame, sf::RenderStates states = sf::RenderStates::Default) const
		{
			if (hasAnimation(alias))
				m_animations.at(alias).draw(target, frame, states);
			else
				throw std::invalid_argument("The animation <" + alias + "> does not exist.");
		}
		void draw           (sf::RenderTarget & target, const sf::String & alias, sf::Time time, sf::RenderStates states = sf::RenderStates::Default) const
		{
			if (hasAnimation(alias))
				m_animations.at(alias).draw(target, time, states);
			else
				throw std::invalid_argument("The animation <" + alias + "> does not exist.");
		}
		void draw           (sf::RenderTarget & target, const sf::String & alias, const sf::Vector2f & position, sf::RenderStates states = sf::RenderStates::Default) const
		{
			if (hasAnimation(alias))
				m_animations.at(alias).draw(target, position, states);
			else
				throw std::invalid_argument("The animation <" + alias + "> does not exist.");
		}
		void draw           (sf::RenderTarget & target, const sf::String & alias, const sf::Vector2f & position, sf::Time time, sf::RenderStates states = sf::RenderStates::Default) const
		{
			if (hasAnimation(alias))
				m_animations.at(alias).draw(target, position, time, states);
			else
				throw std::invalid_argument("The animation <" + alias + "> does not exist.");
		}
		void draw           (sf::RenderTarget & target, const sf::String & alias, const sf::Vector2f & position, std::size_t frame, sf::RenderStates states = sf::RenderStates::Default) const
		{
			if (hasAnimation(alias))
				m_animations.at(alias).draw(target, position, frame, states);
			else
				throw std::invalid_argument("The animation <" + alias + "> does not exist.");
		}
		void batch          (sf::RenderTarget & target, const sf::String & alias, const std::vector<sf::Vector2f> & positions, sf::RenderStates states = sf::RenderStates::Default) const
		{
			if (hasAnimation(alias))
			{
				sf::IntRect rectangle = m_animations.at(alias).currentTextureRect();
				sf::VertexArray vertices(sf::Quads);
				for (const sf::Vector2f & position : positions)
				{
					vertices.append(sf::Vertex(position, sf::Vector2f(static_cast<float>(rectangle.left), static_cast<float>(rectangle.top))));
					vertices.append(sf::Vertex(position + sf::Vector2f(static_cast<float>(rectangle.width), 0.f), sf::Vector2f(static_cast<float>(rectangle.left + rectangle.width), static_cast<float>(rectangle.top))));
					vertices.append(sf::Vertex(position + sf::Vector2f(static_cast<float>(rectangle.width), static_cast<float>(rectangle.height)), sf::Vector2f(static_cast<float>(rectangle.left + rectangle.width), static_cast<float>(rectangle.top + rectangle.height))));
					vertices.append(sf::Vertex(position + sf::Vector2f(0.f, static_cast<float>(rectangle.height)), sf::Vector2f(static_cast<float>(rectangle.left), static_cast<float>(rectangle.top + rectangle.height))));
				}
				states.texture = &m_sprites.getTextureHandler().getTexture(alias);
				target.draw(vertices, states);
			}
			else
				throw std::invalid_argument("The animation <" + alias + "> does not exist.");
		}
		void batch          (sf::RenderTarget & target, const sf::String & alias, const std::vector<sf::Vector2f> & positions, std::size_t frame, sf::RenderStates states = sf::RenderStates::Default) const
		{
			if (hasAnimation(alias))
			{
				sf::IntRect rectangle = m_animations.at(alias).currentTextureRect(frame);
				sf::VertexArray vertices(sf::Quads);
				for (const sf::Vector2f & position : positions)
				{
					vertices.append(sf::Vertex(position, sf::Vector2f(static_cast<float>(rectangle.left), static_cast<float>(rectangle.top))));
					vertices.append(sf::Vertex(position + sf::Vector2f(static_cast<float>(rectangle.width), 0.f), sf::Vector2f(static_cast<float>(rectangle.left + rectangle.width), static_cast<float>(rectangle.top))));
					vertices.append(sf::Vertex(position + sf::Vector2f(static_cast<float>(rectangle.width), static_cast<float>(rectangle.height)), sf::Vector2f(static_cast<float>(rectangle.left + rectangle.width), static_cast<float>(rectangle.top + rectangle.height))));
					vertices.append(sf::Vertex(position + sf::Vector2f(0.f, static_cast<float>(rectangle.height)), sf::Vector2f(static_cast<float>(rectangle.left), static_cast<float>(rectangle.top + rectangle.height))));
				}
				states.texture = &m_sprites.getTextureHandler().getTexture(alias);
				target.draw(vertices, states);
			}
			else
				throw std::invalid_argument("The animation <" + alias + "> does not exist.");
		}
		void batch          (sf::RenderTarget & target, const sf::String & alias, const std::vector<sf::Vector2f> & positions, sf::Time time, sf::RenderStates states = sf::RenderStates::Default) const
		{
			if (hasAnimation(alias))
			{
				sf::IntRect rectangle = m_animations.at(alias).currentTextureRect(time);
				sf::VertexArray vertices(sf::Quads);
				for (const sf::Vector2f & position : positions)
				{
					vertices.append(sf::Vertex(position, sf::Vector2f(static_cast<float>(rectangle.left), static_cast<float>(rectangle.top))));
					vertices.append(sf::Vertex(position + sf::Vector2f(static_cast<float>(rectangle.width), 0.f), sf::Vector2f(static_cast<float>(rectangle.left + rectangle.width), static_cast<float>(rectangle.top))));
					vertices.append(sf::Vertex(position + sf::Vector2f(static_cast<float>(rectangle.width), static_cast<float>(rectangle.height)), sf::Vector2f(static_cast<float>(rectangle.left + rectangle.width), static_cast<float>(rectangle.top + rectangle.height))));
					vertices.append(sf::Vertex(position + sf::Vector2f(0.f, static_cast<float>(rectangle.height)), sf::Vector2f(static_cast<float>(rectangle.left), static_cast<float>(rectangle.top + rectangle.height))));
				}
				states.texture = &m_sprites.getTextureHandler().getTexture(alias);
				target.draw(vertices, states);
			}
			else
				throw std::invalid_argument("The animation <" + alias + "> does not exist.");
		}
		void batch          (sf::RenderTarget & target, const sf::String & alias, const std::vector<sf::Vector2f> & positions, const std::vector<std::size_t> & frames, sf::RenderStates states = sf::RenderStates::Default) const
		{
			if (hasAnimation(alias) && positions.size() == frames.size())
			{
				sf::IntRect rectangle;
				sf::VertexArray vertices(sf::Quads);
				for (unsigned int i = 0; i < positions.size(); ++i)
				{
					rectangle = m_animations.at(alias).currentTextureRect(frames.at(i));
					vertices.append(sf::Vertex(positions.at(i), sf::Vector2f(static_cast<float>(rectangle.left), static_cast<float>(rectangle.top))));
					vertices.append(sf::Vertex(positions.at(i) + sf::Vector2f(static_cast<float>(rectangle.width), 0.f), sf::Vector2f(static_cast<float>(rectangle.left + rectangle.width), static_cast<float>(rectangle.top))));
					vertices.append(sf::Vertex(positions.at(i) + sf::Vector2f(static_cast<float>(rectangle.width), static_cast<float>(rectangle.height)), sf::Vector2f(static_cast<float>(rectangle.left + rectangle.width), static_cast<float>(rectangle.top + rectangle.height))));
					vertices.append(sf::Vertex(positions.at(i) + sf::Vector2f(0.f, static_cast<float>(rectangle.height)), sf::Vector2f(static_cast<float>(rectangle.left), static_cast<float>(rectangle.top + rectangle.height))));
				}
				states.texture = &m_sprites.getTextureHandler().getTexture(alias);
				target.draw(vertices, states);
			}
			else
				throw std::invalid_argument("The animation <" + alias + "> does not exist.");
		}
		void batch          (sf::RenderTarget & target, const sf::String & alias, const std::vector<sf::Vector2f> & positions, const std::vector<sf::Time> & times, sf::RenderStates states = sf::RenderStates::Default) const
		{
			if (hasAnimation(alias) && positions.size() == times.size())
			{
				sf::IntRect rectangle;
				sf::VertexArray vertices(sf::Quads);
				for (unsigned int i = 0; i < positions.size(); ++i)
				{
					rectangle = m_animations.at(alias).currentTextureRect(times.at(i));
					vertices.append(sf::Vertex(positions.at(i), sf::Vector2f(static_cast<float>(rectangle.left), static_cast<float>(rectangle.top))));
					vertices.append(sf::Vertex(positions.at(i) + sf::Vector2f(static_cast<float>(rectangle.width), 0.f), sf::Vector2f(static_cast<float>(rectangle.left + rectangle.width), static_cast<float>(rectangle.top))));
					vertices.append(sf::Vertex(positions.at(i) + sf::Vector2f(static_cast<float>(rectangle.width), static_cast<float>(rectangle.height)), sf::Vector2f(static_cast<float>(rectangle.left + rectangle.width), static_cast<float>(rectangle.top + rectangle.height))));
					vertices.append(sf::Vertex(positions.at(i) + sf::Vector2f(0.f, static_cast<float>(rectangle.height)), sf::Vector2f(static_cast<float>(rectangle.left), static_cast<float>(rectangle.top + rectangle.height))));
				}
				states.texture = &m_sprites.getTextureHandler().getTexture(alias);
				target.draw(vertices, states);
			}
			else
				throw std::invalid_argument("The animation <" + alias + "> does not exist.");
		}
		// Iterators
		AnimationIterator             begin  ()
		{
			return m_animations.begin();
		}
		AnimationIterator             end    ()
		{
			return m_animations.end();
		}
		ConstAnimationIterator        cbegin () const
		{
			return m_animations.cbegin();
		}
		ConstAnimationIterator        cend   () const
		{
			return m_animations.cend();
		}
		ReverseAnimationIterator      rbegin ()
		{
			return m_animations.rbegin();
		}
		ReverseAnimationIterator      rend   ()
		{
			return m_animations.rend();
		}
		ConstReverseAnimationIterator crbegin() const
		{
			return m_animations.crbegin();
		}
		ConstReverseAnimationIterator crend  () const
		{
			return m_animations.crend();
		}
		AnimationIterator             find   (const sf::String & alias)
		{
			return m_animations.find(alias);
		}
		ConstAnimationIterator        find   (const sf::String & alias) const
		{
			return m_animations.find(alias);
		}
	};
}