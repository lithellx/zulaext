#define _CRT_SECURE_NO_WARNINGS

#include <Windows.h>
#include <Process.h>
#include <iostream>
#include <math.h>
#include <map>
#include <TlHelp32.h>
#include <vector>
#include <thread>
#include <chrono>
#include <array>
#include <fstream>
#include <NetCon.h>
#include <strsafe.h>
#include <random>
#include <tchar.h>
#include <string.h>
#include <wchar.h>

#include <Userenv.h>
#pragma comment(lib,"Userenv")

#include <dwmapi.h>
#pragma comment(lib, "dwmapi.lib")

#include <Psapi.h>
#pragma comment(lib,"psapi")

#include <d3d9.h>
#include <d3dx9.h>
//#include "C:/Program Files (x86)/Microsoft DirectX SDK (June 2010)/Include/d3dx9.h"

#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")

#include "ImGui/imgui.h"
#include "ImGui/imgui_internal.h"
#include "ImGui/imgui_impl_dx9.h"
#include "ImGui/imgui_impl_win32.h"