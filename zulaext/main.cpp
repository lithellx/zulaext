#pragma warning( disable : 4995 )
#pragma warning( disable : 4616 )
#pragma warning( disable : 3491 )
#pragma warning( disable : 1097 )
#pragma warning( disable : 4244 )
#pragma warning( disable : 4477 )
#pragma warning( disable : 4101 )
#pragma warning( disable : 4002 )
#pragma warning( disable : 4305 )
#pragma warning( disable : 4018 )
#pragma warning( disable : 4312 )
#pragma warning( disable : 4313 )
#pragma warning( disable : 4018 )
#pragma warning( disable : 4005 )
#pragma warning( disable : 4102 )
#pragma warning( disable : 4267 )
#pragma warning( disable : 4715 )
#pragma warning( disable : 8015 )

#include <iostream>
#include <assert.h>
#include <windows.h>
#include <atlimage.h>
#include <comutil.h>
#include <filesystem>
#include <stdio.h>
#include <direct.h>
#include <cassert>
#include <Iphlpapi.h>
#include <Assert.h>
#include <string>
#include <algorithm>

#include "xor.h"
#include "vector.h"
#include "functions.h"
#include "includes.h"

#pragma comment(lib,"WS2_32")
#pragma comment(lib, "ntdll.lib")
#pragma comment(lib, "iphlpapi.lib")
#pragma comment(lib, "comsuppw.lib")

using namespace std;

#define isLocalPlayer strstr(entity_handle, "FPS_") && strstr(entity_handle, "_MESH.MDL")
#define isPlayer strstr(entity_handle, "CHAR_") && strstr(entity_handle, "_MESH.MDL")
#define ZulaTeam strstr(entity_handle, "CHAR_HALIL") || strstr(entity_handle, "CHAR_CEMAL") || strstr(entity_handle, "CHAR_AZAD") || strstr(entity_handle, "CHAR_DAYI") || strstr(entity_handle, "CHAR_TAHIR") || strstr(entity_handle, "CHAR_CETIN") || strstr(entity_handle, "CHAR_OSMAN") || strstr(entity_handle, "CHAR_SAMIL") || strstr(entity_handle, "CHAR_CUMALI") || strstr(entity_handle, "CHAR_POKADIN") || strstr(entity_handle, "CHAR_BAHRAM") || strstr(entity_handle, "CHAR_LEYLA") || strstr(entity_handle, "CHAR_OSMAN") || strstr(entity_handle, "CHAR_ALMANCI")
#define GladioTeam strstr(entity_handle, "CHAR_AZER") || strstr(entity_handle, "CHAR_PIERRE") || strstr(entity_handle, "CHAR_BOB") || strstr(entity_handle, "CHAR_AVI") || strstr(entity_handle, "CHAR_EDWARD") || strstr(entity_handle, "CHAR_GEORGE") || strstr(entity_handle,"CHAR_IVAN") || strstr(entity_handle, "CHAR_JACK") || strstr(entity_handle, "CHAR_ROSA") || strstr(entity_handle, "CHAR_PABLO") || strstr(entity_handle, "CHAR_MILLA") || strstr(entity_handle, "CHAR_SALAH") || strstr(entity_handle, "CHAR_XENO") || strstr(entity_handle, "CHAR_KIERA") || strstr(entity_handle, "CHAR_ASYALI")

void processCheat(uint32_t pEntity, Vector RootPos, float distance, bool isTeamValid)
{
	if (!pEntity || !isTeamValid)
		return;

	Vector vRoot, vHead, vFeet, vSpine, vSpine2, vNeck, vPelvis, rThigh;
	if (WorldToScreen(Vector((float)RootPos.x, (float)RootPos.z + 32.f, (float)RootPos.y), vHead, TempMatrix) &&
		WorldToScreen(Vector((float)RootPos.x, (float)RootPos.z - 30.f, (float)RootPos.y), vFeet, TempMatrix) &&
		WorldToScreen(Vector((float)RootPos.x, (float)RootPos.z + 23.f, (float)RootPos.y), vNeck, TempMatrix) &&
		WorldToScreen(Vector((float)RootPos.x, (float)RootPos.z, (float)RootPos.y), vRoot, TempMatrix)) {

		float BoxHeight = vFeet.y - vHead.y;
		float BoxWidth = BoxHeight / 2.4f;

		if (lithellx.esp.line) {
			Vector target = (lineSelected == 0) ? vFeet : (lineSelected == 1) ? vRoot : vHead;
			DrawLine(ScreenCenterX, ScreenCenterY + ((lineSelected == 2) ? -static_cast<int>(ScreenCenterY) : ScreenCenterY), target.x, target.y, &Col.white, 1);
		}

		if (lithellx.esp.box) {
			if (boxSelected == 0) Box3D(RootPos, &Col.red, 1.7);
			else if (boxSelected == 1) DrawESPBox(vHead, vFeet, &Col.red, 1.7);
		}

		char xdist[64];
		sprintf(xdist, "%.fm", distance);

		if (lithellx.esp.distance)
			DrawStrokeText(vFeet.x - (strlen(xdist) * 7 / 2), vFeet.y, &Col.yellow, xdist);

		if (lithellx.aim.aimbot)
			SetEnemy(pEntity, RootPos);
	}
}

void LoopESP()
{
	if (baseAddr == 0)
		baseAddr = getModuleBaseAddr(XorString("acknex.dll"));

	pEntity = readmem<uint32_t>(baseAddr + 0x265E6C);
	while (pEntity)
	{
		if (pEntity != NULL)
		{
			char entity_handle[26];
			uint32_t EntityHandle = readmem<uint32_t>(pEntity + 0xC);
			readmemB(EntityHandle, entity_handle, sizeof(entity_handle));

			if (EntityHandle != NULL)
			{
				if (isLocalPlayer)
					LocalPlayer = pEntity;

				//if(strstr(entity_handle, "TPS_") && strstr(entity_handle, ".MDL")) // yeni modelleri bulmak için
				//	cout << entity_handle << endl;

				if (isPlayer)
				{
					Vector RootPos = readmem<Vector>(pEntity + 0x3DC);
					Vector LocalPos = readmem<Vector>(LocalPlayer + 0x3DC);
					Vector Screen; WorldToScreen(RootPos, Screen, TempMatrix);

					float distance = LocalPos.DistTo(RootPos) / 100.f;

					Vector HeadPos = RootPos;
					HeadPos.z = RootPos.z + 21.f;

					uint32_t matViewProj = readmem<uint32_t>(baseAddr + 0x243F8C);
					D3DXMATRIX Draw = readmem<D3DXMATRIX>(matViewProj);
					if (Draw._44 != 0.0f)
						TempMatrix = Draw;
					else
						Draw = TempMatrix;

					if (true /*isAlive(pEntity)*/)
					{
						if (enemyTeamSelected == 0) processCheat(pEntity, RootPos, distance, ZulaTeam);
						else if (enemyTeamSelected == 1) processCheat(pEntity, RootPos, distance, GladioTeam);
						else processCheat(pEntity, RootPos, distance, ZulaTeam && GladioTeam);
					}
				}

				if (lithellx.esp.c4)
				{
					if (strstr(entity_handle, XorString("SABOTAJ_C4_MEKAN.MDL")))
					{
						if (true /*isPlanted(pEntity)*/)
						{
							Vector Screen, RootPos = readmem<Vector>(pEntity + 0x3DC);
							if (WorldToScreen(Vector((float)RootPos.x, (float)RootPos.z, (float)RootPos.y), Screen, TempMatrix))
							{
								DrawStrokeText(Screen.x, Screen.y, &Col.red, XorString("C4 PLANTED"));
								DrawLine(ScreenCenterX, ScreenCenterY + ScreenCenterY, Screen.x, Screen.y + 10, &Col.red, 2);
							}
						}
					}
				}

				if (lithellx.esp.grenade)
				{
					if (strstr(entity_handle, XorString("SIL_BS_M18_MESH.MDL")) ||
						strstr(entity_handle, XorString("SIL_BG_DUALK4_MESH.MDL")) ||
						strstr(entity_handle, XorString("SIL_BS_TRP90_MESH.MDL")) ||
						strstr(entity_handle, XorString("SIL_BF_M84_MESH.MDL")) ||
						strstr(entity_handle, XorString("SIL_BG_M61_MESH.MDL")) ||
						strstr(entity_handle, XorString("SIL_BG_C4KUMANDA_MESH.MDL")) ||
						strstr(entity_handle, XorString("SIL_BG_MK3_MESH.MDL")) ||
						strstr(entity_handle, XorString("SIL_BG_MK2_MESH.MDL")))
					{
						Vector Screen, RootPos = readmem<Vector>(pEntity + 0x3DC);

						if (WorldToScreen(Vector((float)RootPos.x, (float)RootPos.z, (float)RootPos.y), Screen, TempMatrix))
						{
							DrawStrokeText(Screen.x, Screen.y, &Col.red, XorString("DANGER"));
						}
					}
				}

			}
		}
		pEntity = readmem<uint32_t>(pEntity + 0x4);
	}

	if (lithellx.esp.crosshair)
		Cross(ScreenCenterX, ScreenCenterY);

	if (lithellx.aim.aimbot)
		Aimbot();
}

bool isWaitingMessagePrinted = false;
bool checkGame()
{
	while (true)
	{
		HWND gameWindow = FindWindow(XorString("acknex_mainwin"), 0);
		GetWindowThreadProcessId(gameWindow, &pid);
		if (pid != 0)
		{
			printf(XorString("Game found.\n"));
			return true;
			break;
		}
		else
		{
			if (!isWaitingMessagePrinted) {
				printf(XorString("Waiting for game.\n"));
				isWaitingMessagePrinted = true;
			}
		}
	}
}

bool isActive = true, isGameChecked = true;
void GameProcessHandler()
{
	SetConsoleTitleA(XorString("zulaext by lithellx"));
	while (isActive)
	{
		if (isGameChecked)
		{
			if (!checkGame())
				continue;
			printf(XorString("Please wait.\n"));
			Sleep(1000);
			isGameChecked = false;
		}
		else
			InitializeOverlayWindow();
	}
	isActive = false;
}

int main()
{
	hProcess = OpenProcess(PROCESS_VM_READ, FALSE, getProcessIdByName(processName));
	GameProcessHandler();
	CloseHandle(hProcess);
}

// github.com/lithellx