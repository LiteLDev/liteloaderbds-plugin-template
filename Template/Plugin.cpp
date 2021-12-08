#include "pch.h"
#include <EventAPI.h>
#include <LoggerAPI.h>

void PluginMain()
{
	Logger::setTitle("MyPlugin");
	Logger::Info("Hello world");
	Logger::Warn("My first BDS plugin!");
}

void PluginInit()
{
	Event::ServerStartedEvent::subscribe([](Event::ServerStartedEvent ev) -> bool {
		PluginMain();
		return true;
	});
}