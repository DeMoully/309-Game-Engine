#pragma once

#include <SFML/System/Vector2.hpp>

#include <cmath>
#include <random>
#include <chrono>

// TODO: tests

namespace sfext
{
	unsigned int seed          ()
	{
		// provides a seed for the random number generators based on the current system time
		return static_cast<unsigned int>(std::chrono::system_clock::now().time_since_epoch().count());
	}
	double       randomReal    (double lower = 0, double upper = 1)
	{
		// Returns a random real number in the range [lower, upper)
		static std::default_random_engine engine(seed());
		if (lower > upper)
			std::swap(lower, upper);
		return std::uniform_real_distribution<double>(lower, upper)(engine);
	}
	int          randomInt     (int lower = 0, int upper = 1)
	{
		// Returns a random integer in the range [lower, upper]
		static std::default_random_engine engine(seed());
		if (lower > upper)
			std::swap(lower, upper);
		return std::uniform_int_distribution<int>(lower, upper)(engine);
	}
	bool         randomBool    ()
	{
		// Returns a random bool
		return randomInt() ? true : false;
	}
	sf::Vector2f randomVector2f(float lowerX, float upperX, float lowerY, float upperY)
	{
		// Gives a rectangular distribution for an sf::Vector2f
		return sf::Vector2f(static_cast<float>(randomReal(lowerX, upperX)), static_cast<float>(randomReal(lowerY, upperY)));
	}
	sf::Vector2f randomVector2f(const sf::Vector2f & center, float radius)
	{
		// Gives a circular distribution for an sf::Vector2f
		float theta = static_cast<float>(randomReal(0, TWO_PI_F));
		return sf::Vector2f(center.x + radius * std::cos(theta), center.y + radius * std::sin(theta));
	}
	sf::Vector2f randomVector2f(const sf::Vector2f & center, float xRadius, float yRadius)
	{
		// Gives an elliptical distribution for an sf::Vector2f
		float theta = static_cast<float>(randomReal(0, TWO_PI_F));
		return sf::Vector2f(center.x + xRadius * std::cos(theta), center.y + yRadius * std::sin(theta));
	}
	sf::Vector2f randomVector2fWithinCircle(const sf::Vector2f & center, float radius)
	{
		// Gives a full circular distribution 
		return randomVector2f(center, static_cast<float>(randomReal(0, static_cast<double>(radius))));
	}
	sf::Color    randomColor   (sf::Uint8 alpha = 255)
	{
		// Gives a random color with a specific alpha value
		return sf::Color(randomInt(0, 255), randomInt(0, 255), randomInt(0, 255), alpha);
	}
	sf::Color    randomColor   (sf::Uint8 minR, sf::Uint8 maxR, sf::Uint8 minG, sf::Uint8 maxG, sf::Uint8 minB, sf::Uint8 maxB, sf::Uint8 minA, sf::Uint8 maxA)
	{
		// Gives a random color with more options for RGBA channels
		return sf::Color(randomInt(minR, maxR), randomInt(minG, maxG), randomInt(minB, maxB), randomInt(minA, maxA));
	}
}