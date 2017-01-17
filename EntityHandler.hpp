#pragma once

#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/System/String.hpp>
#include <SFML/Graphics/Drawable.hpp>

#include "Entity.hpp"

#include <map>
#include <functional>
#include <memory>

// TODO: tests
// TODO: documentation

namespace sfext
{
	class EntityHandler : public sf::Drawable
	{
	private:
		std::map<sf::String, std::shared_ptr<Entity>> entities;
	public:
		// Constructors
		EntityHandler()
		{
		}
		// Destructor
		~EntityHandler()
		{
		}
		// Accessors
		// Mutators
		template <class EntityType>
		void addEntity(const sf::String & alias, const std::function<std::shared_ptr<EntityType>> & allocator)
		{
			entities[alias] = allocator();
		}
		template <class EntityType, class T>
		void addEntity(const sf::String & alias, const std::function<std::shared_ptr<EntityType> (const T &)> & allocator, const T & parameterOne)
		{
			entities[alias] = allocator(parameterOne);
		}
		template <class EntityType, class T, class U>
		void addEntity(const sf::String & alias, const std::function<std::shared_ptr<EntityType> (const T &, const U &)> & allocator, const T & parameterOne, const U & parameterTwo)
		{
			entities[alias] = allocator(parameterOne, parameterTwo);
		}
		template <class EntityType, class T, class U, class V>
		void addEntity(const sf::String & alias, const std::function<std::shared_ptr<EntityType> (const T &, const U &, const V &)> & allocator, const T & parameterOne, const U & parameterTwo, const V & parameterThree)
		{
			entities[alias] = allocator(parameterOne, parameterTwo, parameterThree);
		}
		void removeEntity(const sf::String & alias)
		{
			if (hasEntity(alias))
			{
				entities.erase(alias);
			}
		}
		// Utilities
		void         handleIntersections(Entity & entity) const
		{
			for (const auto & pair : entities)
			{
				entity.handleIntersection(*(pair.second));
			}
		}
		void         update             (sf::Time elapsed)
		{
			for (auto & pair : entities)
			{
				pair.second->update(elapsed);
			}
		}
		void         update             (const sf::String & alias, sf::Time elapsed)
		{
			if (hasEntity(alias))
			{
				entities.at(alias)->update(elapsed);
			}
		}
		virtual void draw               (sf::RenderTarget & target, sf::RenderStates states = sf::RenderStates::Default) const
		{
			for (const auto & pair : entities)
			{
				target.draw(*(pair.second), states);
			}
		}
		bool         hasEntity          (const sf::String & alias) const
		{
			return entities.find(alias) != entities.cend();
		}
		std::size_t  size               () const
		{
			return entities.size();
		}
	};
}