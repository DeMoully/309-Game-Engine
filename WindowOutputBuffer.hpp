// UPDATE FORMATTING
// ADD NULLPTR CHECKS
// MAKE IT LESS BAD

#pragma once

#include <string>
#include <functional>
#include <vector>
#include <deque>
#include <list>
#include <array>
#include <set>
#include <iterator>

#include <SFML/Graphics/Text.hpp>
#include <SFML/System/String.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Shape.hpp>

#include "FontHandler.hpp"

namespace sfext
{
	class WindowOutputBuffer final
	{
	private:
		sf::RenderTarget * target;
		FontHandler        fonts;
		sf::Text           text;
		sf::Sprite         sprite;
	public:
		// Constructors
		WindowOutputBuffer         () : target(nullptr)
		{
		}
		explicit WindowOutputBuffer(sf::RenderTarget * const trgt) : target(trgt)
		{
		}
		explicit WindowOutputBuffer(const FontHandler & fontHandler) : target(nullptr), fonts(fontHandler)
		{
		}
		// Destructor
		~WindowOutputBuffer()
		{
		}
		// Accessors
		sf::Vector2f getPosition   () const
		{
			return text.getPosition();
		}
		float        getLineSpacing() const
		{
			if (text.getFont())
			{
				return static_cast<float>(text.getFont()->getLineSpacing(text.getCharacterSize()));
			}
			return 0.f;
		}
		// Mutators
		void setPosition     (const sf::Vector2f & position)
		{
			text.setPosition(position);
			sprite.setPosition(position);
		}
		void setPosition     (float x, float y)
		{
			text.setPosition(x, y);
			sprite.setPosition(x, y);
		}
		void setFont         (const sf::String & fontAlias)
		{
			if (fonts.hasFont(fontAlias))
			{
				text.setFont(fonts.getFont(fontAlias));
			}
		}
		void setColor        (const sf::Color & color)
		{
			text.setColor(color);
		}
		void setCharacterSize(unsigned int size)
		{
			text.setCharacterSize(size);
		}
		// Utilities
		bool validRenderTarget() const
		{
			return target != nullptr;
		}
		void bindRenderTarget (sf::RenderTarget * const trgt)
		{
			target = trgt;
		}
		void move             (const sf::Vector2f & offset)
		{
			text.move(offset);
		}
		void move             (float x, float y)
		{
			text.move(x, y);
		}
		// Overloaded Operators
		WindowOutputBuffer & operator << (const std::string & str)
		{
			if (validRenderTarget())
			{
				text.setString(str);
				target->draw(text);
				text.move(text.getGlobalBounds().width, 0.f);
				sprite.move(text.getGlobalBounds().width, 0.f);
			}
			return *this;
		}
		WindowOutputBuffer & operator << (int value)
		{
			return (*this) << std::to_string(value);
		}
		WindowOutputBuffer & operator << (float value)
		{
			return (*this) << std::to_string(value);
		}
		WindowOutputBuffer & operator << (double value)
		{
			return (*this) << std::to_string(value);
		}
		WindowOutputBuffer & operator << (unsigned int value)
		{
			return (*this) << std::to_string(value);
		}
		WindowOutputBuffer & operator << (long value)
		{
			return (*this) << std::to_string(value);
		}
		WindowOutputBuffer & operator << (long double value)
		{
			return (*this) << std::to_string(value);
		}
		WindowOutputBuffer & operator << (unsigned long value)
		{
			return (*this) << std::to_string(value);
		}
		WindowOutputBuffer & operator << (long long value)
		{
			return (*this) << std::to_string(value);
		}
		WindowOutputBuffer & operator << (unsigned long long value)
		{
			return (*this) << std::to_string(value);
		}
		template <class T>
		WindowOutputBuffer & operator << (const sf::Vector2<T> & value)
		{
			return (*this) << "(" << value.x << ", " << value.y << ")";
		}
		template <class T>
		WindowOutputBuffer & operator << (const sf::Vector3<T> & value)
		{
			return (*this) << "(" << value.x << ", " << value.y << ", " << value.z << ")";
		}
		WindowOutputBuffer & operator << (sf::Time value)
		{
			return (*this) << value.asSeconds();
		}
		WindowOutputBuffer & operator << (const sf::Sprite & spr)
		{
			if (validRenderTarget())
			{
				sprite = spr;
				sprite.setPosition(getPosition());
				target->draw(sprite);
				text.move(sprite.getGlobalBounds().width, 0.f);
			}
			return *this;
		}
		WindowOutputBuffer & operator << (const sf::Texture & texture)
		{
			if (validRenderTarget())
			{
				sprite.setTexture(texture);
				(*this) << sprite;
			}
			return *this;
		}
	};

	//Output Manipulators
	class EndLine
	{
	private:
		float x;
	public:
		// Constructors
		EndLine(float xPosition = 0) : x(xPosition)
		{
		}
		// Accessors
		float getX() const
		{
			return x;
		}
	};

	template <class T, class V = std::string>
	class Repeat
	{
	private:
		unsigned int timesToRepeat;
		T value;
		V separator;
	public:
		// Constructors
		Repeat(const T & val, unsigned int count) : value(val), timesToRepeat(count), separator()
		{
		}
		Repeat(const T & val, unsigned int count, const V & sep) : value(val), timesToRepeat(count), separator(sep)
		{
		}
		// Accessors
		const T &    getValue        () const
		{
			return value;
		}
		unsigned int getTimesToRepeat() const
		{
			return timesToRepeat;
		}
		const V &    getSeparator    () const
		{
			return separator;
		}
	};

	template <class Container, class V = std::string>
	class ContainerOutputAdaptor
	{
	private:
		typename Container::iterator first;
		typename Container::iterator last;
		V                            separator;
	public:
		// Constructors
		ContainerOutputAdaptor(typename Container::iterator firstIter, typename Container::iterator lastIter) : first(firstIter), last(lastIter), separator()
		{
		}
		ContainerOutputAdaptor(typename Container::iterator firstIter, typename Container::iterator lastIter, const V & sep) : first(firstIter), last(lastIter), separator(sep)
		{
		}
		// Accessors
		const typename Container::iterator begin() const
		{
			return first;
		}
		const typename Container::iterator end() const
		{
			return last;
		}
		const V & getSeparator() const
		{
			return separator;
		}
		// Utilities
		std::size_t difference() const
		{
			return last - first;
		}
	};

	WindowOutputBuffer & operator << (WindowOutputBuffer & buffer, const EndLine & endl)
	{
		if (buffer.validRenderTarget())
		{
			buffer.move(-(buffer.getPosition().x - endl.getX()), buffer.getLineSpacing());
		}
		return buffer;
	}
	template <class T, class V>
	WindowOutputBuffer & operator << (WindowOutputBuffer & buffer, const Repeat<T, V> & repeat)
	{
		if (repeat.getTimesToRepeat())
		{
			for (unsigned int i = 0; i < repeat.getTimesToRepeat() - 1; ++i)
			{
				buffer << repeat.getValue() << repeat.getSeparator();
			}
			buffer << repeat.getValue();
		}
		return buffer;
	}
	template <class T, class V>
	WindowOutputBuffer & operator << (WindowOutputBuffer & buffer, const ContainerOutputAdaptor<T, V> & adaptor)
	{
		if (adaptor.difference())
		{
			auto iterator = adaptor.begin();
			for (unsigned int i = 0; i < adaptor.difference() - 1; ++i)
			{
				buffer << *iterator++ << adaptor.getSeparator();
			}
			buffer << *iterator;
		}
		return buffer;
	}
}