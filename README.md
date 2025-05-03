## About ZulaExt
External ESP and Aimbot project for Zula. 

Made just for fun. You can do whatever you want with it.

[![ZulaAimbot](https://img.shields.io/github/downloads/lithellx/zulaext/total?style=for-the-badge&label=zulaext%20Downloads&color=red)](https://github.com/lithellx/zulaext/releases)

## How to use
<a href="https://streamable.com/nimbcy">
    <img src="https://cdn-cf-east.streamable.com/image/nimbcy.jpg" width="640" height="360"/>
</a>
<br><br>
1. Drag and drop zulaext_drv.sys on kdmapper_Release.exe (You can do it with cmd to see if driver is successfully mapped)<br>
2. Run Zula game<br>
3. Run zulaext.exe<br>
4. Have fun

<br>

## Known errors and solutions
Error: Your vulnerable driver list is enabled and have blocked the driver loading, you must disable vulnerable driver list to use kdmapper with intel driver
<br>
Solution: Copy the code below and save it as ".reg" file then execute it.
```
Windows Registry Editor Version 5.00

[HKEY_LOCAL_MACHINE\SYSTEM\CurrentControlSet\Control\CI\Config]
"VulnerableDriverBlocklistEnable"=dword:00000000
```
<br>
Error: Access Denied or Insufficient Resources (0xc000009a), Probably some anticheat or antivirus running blocking the load of vulnerable driver
Solution: Make sure you deactivated Windows Defender Real Time Protection, Riot Vanguard, Faceit Anticheat etc. 

## Authors
[lithellx](https://github.com/lithellx)
