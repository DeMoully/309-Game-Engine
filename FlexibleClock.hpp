#pragma once

#include <SFML/System/Clock.hpp>
#include <SFML/System/Time.hpp>

// TODO: tests
// TODO: documentation

namespace sfext
{
	class FlexibleClock final
	{
	private:
		sf::Clock m_clock;
		sf::Time  m_time;
		float     m_modifier;
		bool      m_paused;
	public:
		// Constructors
		FlexibleClock         () : m_time(sf::Time::Zero), m_modifier(1.f), m_paused(false)
		{
		}
		explicit FlexibleClock(bool paused) : m_time(sf::Time::Zero), m_paused(paused), m_modifier(1.f)
		{
		}
		// Accessors
		sf::Time getElapsedTime() const
		{
			return m_time + (m_paused ? sf::Time::Zero : (m_clock.getElapsedTime() * m_modifier));
		}
		float    getModifier   () const
		{
			return m_modifier;
		}
		bool     isPaused      () const
		{
			return m_paused;
		}
		// Mutators
		void setModifier   (float modifier)
		{
			m_modifier = modifier;
		}
		void scaleModifier (float scale)
		{
			m_modifier *= scale;
		}
		void setCurrentTime(sf::Time newTime)
		{
			m_time = newTime;
			m_clock.restart();
		}
		// Utilities
		void     pause  ()
		{
			if (!m_paused)
			{
				m_time += m_clock.getElapsedTime() * m_modifier;
				m_paused = true;
			}
		}
		void     start  ()
		{
			if (m_paused)
			{
				m_clock.restart();
				m_paused = false;
			}
		}
		sf::Time restart(bool paused = false)
		{
			sf::Time returnTime = getElapsedTime();
			m_paused = paused;
			m_time = sf::Time::Zero;
			m_clock.restart();
			return returnTime;
		}
	};
}