#include "pch.h"
#include <EventAPI.h>
#include <LoggerAPI.h>
#include <MC/Level.hpp>
#include <MC/BlockInstance.hpp>
#include <MC/Block.hpp>
#include <MC/BlockSource.hpp>
#include <MC/Actor.hpp>
#include <MC/Player.hpp>
#include <MC/ItemStack.hpp>
#include <LLAPI.h>

static_assert(false, "Please set your plugin's name below and delete this line");
Logger logger("PluginName");

void PluginInit()
{
	static_assert(false, "Please modify your plugin registration details and delete this line");
	LL::registerPlugin("PluginName", "Introduction", LL::Version(1, 0, 0));

}