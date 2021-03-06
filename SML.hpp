#pragma once

#include <map>
#include <list>
#include <iostream>
#include <functional>
#include <set>

#include "FileWrapper.hpp"
#include "FormattingFunctions.hpp"
#include "ContainerAlgorithms.hpp"

#include <SFML/System/String.hpp>

// TODO: tests
// TODO: documentation

namespace sfext
{
	typedef std::map<sf::String, std::map<sf::String, sf::String>> SMLValues;

	class SML final
	{
	private:
		SMLValues values;
		FileWrapper file;
	public:
		// Constructors
		SML() : file()
		{
		}
		SML(const sf::String & fileName) : file(fileName, FileCloseAction::NONE)
		{
			parseValues();
		}
		// Destructor
		~SML()
		{
		}
		// Accessors
		sf::String             getValue          (const sf::String & variable, const sf::String & tag) const
		{
			return hasTag(variable, tag) ? values.at(variable).at(tag) : "";
		}
		std::set<sf::String>   getValueNames     () const
		{
			return getFirstValues(values);
		}
		std::set<sf::String>   getTags           (const sf::String & variable) const
		{
			if (hasVariable(variable))
			{
				return getFirstValues(values.at(variable));
			}
			return std::set<sf::String>();
		}
		template <class T>
		T                      interpretAsNumber (const sf::String & variable, const sf::String & tag) const
		{
			return static_cast<T>(std::strtod(getValue(variable, tag).toAnsiString().c_str(), nullptr));
		}
		template <class T>
		std::list<T>           interpretAsList   (const sf::String & variable, const sf::String & tag) const
		{
			// This is intended for lists of numbers only
			std::list<std::string> split = splitString(getValue(variable, tag));
			std::list<T> list;
			for (const std::string & str : split)
			{
				list.push_back(static_cast<T>(strtod(str.c_str(), nullptr)));
			}
			return list;
		}
		std::list<std::string> interpretAsList   (const sf::String & variable, const sf::String & tag) const
		{
			return splitString(getValue(variable, tag));
		}
		template <class T>
		sf::Vector2<T>         interpretAsVector2(const sf::String & variable, const sf::String & tag) const
		{
			std::list<T> list = interpretAsList<T>(variable, tag);
			sf::Vector2<T> result;
			list.resize(2, T());
			auto iterator = list.cbegin();
			result.x = *iterator++;
			result.y = *iterator;
			return result;
		}
		template <class T>
		sf::Vector3<T>         interpretAsVector3(const sf::String & variable, const sf::String & tag) const
		{
			std::list<T> list = interpretAsList<T>(variable, tag);
			sf::Vector3<T> result;
			list.resize(3, T());
			auto iterator = list.cbegin();
			result.x = *iterator++;
			result.y = *iterator++;
			result.z = *iterator;
			return result;
		}
		sf::Color              interpretAsColor  (const sf::String & variable, const sf::String & tag) const
		{
			std::list<sf::Uint8> list = interpretAsList<sf::Uint8>(variable, tag);
			sf::Color result;
			list.resize(4, 255);
			auto iterator = list.cbegin();
			result.r = *iterator++;
			result.g = *iterator++;
			result.b = *iterator++;
			result.a = *iterator;
			return result;
		}
		// Mutators
		void setTargetFile (const sf::String & filePath)
		{
			file.setFilename(filePath);
		}
		void addVariable   (const sf::String & variable)
		{
			if (!hasVariable(variable)) // We want to make sure that the variable doesn't already exist
			{
				values[variable] = std::map<sf::String, sf::String>();
			}
		}
		template <class T>
		void addTag        (const sf::String & variable, const sf::String & tag, const T & value)
		{
			if (hasVariable(variable))
			{
				values.at(variable)[tag] = std::to_string(value);
			}
		}
		template <class T>
		void addTag        (const sf::String & variable, const sf::String & tag, const sf::Vector2<T> & value)
		{
			if (hasVariable(variable))
			{
				values.at(variable)[tag] = std::to_string(value.x) + ", " + std::to_string(value.y);
			}
		}
		template <class T>
		void addTag        (const sf::String & variable, const sf::String & tag, const sf::Vector3<T> & value)
		{
			if (hasVariable(variable))
			{
				values.at(variable)[tag] = std::to_string(value.x) + ", " + std::to_string(value.y) + ", " + std::to_string(value.z);
			}
		}
		void addTag        (const sf::String & variable, const sf::String & tag, const sf::Color & value)
		{
			if (hasVariable(variable))
			{
				values.at(variable)[tag] = std::to_string(static_cast<int>(value.r)) + ", " + std::to_string(static_cast<int>(value.g)) + ", " + std::to_string(static_cast<int>(value.b)) + ", " + std::to_string(static_cast<int>(value.a));
			}
		}
		void addTag        (const sf::String & variable, const sf::String & tag, const sf::String & value)
		{
			if (hasVariable(variable))
			{
				values.at(variable)[tag] = value;
			}
		}
		void removeVariable(const sf::String & variable)
		{
			if (hasVariable(variable))
			{
				values.erase(values.find(variable));
			}
		}
		void removeTag     (const sf::String & variable, const sf::String & tag)
		{
			if (hasVariable(variable) && hasTag(variable, tag))
			{
				values.at(variable).erase(values.at(variable).find(tag));
			}
		}
		// Utilities
		void parseValues()
		{
			file.clearContentsIf(lengthIs, 0); // Get rid of empty lines
			file.applyFunctionToContents(removeLeadingSpaces); // Get rid of all spaces
			file.applyFunctionToContents(removeTrailingSpaces); // Get rid of all spaces
			file.applyFunctionToContents([](std::string str) // Put all lines in the format that we want
			{
				std::string result;
				auto iterator = str.begin();
				while (iterator != str.end() && *iterator != ':') // Keep the variable name
				{
					result += *iterator++;
				}
				if (iterator != str.end()) // Move up by one if the line still has more values
				{
					result += *iterator++;
				}
				while (iterator != str.end() && (*iterator) == ' ') // Get rid of all spaces before the value
				{
					++iterator;
				}
				while (iterator != str.end()) // Take the rest and keep as the value
				{
					result += *iterator++;
				}
				return result;
			});
			FileWrapper::ConstIterator iterator = file.cbegin();
			while (iterator != file.cend())
			{
				if (std::string(*iterator).find("var_begin:") != std::string::npos) // Start a new variable
				{
					std::string varName = std::string(iterator->cbegin() + 10, iterator->cend());
					++iterator;
					while (iterator != file.cend() && *iterator != "var_end") // Go until the end of the declaration is reached
					{
						std::string::size_type colonPos = iterator->find_first_of(':');
						if (colonPos != std::string::npos)
						{
							std::string tag(iterator->cbegin(), iterator->cbegin() + colonPos);
							std::string value(iterator->cbegin() + colonPos + 1, iterator->cend());
							values[varName][tag] = value;
						}
						++iterator;
					}
				}
				++iterator;
			}
		}
		bool hasVariable(const sf::String & variable) const
		{
			return values.find(variable) != values.cend();
		}
		bool hasTag     (const sf::String & variable, const sf::String & tag) const
		{
			return hasVariable(variable) && values.at(variable).find(tag) != values.at(variable).cend();
		}
		void updateFile ()
		{
			file.clearContents();
			for (auto variable : values) // variable = <name, tag(s)>
			{
				file.appendLine("var_begin:" + variable.first);
				for (auto tag : variable.second) // tag = <name, value>
				{
					file.appendLine("    " + tag.first + ":" + tag.second);
				}
				file.appendLine("var_end");
			}
			file.outputToFile();
		}
	};
}