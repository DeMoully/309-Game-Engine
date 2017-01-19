#pragma once

#include <SFML/Window/Keyboard.hpp>

#include <map>
#include <functional>

// TODO: tests

// The KeyboardHandler class provides a way of mapping keyboard inputs
// to callback functions. Each key that has a mapping can be checked
// with a single function call.
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
			// Returns the function that is associated with the key value
			if (hasFunctionBoundToKey(key))
				return functions.at(key);
			else
				throw std::invalid_argument("The key with index <" + std::to_string(key) + "> has no bound function.");
		}
		// Mutators
		void addFunction   (sf::Keyboard::Key key, const std::function<void ()> & function)
		{
			// Binds a callback function to the specified key
			functions[key] = function;
		}
		void removeFunction(sf::Keyboard::Key key)
		{
			// Removes a key-binding if it exists
			auto iterator = functions.find(key);
			if (iterator != functions.cend())
				functions.erase(iterator);
		}
		// Utilities
		bool hasFunctionBoundToKey(sf::Keyboard::Key key) const
		{
			// Returns true if the key has a callback function bound to it
			// Else, returns false
			return functions.find(key) != functions.cend();
		}
		void checkForInput        () const
		{
			// Handles all bound keys if they are pressed
			for (const auto & pair : functions)
				if (sf::Keyboard::isKeyPressed(pair.first))
					pair.second();
		}
	};
}