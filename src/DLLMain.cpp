/**
 * @file DLLMain.cpp
 * @note DO NOT modify or remove this file!
 */

#include <LoggerAPI.h>
#include <ServerAPI.h>

#include "Version.h"

#pragma comment(lib, "../SDK/Lib/bedrock_server_api.lib")
#pragma comment(lib, "../SDK/Lib/bedrock_server_var.lib")
#pragma comment(lib, "../SDK/Lib/SymDBHelper.lib")
#pragma comment(lib, "../SDK/Lib/LiteLoader.lib")

void PluginInit();

void CheckProtocolVersion()
{

#ifdef TARGET_BDS_PROTOCOL_VERSION

    Logger logger(PLUGIN_NAME);

    auto currentProtocol = LL::getServerProtocolVersion();
    if (TARGET_BDS_PROTOCOL_VERSION != currentProtocol)
    {
        logger.warn("Protocol version mismatched! Target version: {}. Current version: {}.",
                    TARGET_BDS_PROTOCOL_VERSION, currentProtocol);
        logger.warn("This will most likely crash the server. Please use the plugin that matches the BDS version!");
    }

#endif // TARGET_BDS_PROTOCOL_VERSION
}

BOOL APIENTRY DllMain(HMODULE hModule,
                      DWORD ul_reason_for_call,
                      LPVOID lpReserved)
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
        LL::registerPlugin(
            PLUGIN_NAME,
            PLUGIN_INTRODUCTION,
            LL::Version(PLUGIN_VERSION_MAJOR, PLUGIN_VERSION_MINOR, PLUGIN_VERSION_REVISION, PLUGIN_LLVERSION_STATUS),
            std::map<std::string, std::string>{
                {"Author", PLUGIN_AUTHOR},
            });
        break;

    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

extern "C"
{
    _declspec(dllexport) void onPostInit()
    {
        std::ios::sync_with_stdio(false);
        CheckProtocolVersion();
        PluginInit();
    }
}
