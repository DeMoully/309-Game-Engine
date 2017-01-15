#pragma once

#include <cmath>

#include "VectorMath.hpp"
#include "Random.hpp"
#include "FlexibleClock.hpp"

namespace sfext
{
	enum class TweenerStyle
	{
		Constant,
		Linear,
		Quadratic,
		Exponential,
		Sinusoidal,
		Circular,
		Random
	};

	enum class TweenerPeriod
	{
		Single,
		BackAndForth,
		WrapAround
	};

	class Tweener final
	{
	private:
		FlexibleClock clock;
		float start;
		float finish;
		TweenerStyle style;
		TweenerPeriod period;
		sf::Time duration;
	public:
		// Constructors
		Tweener() : clock(false), start(0.f), finish(0.f), style(TweenerStyle::Linear), period(TweenerPeriod::Single), duration(sf::Time::Zero)
		{
		}
		Tweener(float startVal, float finishVal, sf::Time dur, TweenerStyle tweenerStyle = TweenerStyle::Linear, TweenerPeriod tweenerPeriod = TweenerPeriod::Single) : clock(false), start(startVal), finish(finishVal), style(tweenerStyle), period(tweenerPeriod), duration(dur)
		{
		}
		// Destructor
		~Tweener()
		{
		}
		// Mutators
		void setStyle      (TweenerStyle tweenerStyle)
		{
			style = tweenerStyle;
		}
		void setPeriod     (TweenerPeriod tweenerPeriod)
		{
			period = tweenerPeriod;
		}
		void setDuration   (sf::Time dur)
		{
			duration = dur;
		}
		void setCurrentTime(sf::Time time)
		{
			clock.setCurrentTime(time);
		}
		void setTimeScale  (float scale)
		{
			clock.setModifier(scale);
		}
		// Accessors
		TweenerStyle  getStyle      () const
		{
			return style;
		}
		TweenerPeriod getPeriod     () const
		{
			return period;
		}
		sf::Time      getDuration   () const
		{
			return duration;
		}
		sf::Time      getElapsedTime() const
		{
			return clock.getElapsedTime();
		}
		float         getTimeScale  () const
		{
			return clock.getModifier();
		}
		// Utilities
		float  get    () const
		{
			switch (style)
			{
			case TweenerStyle::Constant:
				{
					return start;
				}
			case TweenerStyle::Linear:
				{
					switch (period)
					{
					case TweenerPeriod::Single:
						{
							if (clock.getElapsedTime() <= sf::Time::Zero)
							{
								return start;
							}
							else if (clock.getElapsedTime() >= duration)
							{
								return finish;
							}
							else
							{
								return start + (finish - start) * (clock.getElapsedTime().asSeconds() / duration.asSeconds());
							}
						}
					case TweenerPeriod::BackAndForth:
						{
							sf::Time time = sf::seconds(std::fmodf(clock.getElapsedTime().asSeconds(), 2.f * duration.asSeconds()));
							if (time >= duration)
							{
								time -= duration;
								return finish - (finish - start) * (time.asSeconds() / duration.asSeconds());
							}
							else
							{
								return start + (finish - start) * (time.asSeconds() / duration.asSeconds());
							}
						}
					case TweenerPeriod::WrapAround:
						{
							sf::Time time = sf::seconds(std::fmodf(clock.getElapsedTime().asSeconds(), duration.asSeconds()));
							return start + (finish - start) * (time.asSeconds() / duration.asSeconds());
						}
					default:
						{
							return start;
						}
					}
				}
			case TweenerStyle::Quadratic:
				{
					switch (period)
					{
					case TweenerPeriod::Single:
						{
							if (clock.getElapsedTime() <= sf::Time::Zero)
							{
								return start;
							}
							else if (clock.getElapsedTime() >= duration)
							{
								return finish;
							}
							else
							{
								return start + (finish - start) * std::powf((clock.getElapsedTime().asSeconds() / duration.asSeconds()), 2.f);
							}
						}
					case TweenerPeriod::BackAndForth:
						{
							sf::Time time = sf::seconds(std::fmodf(clock.getElapsedTime().asSeconds(), 2.f * duration.asSeconds()));
							if (time >= duration)
							{
								time -= duration;
								return start + (finish - start) * std::powf(1.f - (time.asSeconds() / duration.asSeconds()), 2.f);
							}
							else
							{
								return start + (finish - start) * std::powf((time.asSeconds() / duration.asSeconds()), 2.f);
							}
						}
					case TweenerPeriod::WrapAround:
						{
							sf::Time time = sf::seconds(std::fmodf(clock.getElapsedTime().asSeconds(), duration.asSeconds()));
							return start + (finish - start) * std::powf((time.asSeconds() / duration.asSeconds()), 2.f);
						}
					default:
						{
							return start;
						}
					}
				}
			case TweenerStyle::Sinusoidal:
				{
					switch (period)
					{
					case TweenerPeriod::Single:
						{
							if (clock.getElapsedTime() <= sf::Time::Zero)
							{
								return start;
							}
							else if (clock.getElapsedTime() >= duration)
							{
								return finish;
							}
							else
							{
								return start + (finish - start) * .5f + (finish - start) * -.5f * std::cosf(3.141592f * (clock.getElapsedTime().asSeconds() / duration.asSeconds()));
							}
						}
					case TweenerPeriod::BackAndForth:
						{
							return start + (finish - start) * .5f + (finish - start) * -.5f * std::cosf(3.141592f * (clock.getElapsedTime().asSeconds() / duration.asSeconds()));
						}
					case TweenerPeriod::WrapAround:
						{
							sf::Time time = sf::seconds(std::fmodf(clock.getElapsedTime().asSeconds(), duration.asSeconds()));
							return start + (finish - start) * .5f + (finish - start) * -.5f * std::cosf(3.141592f * (time.asSeconds() / duration.asSeconds()));
						}
					default:
						{
							return start;
						}
					}
				}
			case TweenerStyle::Circular:
				{
					switch (period)
					{
					case TweenerPeriod::Single:
						{
							if (clock.getElapsedTime() <= sf::Time::Zero)
							{
								return start;
							}
							else if (clock.getElapsedTime() >= duration)
							{
								return start;
							}
							else
							{
								return start + (finish - start) * std::sqrtf(1.f - std::powf(1.f - (2.f * clock.getElapsedTime().asSeconds() / duration.asSeconds()), 2.f));
							}
						}
					case TweenerPeriod::BackAndForth:
					case TweenerPeriod::WrapAround:
						{
							sf::Time time = sf::seconds(fmodf(clock.getElapsedTime().asSeconds(), duration.asSeconds()));
							return start + (finish - start) * std::sqrtf(1.f - std::powf(1.f - (2.f * time.asSeconds() / duration.asSeconds()), 2.f));
						}
					}
				}
			case TweenerStyle::Random:
				{
					return static_cast<float>(randomReal(start, finish));
				}
			default:
				{
					return start;
				}
			}
		}
		void   reset  ()
		{
			clock.restart(false);
		}
	};
}