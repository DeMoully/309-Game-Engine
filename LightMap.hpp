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

// TODO: redesign how LightSources are stored and allocated. There has to be a better way to do this.

namespace sfext
{
	class LightMap : public sf::Drawable
	{
	private:
		sf::RenderTexture m_texture;
		mutable sf::RenderTexture m_buffer;
		std::map<sf::String, std::shared_ptr<LightSource>> m_lights;
		sf::Uint8 m_alpha;
		sf::VertexArray m_vertices;
	public:
		LightMap(unsigned int width, unsigned int height, sf::Uint8 alpha = sf::Uint8(0)) : m_alpha(alpha), m_vertices(sf::PrimitiveType::Quads, 4)
		{
			m_texture.create(width, height);
			m_buffer.create(width, height);
			sf::Vector2f dimensions = sf::Vector2f(m_texture.getSize());
			// create the vertices that will display the internal texture after it has had light applied to it
			m_vertices[0] = sf::Vertex(sf::Vector2f(0.f, 0.f), sf::Color(255, 255, 255, m_alpha), sf::Vector2f(0.f, 0.f));
			m_vertices[1] = sf::Vertex(sf::Vector2f(dimensions.x, 0.f), sf::Color(255, 255, 255, m_alpha), sf::Vector2f(dimensions.x, 0.f));
			m_vertices[2] = sf::Vertex(dimensions, sf::Color(255, 255, 255, m_alpha), dimensions);
			m_vertices[3] = sf::Vertex(sf::Vector2f(0.f, dimensions.y), sf::Color(255, 255, 255, m_alpha), sf::Vector2f(0.f, dimensions.y));
		}
		// Accessors
		const sf::View &    getView() const
		{
			// get the current view of the light map
			return m_texture.getView();
		}
		sf::Uint8           getAlpha() const
		{
			// get the minimum alpha map that the light map can display
			return m_alpha;
		}
		sf::Vector2f        getPosition(const sf::String & alias) const
		{
			// get the position of a light in the light map
			if (hasLightSource(alias))
				return m_lights.at(alias)->getPosition();
			else
				throw std::invalid_argument("The light <" + alias + "> does not exist.");
		}
		// Mutators
		template <class FunctionType, class ... Args>
		void addLightSource(const sf::String & alias, const FunctionType & allocator, const Args &... args)
		{
			// create a light from an allocation and (optional) parameters
			m_lights[alias] = allocator(args...);
		}
		void removeLightSource(const sf::String & alias)
		{
			// remove a light source from the light map
			if (hasLightSource(alias))
				m_lights.erase(m_lights.find(alias));
			else
				throw std::invalid_argument("The light <" + alias + "> does not exist.");
		}
		void setView(const sf::View & view)
		{
			// change the view of the light map
			m_texture.setView(view);
		}
		void setAlpha(sf::Uint8 minA)
		{
			// change the minimum alpha value that the light map can display
			m_alpha = minA;
			for (int i = 0; i < 3; ++i)
				m_vertices[i].color.a = m_alpha;
		}
		void setPosition(const sf::String & alias, const sf::Vector2f & position)
		{
			// change the position of a light
			if (hasLightSource(alias))
				m_lights.at(alias)->setPosition(position);
			else
				throw std::invalid_argument("The light <" + alias + "> does not exist.");
		}
		// Utilities
		std::size_t  size() const
		{
			// returns the number of lights held by the light map
			return m_lights.size();
		}
		void         clear(const sf::Color & color = sf::Color::Black)
		{
			// erase the contents of the internal texture
			m_texture.clear(color);
		}
		void         display()
		{
			// display the internal texture so that it can be properly drawn
			m_texture.display();
		}
		void         draw(const sf::Drawable & drawable, sf::RenderStates states = sf::RenderStates::Default)
		{
			// Draw a Drawable to the internal texture
			m_texture.draw(drawable, states);
		}
		void         draw(sf::RenderTarget & target, sf::RenderStates states = sf::RenderStates::Default) const
		{
			// Clear the buffer to which we will be drawing lights
			m_buffer.clear();
			// Fetch the texture of everything else that we've drawn
			states.texture = &m_texture.getTexture();
			// Draw the lights on top of everything else
			for (const auto & light : m_lights)
				light.second->draw(m_buffer, getView(), states);
			// Draw the vertices over the buffer
			m_buffer.draw(m_vertices, states);
			m_buffer.display();
			// Draw to the screen
			target.draw(sf::Sprite(m_buffer.getTexture()));
		}
		bool         hasLightSource(const sf::String & alias) const
		{
			return m_lights.find(alias) != m_lights.cend();
		}
		sf::Vector2f mapPixelToCoords(const sf::Vector2i & position) const
		{
			// Take a pixel from the screen and convert it to a point in the texture based on its view
			return m_texture.mapPixelToCoords(position);
		}
	};
}