/**
 * @file Plugin.cpp
 * @brief The main file of the plugin
 */

#include <LoggerAPI.h>

/**
 * @brief The entrypoint of the plugin. DO NOT remove or rename this function.
 *        
 */
void PluginInit()
{
    // Your code here
    Logger logger;
    logger.info("Hello, world!");
}
