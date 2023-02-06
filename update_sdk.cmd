@echo off
setlocal enabledelayedexpansion

rem Use system proxy to access git repo if found it configured
for /f "tokens=3* delims= " %%i in ('Reg query "HKCU\Software\Microsoft\Windows\CurrentVersion\Internet Settings" /v ProxyEnable') do (
    if %%i==0x1 (
        echo [INFO] System Proxy enabled. Adapting proxy settings...
        for /f "tokens=3* delims= " %%a in ('Reg query "HKCU\Software\Microsoft\Windows\CurrentVersion\Internet Settings" /v ProxyServer') do set PROXY_ADDR=%%a
        set http_proxy=http://!PROXY_ADDR!
        set https_proxy=http://!PROXY_ADDR!
        echo [INFO] Proxy settings adapted.
        echo.
    )
) 

echo [INFO] Updating SDK...

git submodule update --init --remote

echo [INFO] SDK updated.

pause