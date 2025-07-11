## About ZulaExt

External ESP and Aimbot project for Zula.

Made just for fun. You can do whatever you want with it.

[![ZulaAimbot](https://img.shields.io/github/downloads/lithellx/zulaext/total?style=for-the-badge&label=zulaext%20Downloads&color=red)](https://github.com/lithellx/zulaext/releases)

## How to use
<a href="https://streamable.com/nimbcy">
    <img src="https://cdn-cf-east.streamable.com/image/nimbcy.jpg" width="640" height="360"/>
</a>
<br><br>
1. Load zulaext_drv.sys with XorMapper (get it from my Discord server) or any other mapper.<br>
2. Run the game and wait until the main screen.<br>
3. Run zulaext.exe as administrator on lobby.<br>
4. Press "Insert" key on your keyboard and have fun.

<br>

## Known errors and solutions
Error: Your vulnerable driver list is enabled and have blocked the driver loading, you must disable vulnerable driver list to use kdmapper with intel driver
<br>
Solution: Turn off "Core Isolation" and "Vulnerable Driver Blocklist" options on Windows security settings. And restart your PC afterwards.
To disable Vunlerable Driver Blocklist easily, copy the code below and save it as ".reg" file then execute it.
```
Windows Registry Editor Version 5.00

[HKEY_LOCAL_MACHINE\SYSTEM\CurrentControlSet\Control\CI\Config]
"VulnerableDriverBlocklistEnable"=dword:00000000
```
<hr>
Error: Access Denied or Insufficient Resources (0xc000009a), Probably some anticheat or antivirus running blocking the load of vulnerable driver

Solution: Make sure you deactivated Windows Defender Real Time Protection, Riot Vanguard, Faceit Anticheat etc. 

## Authors
[lithellx](https://github.com/lithellx)
