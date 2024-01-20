# Hnetwork app
A simple packet sniffer. The app can capture all common network packets:
* IPv4
* ICMP
* IGMPv2 and v3
* TCP
* UDP

**Works only on Windows.**

Requirements:
* Qt 6.6.0
* cmake 3.14

Windows Qt system variables:
* On Windows add QTDIR system variable with the value C:\QtDir\QtVersion\msvc2019_64.
* Then add these paths to the Path system variable: %QTDIR%\lib and %QTDIR%\bin

Build:
```cmd
mkdir build
cmake -S . -Bbuild
cmake --build build --config Release
```
