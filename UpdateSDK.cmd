@echo off
setlocal enabledelayedexpansion

rem Process System Proxy
for /f "tokens=3* delims= " %%i in ('Reg query "HKCU\Software\Microsoft\Windows\CurrentVersion\Internet Settings" /v ProxyEnable') do (
    if %%i==0x1 (
        echo [INFO] System proxy enabled. Adapting settings...
        for /f "tokens=3* delims= " %%a in ('Reg query "HKCU\Software\Microsoft\Windows\CurrentVersion\Internet Settings" /v ProxyServer') do set PROXY_ADDR=%%a
        set http_proxy=http://!PROXY_ADDR!
        set https_proxy=http://!PROXY_ADDR!
        echo [INFO] System proxy enabled. Settings adaptation finished.
        echo.
    )
) 

git submodule update --init --recursive --depth=1

echo.
echo [INFO] LiteLoaderSDK update finished.

pause