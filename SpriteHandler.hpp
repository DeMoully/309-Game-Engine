#pragma once

#include <map>
#include <string>

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/System/String.hpp>
#include <SFML/Graphics/RenderTarget.hpp>

#include "TextureHandler.hpp"

namespace sfext
{
	typedef std::map<sf::String, sf::Sprite>::iterator               SpriteIterator;
	typedef std::map<sf::String, sf::Sprite>::const_iterator         ConstSpriteIterator;
	typedef std::map<sf::String, sf::Sprite>::reverse_iterator       ReverseSpriteIterator;
	typedef std::map<sf::String, sf::Sprite>::const_reverse_iterator ConstReverseSpriteIterator;

	class SpriteHandler final
	{
	private:
		TextureHandler m_textures;
		std::map<sf::String, sf::Sprite> m_sprites;
	public:
		// Constructors
		SpriteHandler()
		{
		}
		explicit SpriteHandler(const TextureHandler & textureHandler) : m_textures(textureHandler)
		{
			for (const auto & texture : m_textures)
			{
				m_sprites[texture.first] = sf::Sprite(texture.second);
			}
		}
		SpriteHandler         (const SpriteHandler & rhs) : m_textures(rhs.m_textures), m_sprites(rhs.m_sprites)
		{
		}
		// Destructor
		~SpriteHandler()
		{
		}
		// Accessors
		sf::Vector2f           getPosition      (const sf::String & alias) const
		{
			if (hasTexture(alias))
				return m_sprites.at(alias).getPosition();
			else
				throw std::invalid_argument("The sprite <" + alias + "> does not exist.");
		}
		const TextureHandler & getTextureHandler() const
		{
			return m_textures;
		}
		// Mutators
		void setRepeated   (const sf::String & alias, bool repeated)
		{
			m_textures.setRepeated(alias, repeated);
		}
		void setSmooth     (const sf::String & alias, bool smooth)
		{
			m_textures.setSmooth(alias, smooth);
		}
		void setPosition   (const sf::String & alias, float x, float y)
		{
			if (hasTexture(alias))
				m_sprites[alias].setPosition(x, y);
			else
				throw std::invalid_argument("The sprite <" + alias + "> does not exist.");
		}
		void setPosition   (const sf::String & alias, const sf::Vector2f & position)
		{
			if (hasTexture(alias))
				m_sprites[alias].setPosition(position);
			else
				throw std::invalid_argument("The sprite <" + alias + "> does not exist.");
		}
		void move          (const sf::String & alias, float x, float y)
		{
			if (hasTexture(alias))
				m_sprites.at(alias).move(x, y);
			else
				throw std::invalid_argument("The sprite <" + alias + "> does not exist.");
		}
		void move          (const sf::String & alias, const sf::Vector2f & offset)
		{
			if (hasTexture(alias))
				m_sprites.at(alias).move(offset);
			else
				throw std::invalid_argument("The sprite <" + alias + "> does not exist.");
		}
		void setScale      (const sf::String & alias, float x, float y)
		{
			if (hasTexture(alias))
				m_sprites[alias].setScale(x, y);
			else
				throw std::invalid_argument("The sprite <" + alias + "> does not exist.");
		}
		void setScale      (const sf::String & alias, const sf::Vector2f & factors)
		{
			if (hasTexture(alias))
				m_sprites[alias].setScale(factors);
			else
				throw std::invalid_argument("The sprite <" + alias + "> does not exist.");
		}
		void scale         (const sf::String & alias, float x, float y)
		{
			if (hasTexture(alias))
				m_sprites[alias].scale(x, y);
			else
				throw std::invalid_argument("The sprite <" + alias + "> does not exist.");
		}
		void scale         (const sf::String & alias, const sf::Vector2f & factors)
		{
			if (hasTexture(alias))
				m_sprites[alias].scale(factors);
			else
				throw std::invalid_argument("The sprite <" + alias + "> does not exist.");
		}
		void setColor      (const sf::String & alias, const sf::Color & color)
		{
			if (hasTexture(alias))
				m_sprites[alias].setColor(color);
			else
				throw std::invalid_argument("The sprite <" + alias + "> does not exist.");
		}
		void setTextureRect(const sf::String & alias, const sf::IntRect & rectangle)
		{
			if (hasTexture(alias))
				m_sprites[alias].setTextureRect(rectangle);
			else
				throw std::invalid_argument("The sprite <" + alias + "> does not exist.");
		}
		void setRotation   (const sf::String & alias, float angle)
		{
			if (hasTexture(alias))
				m_sprites[alias].setRotation(angle);
			else
				throw std::invalid_argument("The sprite <" + alias + "> does not exist.");
		}
		void rotate        (const sf::String & alias, float angle)
		{
			if (hasTexture(alias))
				m_sprites[alias].rotate(angle);
			else
				throw std::invalid_argument("The sprite <" + alias + "> does not exist.");
		}
		void setOrigin     (const sf::String & alias, float x, float y)
		{
			if (hasTexture(alias))
				m_sprites[alias].setOrigin(x, y);
			else
				throw std::invalid_argument("The sprite <" + alias + "> does not exist.");
		}
		void setOrigin     (const sf::String & alias, const sf::Vector2f & origin)
		{
			if (hasTexture(alias))
			{
				m_sprites[alias].setOrigin(origin);
			}
		}
		// Utilities
		const sf::Sprite & getSprite(const sf::String & alias)
		{
			if (hasTexture(alias))
				return m_sprites.at(alias);
			else
				throw std::invalid_argument("The sprite <" + alias + "> does not exist.");
		}
		bool addTexture   (const sf::String & filePath, const sf::String & alias, const sf::IntRect & area = sf::IntRect())
		{
			if (m_textures.addTexture(filePath, alias, area))
			{
				m_sprites[alias] = sf::Sprite(m_textures.getTexture(alias));
				return true;
			}
			return false;
		}
		bool addTexture   (const sf::String & filePath, const sf::String & alias, bool repeated, bool smooth, const sf::IntRect & area = sf::IntRect())
		{
			if (m_textures.addTexture(filePath, alias, repeated, smooth, area))
			{
				m_sprites[alias] = sf::Sprite(m_textures.getTexture(alias));
				return true;
			}
			return false;
		}
		bool addTexture   (const sf::Image & image, const sf::String & alias, const sf::IntRect & area = sf::IntRect())
		{
			if (m_textures.addTexture(image, alias, area))
			{
				m_sprites[alias] = sf::Sprite(m_textures.getTexture(alias));
				return true;
			}
			return false;
		}
		bool addTexture   (const sf::Image & image, const sf::String & alias, bool repeated, bool smooth, const sf::IntRect & area = sf::IntRect())
		{
			if (m_textures.addTexture(image, alias, repeated, smooth, area))
			{
				m_sprites[alias] = sf::Sprite(m_textures.getTexture(alias));
				return true;
			}
			return false;
		}
		bool hasTexture   (const sf::String & alias) const
		{
			return m_textures.hasTexture(alias);
		}
		bool removeTexture(const sf::String & alias)
		{
			if (m_textures.removeTexture(alias))
			{
				ConstSpriteIterator sprite = m_sprites.find(alias);
				m_sprites.erase(sprite);
				return true;
			}
			return false;
		}
		void draw         (sf::RenderTarget & target, const sf::String & alias, sf::RenderStates states = sf::RenderStates::Default) const
		{
			if (hasTexture(alias))
				target.draw(m_sprites.at(alias), states);
			else
				throw std::invalid_argument("The sprite <" + alias + "> does not exist.");
		}
		void draw         (sf::RenderTarget & target, const sf::String & alias, sf::Vector2f & position, const sf::RenderStates states = sf::RenderStates::Default) const
		{
			if (hasTexture(alias))
			{
				sf::Sprite tempSprite(m_sprites.at(alias));
				tempSprite.setPosition(position);
				target.draw(tempSprite, states);
			}
			else
				throw std::invalid_argument("The sprite <" + alias + "> does not exist.");
		}
		void draw         (sf::RenderTarget & target, const sf::String & alias, sf::IntRect & rectangle, const sf::RenderStates states = sf::RenderStates::Default) const
		{
			if (hasTexture(alias))
			{
				sf::Sprite tempSprite(m_sprites.at(alias));
				tempSprite.setTextureRect(rectangle);
				target.draw(tempSprite, states);
			}
			else
				throw std::invalid_argument("The sprite <" + alias + "> does not exist.");
		}
		void draw         (sf::RenderTarget & target, const sf::String & alias, sf::Vector2f & position, const sf::IntRect & rectangle, sf::RenderStates states = sf::RenderStates::Default) const
		{
			if (hasTexture(alias))
			{
				sf::Sprite tempSprite(m_sprites.at(alias));
				tempSprite.setTextureRect(rectangle);
				tempSprite.setPosition(position);
				target.draw(tempSprite, states);
			}
			else
				throw std::invalid_argument("The sprite <" + alias + "> does not exist.");
		}
		void batch        (sf::RenderTarget & target, const sf::String & alias, const std::vector<sf::Vector2f> & positions, sf::RenderStates states = sf::RenderStates::Default) const
		{
			if (hasTexture(alias))
			{
				sf::VertexArray vertices(sf::Quads);
				sf::FloatRect globalBounds = m_sprites.at(alias).getGlobalBounds();
				sf::IntRect textureBounds = m_sprites.at(alias).getTextureRect();
				for (const sf::Vector2f & position : positions)
				{
					vertices.append(sf::Vertex(position, sf::Vector2f(0.f, 0.f)));
					vertices.append(sf::Vertex(position + sf::Vector2f(globalBounds.width, 0.f), sf::Vector2f(static_cast<float>(textureBounds.width), 0.f)));
					vertices.append(sf::Vertex(position + sf::Vector2f(globalBounds.width, globalBounds.height), sf::Vector2f(static_cast<float>(textureBounds.width), static_cast<float>(textureBounds.height))));
					vertices.append(sf::Vertex(position + sf::Vector2f(0.f, globalBounds.height), sf::Vector2f(0.f, static_cast<float>(textureBounds.height))));
				}
				states.texture = &m_textures.getTexture(alias);
				target.draw(vertices, states);
			}
			else
				throw std::invalid_argument("The sprite <" + alias + "> does not exist.");
		}
		void batch        (sf::RenderTarget & target, const sf::String & alias, const std::vector<sf::Vector2f> & positions, const std::vector<sf::IntRect> & rectangles, sf::RenderStates states = sf::RenderStates::Default) const
		{
			if (hasTexture(alias) && positions.size() == rectangles.size())
			{
				sf::VertexArray vertices(sf::Quads);
				for (unsigned int i = 0; i < positions.size(); ++i)
				{
					vertices.append(sf::Vertex(positions.at(i), sf::Vector2f(static_cast<float>(rectangles.at(i).left), static_cast<float>(rectangles.at(i).top))));
					vertices.append(sf::Vertex(positions.at(i) + sf::Vector2f(static_cast<float>(rectangles.at(i).width), 0.f), sf::Vector2f(static_cast<float>(rectangles.at(i).left) + static_cast<float>(rectangles.at(i).width), static_cast<float>(rectangles.at(i).top))));
					vertices.append(sf::Vertex(positions.at(i) + sf::Vector2f(static_cast<float>(rectangles.at(i).width), static_cast<float>(rectangles.at(i).height)), sf::Vector2f(static_cast<float>(rectangles.at(i).left) + static_cast<float>(rectangles.at(i).width), static_cast<float>(rectangles.at(i).top) + static_cast<float>(rectangles.at(i).height))));
					vertices.append(sf::Vertex(positions.at(i) + sf::Vector2f(0.f, static_cast<float>(rectangles.at(i).height)), sf::Vector2f(static_cast<float>(rectangles.at(i).left), static_cast<float>(rectangles.at(i).top) + static_cast<float>(rectangles.at(i).height))));
				}
				states.texture = &m_textures.getTexture(alias);
				target.draw(vertices, states);
			}
			else
				throw std::invalid_argument("The sprite <" + alias + "> does not exist.");
		}
		void batch        (sf::RenderTarget & target, const sf::String & alias, const std::vector<sf::Vector2f> & positions, const sf::IntRect & rectangle, sf::RenderStates states = sf::RenderStates::Default) const
		{
			if (hasTexture(alias))
			{
				sf::VertexArray vertices(sf::Quads);
				for (unsigned int i = 0; i < positions.size(); ++i)
				{
					vertices.append(sf::Vertex(positions.at(i), sf::Vector2f(static_cast<float>(rectangle.left), static_cast<float>(rectangle.top))));
					vertices.append(sf::Vertex(positions.at(i) + sf::Vector2f(static_cast<float>(rectangle.width), 0.f), sf::Vector2f(static_cast<float>(rectangle.left) + static_cast<float>(rectangle.width), static_cast<float>(rectangle.top))));
					vertices.append(sf::Vertex(positions.at(i) + sf::Vector2f(static_cast<float>(rectangle.width), static_cast<float>(rectangle.height)), sf::Vector2f(static_cast<float>(rectangle.left) + static_cast<float>(rectangle.width), static_cast<float>(rectangle.top) + static_cast<float>(rectangle.height))));
					vertices.append(sf::Vertex(positions.at(i) + sf::Vector2f(0.f, static_cast<float>(rectangle.height)), sf::Vector2f(static_cast<float>(rectangle.left), static_cast<float>(rectangle.top) + static_cast<float>(rectangle.height))));
				}
				states.texture = &m_textures.getTexture(alias);
				target.draw(vertices, states);
			}
			else
				throw std::invalid_argument("The sprite <" + alias + "> does not exist.");
		}
		// Iterators
		SpriteIterator             begin  ()
		{
			return m_sprites.begin();
		}
		SpriteIterator             end    ()
		{
			return m_sprites.end();
		}
		ConstSpriteIterator        cbegin () const
		{
			return m_sprites.cbegin();
		}
		ConstSpriteIterator        cend   () const
		{
			return m_sprites.cend();
		}
		ReverseSpriteIterator      rbegin ()
		{
			return m_sprites.rbegin();
		}
		ReverseSpriteIterator      rend   ()
		{
			return m_sprites.rend();
		}
		ConstReverseSpriteIterator crbegin() const
		{
			return m_sprites.crbegin();
		}
		ConstReverseSpriteIterator crend  () const
		{
			return m_sprites.crend();
		}
		SpriteIterator             find   (const sf::String & alias)
		{
			return m_sprites.find(alias);
		}
		ConstSpriteIterator        find   (const sf::String & alias) const
		{
			return m_sprites.find(alias);
		}
	};
}