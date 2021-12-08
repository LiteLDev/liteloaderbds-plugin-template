#include "pch.h"
#include <EventAPI.h>
#include <LoggerAPI.h>
#include <MC/Player.hpp>
#include <MC/ItemStack.hpp>

bool PlayerAttack(Event::PlayerAttackEvent event)
{
	ItemStack *bread = ItemStack::create("minecraft:bread", 1);
	event.mPlayer->giveItem(bread);
	return true;
}

void PluginMain()
{
	Logger::setTitle("MyPlugin");
	Logger::Info("Hello world");
	Logger::Warn("My first BDS plugin!");

	Event::PlayerAttackEvent::subscribe(PlayerAttack);
}

void PluginInit()
{
	Event::ServerStartedEvent::subscribe([](Event::ServerStartedEvent ev) -> bool {
		PluginMain();
		return true;
	});
}