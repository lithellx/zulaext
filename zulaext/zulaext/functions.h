#include "includes.h"
#include "xor.h"

#include <ShlObj_core.h>
#pragma comment(lib,"shell32")

#define M_Name " "
#define TopWindowGame 11
#define TopWindowMvoe 22

DWORD pid = 0;
const char* processName = "zula.exe";

void LoopESP();
extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

HWND tWnd = 0x0;
HWND hWnd = 0x0;
HWND GameWnd = NULL;
HWND MyWnd = NULL;
MSG Message = { NULL };
RECT GameRect = { NULL };

IDirect3D9Ex* p_Object = NULL;
IDirect3DDevice9Ex* p_Device = NULL;
D3DPRESENT_PARAMETERS p_Params = { NULL };

int Width = GetSystemMetrics(SM_CXSCREEN);
int Height = GetSystemMetrics(SM_CYSCREEN);
int rWidth = 0;
int rHeight = 0;

const MARGINS Margin = { -1 };
DWORD ScreenCenterX;
DWORD ScreenCenterY;

HANDLE hProcess;
uint32_t pEntity, pEntityX;
uintptr_t baseAddr;
uint32_t LocalPlayer = NULL;
D3DXMATRIX TempMatrix;

bool menu = true;

static int boxSelected = 0;
static int lineSelected = 0;
static int aimKeySelected = 0;
static int aimPosSelected = 0;

const char* boxShape[] = { "3D", "2D" };
const char* linePos[] = { "Bottom", "Middle", "Top" };
const char* aimPos[] = { "Head", "Throat", "Body" };
const char* aimKey[] = { "Capslock", "Shift", "CTRL", "L Click", "R Click" };
const char* enemyTeam[] = { "Zula", "Gladio", "All" };
int enemyTeamSelected;

bool isaimbotting;
float aimFov = 120.f;
float aimSmooth = 1.00f;

struct ProcessInfo {
	HWND targetHWND;
	HWND overlayHWND;
	LPCWSTR className;
	MSG msg;
	RECT wRect;
};
ProcessInfo proc;

typedef struct
{
	DWORD R;
	DWORD G;
	DWORD B;
	DWORD A;
}RGBA;

class Color
{
public:
	RGBA red = { 255, 0, 0, 255 };
	RGBA green = { 0, 255, 0, 255 };
	RGBA blue = { 0, 0, 255, 255 };

	RGBA white = { 255, 255, 255, 255 };
	RGBA black = { 0, 0, 0, 255 };

	RGBA yellow = { 255, 255, 0, 255 };
	RGBA brown = { 192, 96, 0, 255 };
	RGBA pink = { 255, 168, 255, 255 };
	RGBA purple = { 144, 0, 255, 255 };
	RGBA skyblue = { 0, 136, 255, 255 };
	RGBA graygreen = { 128, 160, 128, 255 };
};
Color Col;

struct features
{
	struct esp
	{
		bool box, lookdirection, line, name, distance, skeleton, radar, crosshair, nosmoke, noflash, laser, grenade, c4;
	} esp;

	struct aim
	{
		bool aimbot, aimfov, triggerbot;
	} aim;

	struct misc
	{
		bool norecoil, noreload, noswitch, nospread, rapidfire, antiafk, rapidknife, nosway, fovchanger, allowSpam, speed, laser, megajump, printModelNames;
	} misc;

	struct settings
	{

	} settings;
};
features lithellx;

static std::string string_To_UTF8(const std::string& str)
{
	int nwLen = ::MultiByteToWideChar(CP_ACP, 0, str.c_str(), -1, NULL, 0);

	wchar_t* pwBuf = new wchar_t[nwLen + 1];
	ZeroMemory(pwBuf, nwLen * 2 + 2);

	::MultiByteToWideChar(CP_ACP, 0, str.c_str(), str.length(), pwBuf, nwLen);

	int nLen = ::WideCharToMultiByte(CP_UTF8, 0, pwBuf, -1, NULL, NULL, NULL, NULL);

	char* pBuf = new char[nLen + 1];
	ZeroMemory(pBuf, nLen + 1);

	::WideCharToMultiByte(CP_UTF8, 0, pwBuf, nwLen, pBuf, nLen, NULL, NULL);

	std::string retStr(pBuf);

	delete[]pwBuf;
	delete[]pBuf;

	pwBuf = NULL;
	pBuf = NULL;

	return retStr;
}

static void DrawStrokeText(int x, int y, RGBA* color, const char* str)
{
	ImFont a;
	std::string utf_8_1 = std::string(str);
	std::string utf_8_2 = string_To_UTF8(utf_8_1);
	ImGui::GetForegroundDrawList()->AddText(ImVec2(x, y - 1), ImGui::ColorConvertFloat4ToU32(ImVec4(1 / 255.0, 1 / 255.0, 1 / 255.0, 255 / 255.0)), utf_8_2.c_str());
	ImGui::GetForegroundDrawList()->AddText(ImVec2(x, y + 1), ImGui::ColorConvertFloat4ToU32(ImVec4(1 / 255.0, 1 / 255.0, 1 / 255.0, 255 / 255.0)), utf_8_2.c_str());
	ImGui::GetForegroundDrawList()->AddText(ImVec2(x - 1, y), ImGui::ColorConvertFloat4ToU32(ImVec4(1 / 255.0, 1 / 255.0, 1 / 255.0, 255 / 255.0)), utf_8_2.c_str());
	ImGui::GetForegroundDrawList()->AddText(ImVec2(x + 1, y), ImGui::ColorConvertFloat4ToU32(ImVec4(1 / 255.0, 1 / 255.0, 1 / 255.0, 255 / 255.0)), utf_8_2.c_str());
	ImGui::GetForegroundDrawList()->AddText(ImVec2(x, y), ImGui::ColorConvertFloat4ToU32(ImVec4(color->R / 255.0, color->G / 255.0, color->B / 255.0, color->A / 255.0)), utf_8_2.c_str());
}

static void DrawNewText(int x, int y, RGBA* color, const char* str)
{
	//	ImFont a;
	//litheIO& io = ImGui::GetIO();
		//ImFont* pFont = io.Fonts->AddFontFromFileTTF("sansation.ttf", 10.0f);
		//a.FontSize = 5.f;
	std::string utf_8_1 = std::string(str);
	std::string utf_8_2 = string_To_UTF8(utf_8_1);
	ImGui::GetForegroundDrawList()->AddText(ImVec2(x, y), ImGui::ColorConvertFloat4ToU32(ImVec4(color->R / 255.0, color->G / 255.0, color->B / 255.0, color->A / 255.0)), utf_8_2.c_str());
}

static void DrawRect(int x, int y, int w, int h, RGBA* color, int thickness)
{
	ImGui::GetForegroundDrawList()->AddRect(ImVec2(x, y), ImVec2(x + w, y + h), ImGui::ColorConvertFloat4ToU32(ImVec4(color->R / 255.0, color->G / 255.0, color->B / 255.0, color->A / 255.0)), 0, 0, thickness);
}

static void DrawFilledRect(int x, int y, int w, int h, RGBA* color)
{
	ImGui::GetForegroundDrawList()->AddRectFilled(ImVec2(x, y), ImVec2(x + w, y + h), ImGui::ColorConvertFloat4ToU32(ImVec4(color->R / 255.0, color->G / 255.0, color->B / 255.0, color->A / 255.0)), 0, 0);
}

void DrawColoredRect(int x, int y, int w, int h, RGBA* color, int thickness)
{
	ImGui::GetForegroundDrawList()->AddRect(ImVec2(x, y), ImVec2(x + w, y + h), ImGui::ColorConvertFloat4ToU32(ImVec4(color->R / 255.0, color->G / 255.0, color->B / 255.0, color->A / 255)), 3.f, 0, thickness);
}

static void DrawFilledRectIMCol(int x, int y, int w, int h, ImU32 col)
{
	ImGui::GetForegroundDrawList()->AddRectFilled(ImVec2(x, y), ImVec2(x + w, y + h), col, 0, 0);
}

static void DrawCircleFilled(int x, int y, int radius, RGBA* color, int segments)
{
	ImGui::GetForegroundDrawList()->AddCircleFilled(ImVec2(x, y), radius, ImGui::ColorConvertFloat4ToU32(ImVec4(color->R / 255.0, color->G / 255.0, color->B / 255.0, color->A / 255.0)), segments);
}

static void DrawCircle(int x, int y, int radius, RGBA* color, int segments)
{
	ImGui::GetForegroundDrawList()->AddCircle(ImVec2(x, y), radius, ImGui::ColorConvertFloat4ToU32(ImVec4(color->R / 255.0, color->G / 255.0, color->B / 255.0, color->A / 255.0)), segments);
}

static void DrawTriangle(int x1, int y1, int x2, int y2, int x3, int y3, RGBA* color, float thickness)
{
	ImGui::GetForegroundDrawList()->AddTriangle(ImVec2(x1, y1), ImVec2(x2, y2), ImVec2(x3, y3), ImGui::ColorConvertFloat4ToU32(ImVec4(color->R / 255.0, color->G / 255.0, color->B / 255.0, color->A / 255.0)), thickness);
}

static void DrawTriangleFilled(int x1, int y1, int x2, int y2, int x3, int y3, RGBA* color)
{
	ImGui::GetForegroundDrawList()->AddTriangleFilled(ImVec2(x1, y1), ImVec2(x2, y2), ImVec2(x3, y3), ImGui::ColorConvertFloat4ToU32(ImVec4(color->R / 255.0, color->G / 255.0, color->B / 255.0, color->A / 255.0)));
}

static void DrawLine(int x1, int y1, int x2, int y2, RGBA* color, int thickness)
{
	ImGui::GetForegroundDrawList()->AddLine(ImVec2(x1, y1), ImVec2(x2, y2), ImGui::ColorConvertFloat4ToU32(ImVec4(color->R / 255.0, color->G / 255.0, color->B / 255.0, color->A / 255.0)), thickness);
}

template <class dataType>
dataType readmem(DWORD addressToRead)
{
	dataType rpmbuffer;
	ReadProcessMemory(hProcess, (PVOID)addressToRead, &rpmbuffer, sizeof(rpmbuffer), 0);
	return rpmbuffer;
}

template <class data>
void readmemB(DWORD addressToRead, data* handle, SIZE_T size)
{
	ReadProcessMemory(hProcess, (PVOID)addressToRead, handle, size, 0);
}

uintptr_t getProcessIdByName(const char* processName)
{
	uintptr_t processId = 0;
	HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

	if (snapshot != INVALID_HANDLE_VALUE) {
		PROCESSENTRY32 processEntry;
		processEntry.dwSize = sizeof(processEntry);

		if (Process32First(snapshot, &processEntry)) {
			do {
				if (!_stricmp(processEntry.szExeFile, processName)) {
					processId = processEntry.th32ProcessID;
					break;
				}
			} while (Process32Next(snapshot, &processEntry));
		}

		CloseHandle(snapshot);
	}

	return processId;
}

uintptr_t getModuleBaseAddr(const char* moduleName)
{
	uintptr_t baseAddr = 0;
	MODULEENTRY32 moduleEntry = { sizeof(MODULEENTRY32) };

	HANDLE handle = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, pid);

	if (handle != INVALID_HANDLE_VALUE) {
		if (Module32First(handle, &moduleEntry)) {
			do {
				if (_stricmp(moduleEntry.szModule, moduleName) == 0) {
					baseAddr = reinterpret_cast<uintptr_t>(moduleEntry.modBaseAddr);
					break;
				}
			} while (Module32Next(handle, &moduleEntry));
		}
		CloseHandle(handle);
	}

	return baseAddr;
}

bool WorldToScreen(Vector WorldPos, Vector& Screen, D3DXMATRIX matrix)
{
	float z = WorldPos.x * matrix._13 + WorldPos.y * matrix._23 + WorldPos.z * matrix._33 + matrix._43;

	if (z < 0.0f)
		return false;

	float x = WorldPos.x * matrix._11 + WorldPos.y * matrix._21 + WorldPos.z * matrix._31 + matrix._41;
	float y = WorldPos.x * matrix._12 + WorldPos.y * matrix._22 + WorldPos.z * matrix._32 + matrix._42;

	x /= z;
	y /= z;

	Screen.x = (1.0f + x) * (Width / 2);
	Screen.y = (1.0f - y) * (Height / 2);

	return true;
}

bool AimKey()
{
	switch (aimKeySelected)
	{
	case 0: return GetAsyncKeyState(VK_CAPITAL); break;
	case 1: return GetAsyncKeyState(VK_SHIFT); break;
	case 2: return GetAsyncKeyState(VK_CONTROL); break;
	case 3: return GetAsyncKeyState(LVKF_ALT); break;
	case 4: return GetAsyncKeyState(VK_LBUTTON); break;
	case 5: return GetAsyncKeyState(VK_RBUTTON); break;
	default: break;
	}
}

float DistanceBetweenCross(float X, float Y)
{
	float ydist = (Y - (Height / 2));
	float xdist = (X - (Width / 2));
	float Hypotenuse = sqrt(pow(ydist, 2) + pow(xdist, 2));
	return Hypotenuse;
}

bool GetClosestPlayerToCrossHair(Vector RootPos, float& max, float aimfov)
{
	float Dist = DistanceBetweenCross(RootPos.x, RootPos.y);

	if (Dist < max && Dist < aimfov)
	{
		max = Dist;
		aimFov = aimfov;
		return true;
	}
	return false;
}

void Cross(int CenterX, int CenterY)
{
	DrawFilledRect(CenterX - 9, CenterY, 7, 2, &Col.white); // Left
	DrawFilledRect(CenterX + 4, CenterY, 7, 2, &Col.white); // Right
	DrawFilledRect(CenterX, CenterY - 9, 2, 7, &Col.white); // Top
	DrawFilledRect(CenterX, CenterY + 4, 2, 7, &Col.white); // Bottom	
}

int SendMoveInput(float dwx, float dwy) {

	INPUT Inputs[3] = { 0 };
	Inputs[0].type = INPUT_MOUSE;
	Inputs[0].mi.dx = (LONG)dwx; // X Ekseni
	Inputs[0].mi.dy = (LONG)dwy; // Y Ekseni
	Inputs[0].mi.dwFlags = MOUSEEVENTF_MOVE;

	SendInput(3, Inputs, sizeof(INPUT));
	return true;
}

static void MoveCross()
{
	float TargetX = 0;
	float TargetY = 0;

	Vector RootPos = readmem<Vector>(pEntityX + 0x3DC);
	Vector spos2; WorldToScreen(Vector(RootPos.x, RootPos.z += 15.f, RootPos.y), spos2, TempMatrix);

	if (spos2.x != 0 || spos2.y != 0)
	{
		if (DistanceBetweenCross(spos2.x, spos2.y) || isaimbotting)
		{
			//X Axis
			if (spos2.x != 0)
			{
				if (spos2.x > ScreenCenterX)
				{
					TargetX = -(ScreenCenterX - spos2.x);
					TargetX /= aimSmooth;
					if (TargetX + ScreenCenterX > ScreenCenterX * 2) TargetX = 0;
				}

				if (spos2.x < ScreenCenterX)
				{
					TargetX = spos2.x - ScreenCenterX;
					TargetX /= aimSmooth;
					if (TargetX + ScreenCenterX < 0) TargetX = 0;
				}
			}

			//Y Axis
			if (spos2.y != 0)
			{
				if (spos2.y > ScreenCenterY)
				{
					TargetY = -(ScreenCenterY - spos2.y);
					TargetY /= aimSmooth;
					if (TargetY + ScreenCenterY > ScreenCenterY * 2) TargetY = 0;
				}

				if (spos2.y < ScreenCenterY)
				{
					TargetY = spos2.y - ScreenCenterY;
					TargetY /= aimSmooth;
					if (TargetY + ScreenCenterY < 0) TargetY = 0;
				}
			}
			SendMoveInput((int)TargetX, (int)TargetY);
		}
	}
}

void Aimbot()
{
	if (pEntityX)
	{
		if (AimKey()/* && isAlive(entityx)*/)
		{
			isaimbotting = true;
			MoveCross();
		}
		else
		{
			isaimbotting = false;
			pEntityX = 0;
		}
	}
	else
		isaimbotting = false;
}

void SetEnemy(uint32_t pEntity, Vector RootPos)
{
	float max = 30.f;

	Vector enemyPos;
	WorldToScreen(Vector(RootPos.x, RootPos.z, RootPos.y), enemyPos, TempMatrix);

	if (GetClosestPlayerToCrossHair(enemyPos, max, aimFov))
		pEntityX = pEntity;
}

void DrawESPBox(Vector Head, Vector Foot, RGBA* dwColor, int thickness)
{
	Vector Box = Head - Foot;

	if (Box.y < 0)
		Box.y *= -1;

	int BoxWidth = (int)Box.y / 2;
	int DrawX = (int)Head.x - (BoxWidth / 2);
	int DrawY = (int)Head.y;

	DrawColoredRect(DrawX, DrawY, BoxWidth, (int)Box.y, dwColor, thickness);
}

void Box3D(Vector RootPos, RGBA* Color, int Thickness)
{
	auto a1 = RootPos.x;
	auto a2 = RootPos.z;
	auto a3 = RootPos.y;
	auto v7 = a1 - 16.3f;
	auto v8 = a2 - 43.f;
	auto v9 = a3 - 20.f;
	auto v10 = a1 + 17.f;
	auto v12 = a3 + 14.f;
	Vector line1, line2, line3, line4, line5, line6, line7, line8;
	if (WorldToScreen(Vector(v7, v8, v9), line1, TempMatrix))
	{
		auto v4 = a2 + 32.f;
		auto v11 = a2 + 32.f;
		auto v13 = v4;
		auto v14 = v4;

		if (WorldToScreen(Vector(v10, v4, v12), line2, TempMatrix))
		{
			if (WorldToScreen(Vector(v10, v8, v9), line3, TempMatrix))
			{
				if (WorldToScreen(Vector(v7, v11, v9), line4, TempMatrix))
				{
					if (WorldToScreen(Vector(v7, v8, v12), line5, TempMatrix))
					{
						if (WorldToScreen(Vector(v7, v13, v12), line6, TempMatrix))
						{
							if (WorldToScreen(Vector(v10, v8, v12), line7, TempMatrix))
							{
								if (WorldToScreen(Vector(v10, v14, v9), line8, TempMatrix))
								{
									DrawLine(line1.x, line1.y, line4.x, line4.y, Color, Thickness);
									DrawLine(line3.x, line3.y, line8.x, line8.y, Color, Thickness);
									DrawLine(line7.x, line7.y, line2.x, line2.y, Color, Thickness);
									DrawLine(line5.x, line5.y, line6.x, line6.y, Color, Thickness);

									DrawLine(line1.x, line1.y, line3.x, line3.y, Color, Thickness);
									DrawLine(line3.x, line3.y, line7.x, line7.y, Color, Thickness);
									DrawLine(line7.x, line7.y, line5.x, line5.y, Color, Thickness);
									DrawLine(line5.x, line5.y, line1.x, line1.y, Color, Thickness);

									DrawLine(line4.x, line4.y, line8.x, line8.y, Color, Thickness);
									DrawLine(line8.x, line8.y, line2.x, line2.y, Color, Thickness);
									DrawLine(line2.x, line2.y, line6.x, line6.y, Color, Thickness);
									DrawLine(line6.x, line6.y, line4.x, line4.y, Color, Thickness);
								}
							}
						}
					}
				}
			}
		}
	}
}

namespace Render
{
	void ApplyDarkTheme()
	{
		ImGuiStyle& style = ImGui::GetStyle();

		style.Colors[ImGuiCol_WindowBg] = ImVec4(0.10f, 0.10f, 0.10f, 1.00f);
		style.Colors[ImGuiCol_Text] = ImVec4(0.95f, 0.95f, 0.95f, 1.00f);
		style.Colors[ImGuiCol_Border] = ImVec4(0.30f, 0.30f, 0.30f, 1.00f);
		style.Colors[ImGuiCol_FrameBg] = ImVec4(0.20f, 0.20f, 0.20f, 1.00f);
		style.Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.30f, 0.30f, 0.30f, 1.00f);
		style.Colors[ImGuiCol_FrameBgActive] = ImVec4(0.40f, 0.40f, 0.40f, 1.00f);

		style.Colors[ImGuiCol_Button] = ImVec4(0.25f, 0.25f, 0.25f, 1.00f);
		style.Colors[ImGuiCol_ButtonHovered] = ImVec4(0.35f, 0.35f, 0.35f, 1.00f);
		style.Colors[ImGuiCol_ButtonActive] = ImVec4(0.45f, 0.45f, 0.45f, 1.00f);

		style.Colors[ImGuiCol_Header] = ImVec4(0.30f, 0.30f, 0.30f, 1.00f);
		style.Colors[ImGuiCol_HeaderHovered] = ImVec4(0.40f, 0.40f, 0.40f, 1.00f);
		style.Colors[ImGuiCol_HeaderActive] = ImVec4(0.50f, 0.50f, 0.50f, 1.00f);

		style.Colors[ImGuiCol_TitleBg] = ImVec4(0.15f, 0.15f, 0.15f, 1.00f);
		style.Colors[ImGuiCol_TitleBgActive] = ImVec4(0.20f, 0.20f, 0.20f, 1.00f);
		style.Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.10f, 0.10f, 0.10f, 1.00f);

		style.WindowRounding = 8.0f;
		style.FrameRounding = 5.0f;
		style.ScrollbarRounding = 5.0f;
		style.GrabRounding = 5.0f;
	}

	void DrawMenu(IDirect3DDevice9* pDevice)
	{
		if (GetAsyncKeyState(VK_INSERT) & 1)
			menu = !menu;

		LoopESP();

		ApplyDarkTheme();
		ImGui::GetIO().MouseDrawCursor = menu;
		if (menu)
		{
			ImGui::Begin(XorString("zulaext"), &menu, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoSavedSettings);

			// Esp
			ImGui::Text(XorString("ESP"));
			ImGui::Separator();

			ImGui::Checkbox(XorString("Enable ESP"), &lithellx.esp.box);

			ImGui::Checkbox(XorString("Show Distance"), &lithellx.esp.distance);

			ImGui::Checkbox(XorString("Draw Lines"), &lithellx.esp.line);
			ImGui::PushItemWidth(100);
			ImGui::Combo(XorString("##espline"), &lineSelected, linePos, IM_ARRAYSIZE(linePos));

			ImGui::Checkbox(XorString("Show Grenades"), &lithellx.esp.grenade);

			ImGui::Checkbox(XorString("Show Bomb (C4)"), &lithellx.esp.c4);

			ImGui::Checkbox(XorString("Crosshair"), &lithellx.esp.crosshair);

			// Aimbot
			ImGui::NewLine();
			ImGui::Text(XorString("AIM"));
			ImGui::Separator();

			ImGui::Checkbox(XorString("Enable Aimbot"), &lithellx.aim.aimbot);

			ImGui::Text(XorString("Aim Position:"));
			ImGui::PushItemWidth(100);
			ImGui::Combo(XorString("##aimpos"), &aimPosSelected, aimPos, IM_ARRAYSIZE(aimPos));

			ImGui::Text(XorString("Aim Key:"));
			ImGui::PushItemWidth(100);
			ImGui::Combo(XorString("##aimkey"), &aimKeySelected, aimKey, IM_ARRAYSIZE(aimKey));

			ImGui::Text(XorString("Aim FOV:"));
			ImGui::SliderFloat(XorString("##aimfov"), &aimFov, 60.f, 120.f, XorString("%.2f"));

			ImGui::Text(XorString("Aim Smooth:"));
			ImGui::SliderFloat(XorString("##aimsmooth"), &aimSmooth, 0.50f, 5.00f, XorString("%.2f"));

			// Settings
			ImGui::NewLine();
			ImGui::Text(XorString("SETTINGS"));
			ImGui::Separator();

			ImGui::Text(XorString("Enemy Team"));
			ImGui::Combo(XorString("##enemyteam"), &enemyTeamSelected, enemyTeam, IM_ARRAYSIZE(enemyTeam));

			ImGui::End();
		}
		ImGui::Render();
		ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());
	}
}

static void CleanuoD3D()
{
	if (p_Device != NULL)
	{
		p_Device->EndScene();
		p_Device->Release();
	}

	if (p_Object != NULL)
		p_Object->Release();
}

static int isTopWin()
{
	HWND hWnd = GetForegroundWindow();
	if (hWnd == GameWnd)
		return TopWindowGame;
	if (hWnd == MyWnd)
		return TopWindowMvoe;

	return 0;
}

static void SetWindowToTarget()
{
	while (1)
	{
		HWND XGameWnd = FindWindow(XorString("acknex_mainwin"), 0);
		if (XGameWnd)
		{
			ZeroMemory(&GameRect, sizeof(GameRect));
			GetWindowRect(XGameWnd, &GameRect);
			Width = GameRect.right - GameRect.left;
			Height = GameRect.bottom - GameRect.top;
			DWORD dwStyle = GetWindowLong(XGameWnd, GWL_STYLE);

			if (dwStyle & WS_BORDER)
			{
				GameRect.top += 32;
				Height -= 39;
			}
			ScreenCenterX = Width / 2;
			ScreenCenterY = Height / 2;
			MoveWindow(MyWnd, GameRect.left, GameRect.top, Width, Height, true);
		}
		Sleep(5);
	}
}

BOOL CALLBACK EnumWindowsCallback(HWND hwnd, LPARAM lParam)
{
	wchar_t className[255] = L"";
	GetClassNameW(hwnd, className, 255);
	//litzext
	std::wstring xw = std::wstring(L"z");

	std::wstring yw = (std::wstring(L"lit") + xw + std::wstring(L"ext"));
	const wchar_t* ptr = yw.c_str();

	if (wcscmp((wchar_t*)ptr, className) == 0)
	{
		ProcessInfo* proc = (ProcessInfo*)lParam;
		proc->overlayHWND = hwnd;
		memset((void*)&ptr[0], 0x00, yw.size() * 2);
		yw.clear();
		return TRUE;
	}
	return TRUE;
}

static LRESULT CALLBACK WinProc(HWND hWnd, UINT Message, WPARAM wParam, LPARAM lParam)
{
	if (ImGui_ImplWin32_WndProcHandler(hWnd, Message, wParam, lParam))
		return true;

	switch (Message)
	{
	case WM_DESTROY:
		CleanuoD3D();
		PostQuitMessage(0);
		exit(4);
		break;
	case WM_SIZE:
		if (p_Device != NULL && wParam != SIZE_MINIMIZED)
		{
			ImGui_ImplDX9_InvalidateDeviceObjects();
			p_Params.BackBufferWidth = LOWORD(lParam);
			p_Params.BackBufferHeight = HIWORD(lParam);
			HRESULT hr = p_Device->Reset(&p_Params);
			if (hr == D3DERR_INVALIDCALL)
				IM_ASSERT(0);
			ImGui_ImplDX9_CreateDeviceObjects();
		}
		break;
	default:
		return DefWindowProc(hWnd, Message, wParam, lParam);
		break;
	}
	return 0;
}

static HRESULT DirectXInit(HWND hWnd)
{
	CreateThread(0, 0, (LPTHREAD_START_ROUTINE)SetWindowToTarget, 0, 0, 0);

	WNDCLASSEX wClass =
	{
		sizeof(WNDCLASSEX),0,WinProc,0,0,nullptr,LoadIcon(nullptr, IDI_APPLICATION),LoadCursor(nullptr, IDC_ARROW),nullptr,nullptr, M_Name,LoadIcon(nullptr, IDI_APPLICATION)
	};

	if (!RegisterClassEx(&wClass))
		exit(1);

	GameWnd = FindWindow(XorString("litzext"), 0);
	if (GameWnd)
	{
		GetClientRect(GameWnd, &GameRect);
		POINT xy;
		ClientToScreen(GameWnd, &xy);
		GameRect.left = xy.x;
		GameRect.top = xy.y;

		Width = GameRect.right;
		Height = GameRect.bottom;
	}
	else exit(2);
	DwmExtendFrameIntoClientArea(MyWnd, &Margin);

	SetWindowLong(MyWnd, GWL_EXSTYLE, WS_EX_LAYERED | WS_EX_TRANSPARENT | WS_EX_TOOLWINDOW);

	if (FAILED(Direct3DCreate9Ex(D3D_SDK_VERSION, &p_Object)))
		exit(3);

	ZeroMemory(&p_Params, sizeof(p_Params));
	p_Params.Windowed = TRUE;
	p_Params.SwapEffect = D3DSWAPEFFECT_DISCARD;
	p_Params.hDeviceWindow = hWnd;
	p_Params.MultiSampleQuality = D3DMULTISAMPLE_NONE;
	p_Params.BackBufferFormat = D3DFMT_A8R8G8B8;
	p_Params.BackBufferWidth = Width;
	p_Params.BackBufferHeight = Height;
	p_Params.EnableAutoDepthStencil = TRUE;
	p_Params.AutoDepthStencilFormat = D3DFMT_D16;

	if (FAILED(p_Object->CreateDeviceEx(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd, D3DCREATE_HARDWARE_VERTEXPROCESSING, &p_Params, 0, &p_Device)))
	{
		p_Object->Release();
		exit(4);
	}

	ImGui::CreateContext();
	ImGui_ImplWin32_Init(hWnd);
	ImGui_ImplDX9_Init(p_Device);
	p_Object->Release();

	return S_OK;
}

void RenderFrame() {

	p_Params.BackBufferHeight = rHeight;
	p_Params.BackBufferWidth = rWidth;
	p_Device->Reset(&p_Params);
	p_Device->Clear(0, 0, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0, 1.0f, 0);
	p_Device->BeginScene();

	tWnd = FindWindow(XorString("acknex_mainwin"), 0);
	if (tWnd == GetForegroundWindow())
	{
		ImGui_ImplDX9_NewFrame();
		ImGui_ImplWin32_NewFrame();
		ImGui::NewFrame();
		Render::DrawMenu(p_Device);
	}

	p_Device->EndScene();
	p_Device->PresentEx(0, 0, 0, 0, 0);
}

WPARAM MainLoop()
{
	static RECT old_rc;
	ZeroMemory(&Message, sizeof(MSG));

	while (Message.message != WM_QUIT)
	{
		if (PeekMessage(&Message, MyWnd, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&Message);
			DispatchMessage(&Message);
		}

		HWND hwnd_active = GetForegroundWindow();

		if (hwnd_active == GameWnd) {
			HWND hwndtest = GetWindow(hwnd_active, GW_HWNDPREV);
			SetWindowPos(MyWnd, hwndtest, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
		}
		RECT rc;
		POINT xy;

		ZeroMemory(&rc, sizeof(RECT));
		ZeroMemory(&xy, sizeof(POINT));
		GetClientRect(GameWnd, &rc);
		ClientToScreen(GameWnd, &xy);
		rc.left = xy.x;
		rc.top = xy.y;

		ImGuiIO& io = ImGui::GetIO();
		io.ImeWindowHandle = GameWnd;

		POINT p;
		GetCursorPos(&p);
		io.MousePos.x = p.x - xy.x;
		io.MousePos.y = p.y - xy.y;

		if (GetAsyncKeyState(VK_LBUTTON))
		{
			io.MouseDown[0] = true;
			io.MouseClicked[0] = true;
			io.MouseClickedPos[0].x = io.MousePos.x;
			io.MouseClickedPos[0].x = io.MousePos.y;
		}
		else
			io.MouseDown[0] = false;
		if (rc.left != old_rc.left || rc.right != old_rc.right || rc.top != old_rc.top || rc.bottom != old_rc.bottom)
		{
			old_rc = rc;

			Width = rc.right;
			Height = rc.bottom;

			p_Params.BackBufferWidth = Width;
			p_Params.BackBufferHeight = Height;
			SetWindowPos(MyWnd, (HWND)0, xy.x, xy.y, Width, Height, SWP_NOREDRAW);
			p_Device->Reset(&p_Params);
		}
		RenderFrame();

	}
	ImGui_ImplDX9_Shutdown();

	CleanuoD3D();
	DestroyWindow(MyWnd);

	return Message.wParam;
}

DWORD InitializeOverlayWindow()
{
	const MARGINS margins = { -1 ,-1, -1, -1 };
	WNDCLASSEXA wc;
	MSG Msg;
	HINSTANCE hInstance = 0;
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = DefWindowProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = (HINSTANCE)hInstance;
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)(RGB(0, 0, 0));
	wc.lpszMenuName = NULL;
	wc.lpszClassName = XorString("litzext");
	wc.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
	RegisterClassExA(&wc);

	MyWnd = CreateWindowExA(
		WS_EX_LAYERED | WS_EX_TRANSPARENT | WS_EX_TOPMOST,
		XorString("litzext"),
		XorString("litzext"),
		WS_POPUP | WS_VISIBLE,
		GameRect.left, GameRect.top, 1920, 1080,
		NULL, NULL, hInstance, 0);

	SetLayeredWindowAttributes(MyWnd, RGB(0, 0, 0), 255, LWA_ALPHA);
	MARGINS Margin = { -1 };
	DwmExtendFrameIntoClientArea(MyWnd, &Margin);

	ShowWindow(MyWnd, SW_SHOW);
	UpdateWindow(MyWnd);
	EnumWindows(EnumWindowsCallback, (LPARAM)&proc);
	system("cls");
	std::cout << XorString("Toggle menu with [Insert] key.") << std::endl;
	DirectXInit(MyWnd);
	MainLoop();
	return 0;
}

// github.com/lithellx