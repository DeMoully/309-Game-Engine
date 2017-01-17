#pragma once

#include <map>
#include <memory>
#include <functional>

#include <SFML/Graphics/RenderTexture.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/System/String.hpp>

#include "VectorMath.hpp"
#include "Tweener.hpp"
#include "LightSource.hpp"

// TODO: rewrite this in its entirety. Inherit from RenderTexture and Drawable so that things can more easily be drawn to this.
// TODO: write documentation. There isn't any at the moment and very little of this is clear just from looking at it.
// TODO: redesign how LightSources are stored and allocated. There has to be a better way to do this.

namespace sfext
{
	class LightMap : public sf::Drawable
	{
	private:
		mutable sf::RenderTexture texture;
		mutable sf::RenderTexture lightBuffer;
		std::map<sf::String, std::shared_ptr<LightSource>> lights;
		sf::Uint8 minAlpha;
	public:
		LightMap(unsigned int width, unsigned int height, sf::Uint8 minA = sf::Uint8(0)) : minAlpha(minA)
		{
			texture.create(width, height);
			lightBuffer.create(width, height);
		}
		// Accessors
		sf::RenderTexture & getRenderTexture()
		{
			return texture;
		}
		const sf::View &    getView         () const
		{
			return texture.getView();
		}
		sf::Uint8           getMinimumAlpha () const
		{
			return minAlpha;
		}
		sf::Vector2f        getPosition     (const sf::String & alias) const
		{
			if (hasLightSource(alias))
			{
				return lights.at(alias)->getPosition();
			}
			return sf::Vector2f();
		}
		// Mutators
		template <class LightSourceType>
		void addLightSource(const sf::String & alias, const std::function<std::shared_ptr<LightSourceType>> & allocator)
		{
			lights[alias] = allocator();
		}
		template <class LightSourceType, class T>
		void addLightSource(const sf::String & alias, const std::function<std::shared_ptr<LightSourceType> (const T &)> & allocator, const T & parameterOne)
		{
			lights[alias] = allocator(parameterOne);
		}
		template <class LightSourceType, class T, class U>
		void addLightSource(const sf::String & alias, const std::function<std::shared_ptr<LightSourceType> (const T &, const U &)> & allocator, const T & parameterOne, const U & parameterTwo)
		{
			lights[alias] = allocator(parameterOne, parameterTwo);
		}
		template <class LightSourceType, class T, class U, class V>
		void addLightSource(const sf::String & alias, const std::function<std::shared_ptr<LightSourceType> (const T &, const U &, const V &)> & allocator, const T & parameterOne, const U & parameterTwo, const V & parameterThree)
		{
			lights[alias] = allocator(parameterOne, parameterTwo, parameterThree);
		}
		void removeLightSource(const sf::String & alias)
		{
			if (hasLightSource(alias))
			{
				lights.erase(lights.find(alias));
			}
		}
		void setView          (const sf::View & view)
		{
			texture.setView(view);
		}
		void setMinimumAlpha  (sf::Uint8 minA)
		{
			minAlpha = minA;
		}
		void setPosition      (const sf::String & alias, const sf::Vector2f & position)
		{
			if (hasLightSource(alias))
			{
				lights.at(alias)->setPosition(position);
			}
		}
		// Utilities
		std::size_t  size            () const
		{
			return lights.size();
		}
		void         clear           (const sf::Color & color = sf::Color::Black)
		{
			texture.clear(color);
		}
		void         draw            (const sf::Drawable & drawable, sf::RenderStates states = sf::RenderStates::Default) const
		{
			texture.draw(drawable, states);
		}
		void         draw            (sf::RenderTarget & target, sf::RenderStates states = sf::RenderStates::Default) const
		{
			lightBuffer.clear();
			texture.display();
			states.texture = &texture.getTexture();
			for (const auto & light : lights)
			{
				light.second->draw(lightBuffer, getView(), states);
			}
			sf::Vector2f dimensions = sf::Vector2f(texture.getSize());
			sf::VertexArray vertices(sf::Quads, 4);
			vertices[0] = sf::Vertex(sf::Vector2f(0.f, 0.f), sf::Color(255, 255, 255, minAlpha), sf::Vector2f(0.f, 0.f));
			vertices[1] = sf::Vertex(sf::Vector2f(dimensions.x, 0.f), sf::Color(255, 255, 255, minAlpha), sf::Vector2f(dimensions.x, 0.f));
			vertices[2] = sf::Vertex(dimensions, sf::Color(255, 255, 255, minAlpha), dimensions);
			vertices[3] = sf::Vertex(sf::Vector2f(0.f, dimensions.y), sf::Color(255, 255, 255, minAlpha), sf::Vector2f(0.f, dimensions.y));
			lightBuffer.draw(vertices, states);
			lightBuffer.display();
			target.draw(sf::Sprite(lightBuffer.getTexture()));
		}
		bool         hasLightSource  (const sf::String & alias) const
		{
			return lights.find(alias) != lights.cend();
		}
		sf::Vector2f mapPixelToCoords(const sf::Vector2i & position) const
		{
			return texture.mapPixelToCoords(position);
		}
	};
}