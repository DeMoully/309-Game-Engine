#pragma once

#include <memory>

#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/VertexArray.hpp>

#include "VectorMath.hpp"
#include "Tweener.hpp"
#include "LightSource.hpp"

// TODO: tests
// TODO: documentation

namespace sfext
{
	class Spotlight : public LightSource
	{
	private:
		std::size_t  levelOfDetail;
		Tweener      radius;
		sf::Color    centerColor;
		sf::Color    outerColor;
	public:
		// Constructors
		Spotlight(const sf::Vector2f & pos, const Tweener & radiusTweener, const sf::Color & centerCol = sf::Color::White, const sf::Color & outerCol = sf::Color::White, std::size_t LOD = 30U) : LightSource(sf::PrimitiveType::TrianglesFan, std::max(4U, LOD), pos), radius(radiusTweener), centerColor(centerCol), outerColor(outerCol), levelOfDetail(std::max(4U, LOD))
		{
		}
		// Accessors
		sf::Color    getCenterColor  () const
		{
			return centerColor;
		}
		sf::Color    getOuterColor   () const
		{
			return outerColor;
		}
		Tweener getRadiusTweener() const
		{
			return radius;
		}
		float        getRadius       () const
		{
			return radius.get();
		}
		std::size_t  getLevelOfDetail() const
		{
			return levelOfDetail;
		}
		// Mutators
		void setRadiusTweener(const Tweener & tweener)
		{
			radius = tweener;
		}
		void setCenterColor  (const sf::Color & col)
		{
			centerColor = col;
		}
		void setOuterColor   (const sf::Color & col)
		{
			outerColor = col;
		}
		void setLevelOfDetail(std::size_t LOD)
		{
			if (LOD >= 4)
			{
				levelOfDetail = LOD;
			}
		}
		// Utilities
		void updateVertices() const
		{
			const float twoPi = 2.f * 3.14159265358979f;
			float rad = radius.get();
			if (vertices.getVertexCount() != levelOfDetail)
			{
				vertices.resize(levelOfDetail);
			}
			vertices[0] = sf::Vertex(position, centerColor, position);
			for (unsigned int i = 1; i < levelOfDetail; ++i)
			{
				float theta = twoPi * i / (levelOfDetail - 2);
				sf::Vector2f pos = position + rad * unitVector(theta);
				vertices[i] = sf::Vertex(pos, outerColor, pos);
			}
		}
	};
}