#pragma once

#include <chrono>

#include "TextureHandler.hpp"
#include "SpriteHandler.hpp"
#include "AnimationHandler.hpp"
#include "FontHandler.hpp"
#include "FileWrapper.hpp"

// The Global Resource Cache provides a way to access resources such as textures, fonts, sprites, and animations.
// It also provides global access to a debugging tool.

// The resources that are stored are persistent throughout the lifetime of the application, only being deleted when
// either the application closes or the program explicitly requests that they be deallocated.

// The primary benefit of storing resources in a globally accessible but strongly controlled container is that
// commonly accessed resources are available throughout the application and do not need to be passed around to
// functions or objects.

// The resource handlers contained within can be accessed via a reference to the container and work in exactly the same
// manner as their normal counterparts. Resources can still be created, destroyed, copied, etc. just as they would be
// in any other case.

// The debugging tool automatically writes to the debugging file when the program cleanly finishes executing.
// Premature closing of the program in which objects are not properly cleaned up may result in a case where the
// debugging information is not output to the file. Period flushes of the debugger can help to mitigate this.


// TODO: add resource caches for audio once they have been created.
// TODO: write tests to ensure that everything is working properly.
// TODO: document resource caches that have been written.
// TODO: write tests for resource caches that have been written.
// TODO: incorporate the GRC into the main test program to show how it is used.

namespace sfext
{
	class GlobalResourceCache
	{
	private:
		static TextureHandler m_texture_handler;
		static FontHandler m_font_handler;
		static AnimationHandler m_animation_handler;
		static SpriteHandler m_sprite_handler;
		static FileWrapper m_debugger;
	public:
		// Deletion of constructor to adhere to the Singleton pattern - we don't want anyone instantiating this (that would be silly)
		GlobalResourceCache() = delete;
		// Resource Cache Access
		static TextureHandler & getTextureHandler()
		{
			// Returns a reference to the global TextureHandler instance.
			// For more information on the implementation and usage of the
			// TextureHandler, see TextureHandler.hpp
			return m_texture_handler;
		}
		static FontHandler & getFontHandler()
		{
			// Returns a reference to the global FontHandler instance.
			// For more information on the implementation and usage of the
			// FontHandler, see FontHandler.hpp
			return m_font_handler;
		}
		static AnimationHandler & getAnimationHandler()
		{
			// Returns a reference to the global AnimationHandler instance.
			// For more information on the implementation and usage of the
			// AnimationHandler, see AnimationHandler.hpp
			return m_animation_handler;
		}
		static SpriteHandler & getSpriteHandler()
		{
			// Returns a reference to the global SpriteHandler instance.
			// For more information on the implementation and usage of the
			// SpriteHandler, see SpriteHandler.hpp
			return m_sprite_handler;
		}
		// Debugging tools
		static void setDebugPath(const std::string & filepath)
		{
			// Overwrites the current debugger filepath. Information is not written
			// when this is called.
			m_debugger.setFilename(filepath);
		}
		static void writeToDebugger(const std::string & report)
		{
			// Queues up debugging information to be written the debugging file
			// Writes the current system time followed by the bug report
			std::time_t time = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
			m_debugger.appendLine(std::string(std::ctime(&time)) + "\t" + report);
		}
		static void flushDebugger()
		{
			// Force the debugger to prematurely write to the debugging file
			// Removes the debugging information that was queued for output
			m_debugger.appendToFile();
			m_debugger.clearContents();
		}
		static void flushDebugger(const std::string & filepath)
		{
			// Force the debugger to prematurely write to the specified debugging file
			// Removes the debugging information that was queued for output
			m_debugger.appendToFile(filepath);
			m_debugger.clearContents();
		}
	};

	TextureHandler GlobalResourceCache::m_texture_handler = TextureHandler();
	FontHandler GlobalResourceCache::m_font_handler = FontHandler();
	AnimationHandler GlobalResourceCache::m_animation_handler = AnimationHandler();
	SpriteHandler GlobalResourceCache::m_sprite_handler = SpriteHandler();
	FileWrapper GlobalResourceCache::m_debugger = FileWrapper(FileCloseAction::APPEND);

	// It's a pain to write GlobalResourceCache every time you want to use it.
	using GRC = GlobalResourceCache;
}