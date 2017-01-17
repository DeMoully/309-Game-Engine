#pragma once

#include <SFML/Window/Keyboard.hpp>

#include <map>
#include <functional>

// TODO: tests
// TODO: documentation

namespace sfext
{
	class KeyboardHandler
	{
	private:
		std::map<sf::Keyboard::Key, std::function<void ()>> functions;
	public:
		// Accessors
		std::function<void ()> getFunction(sf::Keyboard::Key key) const
		{
			if (hasFunctionBoundToKey(key))
				return functions.at(key);
			else
				throw std::invalid_argument("The key with index <" + std::to_string(key) + "> has no bound function.");
		}
		// Mutators
		void addFunction   (sf::Keyboard::Key key, const std::function<void ()> & function)
		{
			functions[key] = function;
		}
		void removeFunction(sf::Keyboard::Key key)
		{
			auto iterator = functions.find(key);
			if (iterator != functions.cend())
			{
				functions.erase(iterator);
			}
		}
		// Utilities
		bool hasFunctionBoundToKey(sf::Keyboard::Key key) const
		{
			return functions.find(key) != functions.cend();
		}
		void checkForInput        () const
		{
			for (const auto & pair : functions)
				if (sf::Keyboard::isKeyPressed(pair.first))
					pair.second();
		}
	};
}