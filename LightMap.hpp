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

// TODO: rewrite this in its entirety.
// TODO: write documentation. There isn't any at the moment and very little of this is clear just from looking at it.
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
	public:
		LightMap(unsigned int width, unsigned int height, sf::Uint8 alpha = sf::Uint8(0)) : m_alpha(alpha)
		{
			m_texture.create(width, height);
			m_buffer.create(width, height);
		}
		// Accessors
		const sf::View &    getView() const
		{
			return m_texture.getView();
		}
		sf::Uint8           getAlpha() const
		{
			return m_alpha;
		}
		sf::Vector2f        getPosition(const sf::String & alias) const
		{
			if (hasLightSource(alias))
				return m_lights.at(alias)->getPosition();
			else
				throw std::invalid_argument("The light <" + alias + "> does not exist.");
		}
		// Mutators
		template <class FunctionType, class ... Args>
		void addLightSource(const sf::String & alias, const FunctionType & allocator, const Args &... args)
		{
			m_lights[alias] = allocator(args...);
		}
		void removeLightSource(const sf::String & alias)
		{
			if (hasLightSource(alias))
				m_lights.erase(m_lights.find(alias));
			else
				throw std::invalid_argument("The light <" + alias + "> does not exist.");
		}
		void setView(const sf::View & view)
		{
			m_texture.setView(view);
		}
		void setAlpha(sf::Uint8 minA)
		{
			m_alpha = minA;
		}
		void setPosition(const sf::String & alias, const sf::Vector2f & position)
		{
			if (hasLightSource(alias))
				m_lights.at(alias)->setPosition(position);
			else
				throw std::invalid_argument("The light <" + alias + "> does not exist.");
		}
		// Utilities
		std::size_t  size() const
		{
			return m_lights.size();
		}
		void         clear(const sf::Color & color = sf::Color::Black)
		{
			m_texture.clear(color);
		}
		void         display()
		{
			m_texture.display();
		}
		void         draw(const sf::Drawable & drawable, sf::RenderStates states = sf::RenderStates::Default)
		{
			m_texture.draw(drawable, states);
		}
		void         draw(sf::RenderTarget & target, sf::RenderStates states = sf::RenderStates::Default) const
		{
			m_buffer.clear();

			states.texture = &m_texture.getTexture();

			for (const auto & light : m_lights)
				light.second->draw(m_buffer, getView(), states);

			sf::Vector2f dimensions = sf::Vector2f(m_texture.getSize());

			sf::VertexArray vertices(sf::Quads, 4);
			vertices[0] = sf::Vertex(sf::Vector2f(0.f, 0.f), sf::Color(255, 255, 255, m_alpha), sf::Vector2f(0.f, 0.f));
			vertices[1] = sf::Vertex(sf::Vector2f(dimensions.x, 0.f), sf::Color(255, 255, 255, m_alpha), sf::Vector2f(dimensions.x, 0.f));
			vertices[2] = sf::Vertex(dimensions, sf::Color(255, 255, 255, m_alpha), dimensions);
			vertices[3] = sf::Vertex(sf::Vector2f(0.f, dimensions.y), sf::Color(255, 255, 255, m_alpha), sf::Vector2f(0.f, dimensions.y));

			m_buffer.draw(vertices, states);
			m_buffer.display();

			target.draw(sf::Sprite(m_buffer.getTexture()));
		}
		bool         hasLightSource(const sf::String & alias) const
		{
			return m_lights.find(alias) != m_lights.cend();
		}
		sf::Vector2f mapPixelToCoords(const sf::Vector2i & position) const
		{
			return m_texture.mapPixelToCoords(position);
		}
	};
}