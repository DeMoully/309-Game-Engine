#pragma once

#include <map>

#include <SFML/Graphics/Font.hpp>
#include <SFML/System/String.hpp>

// TODO: tests
// TODO: documentation

namespace sfext
{
	class FontHandler final
	{
	private:
		std::map<sf::String, sf::Font> m_fonts;
	public:
		// Constructors
		FontHandler()
		{
		}
		FontHandler(const FontHandler & rhs) : m_fonts(rhs.m_fonts)
		{
		}
		// Destructor
		~FontHandler()
		{
		}
		// Accessors
		const sf::Font & getFont(const sf::String & fontAlias) const
		{
			if (hasFont(fontAlias))
				return m_fonts.at(fontAlias);
			else
				throw std::invalid_argument("The font <" + fontAlias + "> does not exist.");
		}
		// Utilities
		bool        addFont   (const sf::String & filePath, const sf::String & fontAlias)
		{
			sf::Font font;
			if (font.loadFromFile(filePath))
			{
				m_fonts[fontAlias] = font;
				return true;
			}
			return false;
		}
		bool        hasFont   (const sf::String & fontAlias) const
		{
			return m_fonts.find(fontAlias) != m_fonts.cend();
		}
		bool        removeFont(const sf::String & fontAlias)
		{
			std::map<sf::String, sf::Font>::const_iterator font = m_fonts.find(fontAlias);
			if (font != m_fonts.cend())
			{
				m_fonts.erase(font);
				return true;
			}
			return false;
		}
		std::size_t size      () const
		{
			return m_fonts.size();
		}
	};
}