#pragma once

#include <map>
#include <cassert>

#include <SFML/Graphics/Texture.hpp>
#include <SFML/System/String.hpp>

namespace sfext
{
	typedef std::map<sf::String, sf::Texture>::iterator               TextureIterator;
	typedef std::map<sf::String, sf::Texture>::const_iterator         ConstTextureIterator;
	typedef std::map<sf::String, sf::Texture>::reverse_iterator       ReverseTextureIterator;
	typedef std::map<sf::String, sf::Texture>::const_reverse_iterator ConstReverseTextureIterator;

	class TextureHandler final
	{
	private:
		std::map<sf::String, sf::Texture> m_textures;
	public:
		// Constructors
		TextureHandler()
		{
		}
		TextureHandler(const TextureHandler & rhs) : m_textures(rhs.m_textures)
		{
		}
		// Destructor
		~TextureHandler()
		{
		}
		// Accessors
		const sf::Texture & getTexture(const sf::String & alias) const
		{
			if (hasTexture(alias))
				return m_textures.at(alias);
			else
				throw std::invalid_argument("The texture <" + alias + "> does not exist.");
		}
		// Mutators
		void setRepeated(const sf::String & alias, bool repeated)
		{
			if (hasTexture(alias))
				m_textures.at(alias).setRepeated(repeated);
			else
				throw std::invalid_argument("The texture <" + alias + "> does not exist.");
		}
		void setSmooth  (const sf::String & alias, bool smooth)
		{
			if (hasTexture(alias))
				m_textures.at(alias).setSmooth(smooth);
			else
				throw std::invalid_argument("The texture <" + alias + "> does not exist.");
		}
		// Utilities
		bool      addTexture   (const sf::String & filePath, const sf::String & alias, const sf::IntRect & area = sf::IntRect())
		{
			sf::Texture texture;
			if (texture.loadFromFile(filePath, area))
			{
				m_textures[alias] = texture;
				return true;
			}
			return false;
		}
		bool      addTexture   (const sf::String & filePath, const sf::String & alias, bool smooth, bool repeated, const sf::IntRect & area = sf::IntRect())
		{
			sf::Texture texture;
			if (texture.loadFromFile(filePath, area))
			{
				texture.setRepeated(repeated);
				texture.setSmooth(smooth);
				m_textures[alias] = texture;
				return true;
			}
			return false;
		}
		bool      addTexture   (const sf::Image & image, const sf::String & alias, const sf::IntRect & area = sf::IntRect())
		{
			sf::Texture texture;
			if (texture.loadFromImage(image, area))
			{
				m_textures[alias] = texture;
				return true;
			}
			return false;
		}
		bool      addTexture   (const sf::Image & image, const sf::String & alias, bool smooth, bool repeated, const sf::IntRect & area = sf::IntRect())
		{
			sf::Texture texture;
			if (texture.loadFromImage(image, area))
			{
				texture.setRepeated(repeated);
				texture.setSmooth(smooth);
				m_textures[alias] = texture;
				return true;
			}
			return false;
		}
		bool      hasTexture   (const sf::String & alias) const
		{
			return m_textures.find(alias) != m_textures.cend();
		}
		bool      removeTexture(const sf::String & alias)
		{
			ConstTextureIterator texture = m_textures.find(alias);
			if (texture != m_textures.cend())
			{
				m_textures.erase(texture);
				return true;
			}
			else
				throw std::invalid_argument("The texture <" + alias + "> does not exist.");
		}
		sf::Image copyToImage  (const sf::String & alias) const
		{
			if (hasTexture(alias))
				return m_textures.at(alias).copyToImage();
			else
				throw std::invalid_argument("The texture <" + alias + "> does not exist.");
		}
		// Iterators
		TextureIterator             begin  ()
		{
			return m_textures.begin();
		}
		TextureIterator             end    ()
		{
			return m_textures.end();
		}
		ConstTextureIterator        cbegin () const
		{
			return m_textures.cbegin();
		}
		ConstTextureIterator        cend   () const
		{
			return m_textures.cend();
		}
		ReverseTextureIterator      rbegin ()
		{
			return m_textures.rbegin();
		}
		ReverseTextureIterator      rend   ()
		{
			return m_textures.rend();
		}
		ConstReverseTextureIterator crbegin() const
		{
			return m_textures.crbegin();
		}
		ConstReverseTextureIterator crend  () const
		{
			return m_textures.crend();
		}
		TextureIterator             find   (const sf::String & alias)
		{
			return m_textures.find(alias);
		}
		ConstTextureIterator        find   (const sf::String & alias) const
		{
			return m_textures.find(alias);
		}
	};
}