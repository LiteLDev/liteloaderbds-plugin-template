// dllmain.cpp : 定义 DLL 应用程序的入口点。
#include "pch.h"
#include <LLAPI.h>
#pragma comment(lib, "../SDK/Lib/bedrock_server_api.lib")
#pragma comment(lib, "../SDK/Lib/bedrock_server_var.lib")
#pragma comment(lib, "../SDK/Lib/SymDBHelper.lib")
#pragma comment(lib, "../SDK/Lib/LiteLoader.lib")

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
        static_assert(false, "Please modify your plugin registration details and delete this line");
        LL::registerPlugin("PluginName", "Introduction", LL::Version(1, 0, 0), {
                { "Note", "This is a note, you can remove it" },
                { "Author", "???" },
                { "Key", "Value" }
            }
        );
        break;
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

void PluginInit();

extern "C" {
    // Do something after all the plugins loaded
    _declspec(dllexport) void onPostInit() {
        std::ios::sync_with_stdio(false);
        PluginInit();
    }
}
