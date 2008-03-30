#include "stdafx.h"
#include <windows.h>
#include <string>
#include <list>
#include "Constants.h"
#include "Core.h"
#include "Packet.h"
#include "Battlelist.h"

#ifdef _MANAGED
#pragma managed(push, off)
#endif

using namespace std;

#define MAX_TEXT 512
#define PTEXT_FIRST 150

/* DisplayText. Credits for Displaying text goes to Stiju and Zionz. Thanks for the help!*/
Ctext texts[MAX_TEXT] = {0};
list<PlayerText> CreatureTexts;
//BLAddress BLConsts;
int PlayerCount = 0;

int n_text=0;
char g_Text[1024] = "";
int g_Red = 0;
int g_Green = 0;
int g_Blue = 0;
int g_X = 0;
int g_Y = 0;
int g_Font = 1;

char ttext[1024]="'(use object on target)',0";
char tbuff[2048];

/*Address are loaded from Constants.xml file */

bool FirstTime = true;

inline bool InGame(){ return (*Consts::INGAME == 8); }

inline bool PopupOpened() { return (*Consts::POPUP == 11); }

void MyPrintName(int nSurface, int nX, int nY, int nFont, int nRed, int nGreen, int nBlue, char* lpText, int nAlign)
{
	//TODO Calling this will fuck up the EIP
	PrintText(nSurface, nX, nY, nFont, nRed, nGreen, nBlue, lpText, nAlign);
	/*MessageBoxA(0, "Drawn Normal Text", "Info",0);
	//Write text above player
	DWORD *EntityID = (DWORD*)(lpText - 4);
	if (*EntityID < 0x40000000)
	{
		for(list<PlayerText>::iterator it=CreatureTexts.begin(); it!=CreatureTexts.end(); ++it) {
			if (*EntityID = (*it).CreatureId) {
				PrintText(0x01, (*it).RelativeX, (*it).RelativeY, (*it).TextFont, (*it).cR, (*it).cG, (*it).cB, (*it).DisplayText, 0x00);
			}
		}
	}*/
}

void MyPrintFps(int nSurface, int nX, int nY, int nFont, int nRed, int nGreen, int nBlue, char* lpText, int nAlign)
{
	bool *fps = (bool*)Consts::ptrShowFPS;
	if(*fps == true)
	{
		PrintText(nSurface, nX, nY, nFont, nRed, nGreen, nBlue, lpText, nAlign);
		nY += 12;
	}
	//TODO Normal Writing DONE but not tested
	for(int i=0; i<MAX_TEXT; i++){
		if(!texts[i].used)
			continue;
		
		strcpy(g_Text,texts[i].text);
		g_Blue = texts[i].b;
		g_Green = texts[i].g;
		g_Red = texts[i].r;
		g_Y = texts[i].y;
		g_X = texts[i].x;
		g_Font = texts[i].font;

		PrintText(0x01, g_X, g_Y, g_Font, g_Red, g_Green, g_Blue, g_Text, 0x00); //0x01 Surface, 0x00 Align
	}
}

DWORD HookCall(DWORD dwAddress, DWORD dwFunction)
{
	DWORD dwOldProtect, dwNewProtect, dwOldCall, dwNewCall;
	BYTE callByte[5] = {0xE8, 0x00, 0x00, 0x00, 0x00};

	dwNewCall = dwFunction - dwAddress - 5;
	memcpy(&callByte[1], &dwNewCall, 4);
	
	VirtualProtectEx(GetCurrentProcess(), (LPVOID)(dwAddress), 5, PAGE_READWRITE, &dwOldProtect);
	memcpy(&dwOldCall, (LPVOID)(dwAddress+1), 4);
	memcpy((LPVOID)(dwAddress), &callByte, 5);
	VirtualProtectEx(GetCurrentProcess(), (LPVOID)(dwAddress), 5, dwOldProtect, &dwNewProtect);
	return dwOldCall;
}

void UnhookCall(DWORD dwAddress, DWORD dwOldCall)
{
	DWORD dwOldProtect, dwNewProtect;
	BYTE callByte[5] = {0xE8, 0x00, 0x00, 0x00, 0x00};

	memcpy(&callByte[1], &dwOldCall, 4);
	
	VirtualProtectEx(GetCurrentProcess(), (LPVOID)(dwAddress), 5, PAGE_READWRITE, &dwOldProtect);
	memcpy((LPVOID)(dwAddress), &callByte, 5);
	VirtualProtectEx(GetCurrentProcess(), (LPVOID)(dwAddress), 5, dwOldProtect, &dwNewProtect);
}

void Nop(DWORD dwAddress, int size)
{
	DWORD dwOldProtect, dwNewProtect;
	VirtualProtectEx(GetCurrentProcess(), (LPVOID)(dwAddress), size, PAGE_READWRITE, &dwOldProtect);
	memset((LPVOID)(dwAddress), 0x90, size);
	VirtualProtectEx(GetCurrentProcess(), (LPVOID)(dwAddress), size, dwOldProtect, &dwNewProtect);
}


void SetText(unsigned int TextNum, bool enabled, int nX, int nY, int nRed, int nGreen, int nBlue, int font, char *lpText, int id = 0)
{
	if(TextNum > MAX_TEXT-1){return;}
	if(font<1 || font >4){return;}
	if(nRed > 0xFF || nRed < 0){return;}
	if(nGreen > 0xFF || nGreen < 0){return;}
	if(nBlue > 0xFF || nBlue < 0){return;}

	strcpy(texts[TextNum].text, lpText);
	texts[TextNum].r=nRed;
	texts[TextNum].g=nGreen;
	texts[TextNum].b=nBlue;
	texts[TextNum].x=nX;
	texts[TextNum].y=nY;
	texts[TextNum].used = enabled;
	texts[TextNum].font = font;
	if (texts[TextNum].EntityID != id)
		texts[TextNum].EntityID = id;
}

void __declspec(noreturn) UninjectSelf(HMODULE Module)
{
   __asm
   {
      push -2
      push 0
      push Module
      mov eax, TerminateThread
      push eax
      mov eax, FreeLibrary
      jmp eax
   }
}


LRESULT __stdcall WindowProc(HWND hWnd, int uMsg, WPARAM wParam, LPARAM lParam){
	static int i;
	switch (uMsg) {
		case WM_CHAR:
			{
				/*
				if (wParam == 'q'){
					int t = 0x000001;
					CallWindowProc(WndProc, hWnd, WM_KEYDOWN, VK_SHIFT, t);
					t=0x000001;
					CallWindowProc(WndProc, hWnd, WM_KEYDOWN, VK_F1, t );
					t=0xc0000001;
					CallWindowProc(WndProc, hWnd, WM_KEYUP, VK_F1,t );
					t=0xc0000001;
					CallWindowProc(WndProc, hWnd, WM_KEYUP, VK_SHIFT, t);
					return 0;
				}*/
				if (KeyboardEnabled && InGame() && !PopupOpened()) {
					if (!KeyboardSayMode){
						switch(wParam)
						{
							case 0x0D: // ENTER
								{
									KeyboardSayMode = true;
									return 0;
								}
							case 0x02:
							case 0x03:
							case 0x05:
							case 0x06:
							case 0x07:
							case 0x08:
							case 0x09:
							case 0x0B:
							case 0x0C:
							case 0x0E:
							case 0x0F:
							case 0x12:
							case 0x14:
							case 0x15:
							case 0x16:
							case 0x17:
							case 0x18:
							case 0x1A:
							case 0x1B:
								break;
							default:
								return 0;
						}
					}
				}
			}
			break;
		/*case WM_KEYUP:
			{
				if (wParam == 'q'){
					return 0;
				}
			}
			break;*/
		case WM_KEYDOWN:
			{
				/*
				if (wParam == 'q'){

					int t = 0x2A0001;
					CallWindowProc(WndProc, hWnd, WM_KEYDOWN, VK_SHIFT, t);
					t=0x3b0001;
					CallWindowProc(WndProc, hWnd, WM_KEYDOWN, VK_F1, t );
					t=0xc03b0001;
					CallWindowProc(WndProc, hWnd, WM_KEYUP, VK_F1,t );
					t=0xc02a0001;
					CallWindowProc(WndProc, hWnd, WM_KEYUP, VK_SHIFT, t);
					
					return 0;
				}*/
				if (wParam == VK_F1){
					char output[256];
					FILE *fp = fopen("c:/test.txt","a+");
					sprintf(output, "wParam=%x,lParam=%x\n", wParam, lParam);
					fprintf(fp, output);
					fclose(fp);
					break;
					//CallWindowProc(WndProc, hWnd, uMsg, wParam, lParam );
				}
				if (KeyboardEnabled && InGame() && !PopupOpened()){
					if (KeyboardSayMode){
						if (wParam == VK_ESCAPE){
							KeyboardSayMode = false;
							return 0;
						} else if(wParam == VK_RETURN) {
							KeyboardSayMode = false;
							break;
						}
					} else {
						for (i=0;i<KeyboardVKEntriesCount;i++){
							if (KeyboardVKEntries[i].VirtualKeyOriginalCode == wParam){
								wParam = KeyboardVKEntries[i].VirtualKeyNewCode;
								break;
							}
						}
					}
				}
			}
			break;
	}
    return CallWindowProc(WndProc, hWnd, uMsg, wParam, lParam );
}


void PipeOnRead(){
	int position=0;
	WORD len  = 0;
	len = Packet::ReadWord(Buffer, &position);
	BYTE PacketID = Packet::ReadByte(Buffer, &position);
	switch (PacketID){
		case 1: // Set Constant
			{	
				string ConstantName = Packet::ReadString(Buffer, &position);
				if (ConstantName == "ptrInGame"){
					Consts::INGAME = (const unsigned int*)Packet::ReadDWord(Buffer, &position);
				} else if (ConstantName == "ptrWASDPopup") {
					Consts::POPUP = (const unsigned int*)Packet::ReadDWord(Buffer, &position);
				} else if (ConstantName == "TibiaWindowHandle") {
					TibiaWindowHandle = (HWND)Packet::ReadDWord(Buffer, &position);
				} else if (ConstantName == "ptrBattlelistBegin") {
					Consts::ptrBattlelistBegin = (unsigned int*)Packet::ReadDWord(Buffer, &position);
				} else if (ConstantName == "ptrPrintName") {
					Consts::ptrPrintName = (DWORD)Packet::ReadDWord(Buffer, &position);
				} else if (ConstantName == "ptrPrintFPS") {
					Consts::ptrPrintFPS = (DWORD)Packet::ReadDWord(Buffer, &position);
				} else if (ConstantName == "ptrShowFPS") {
					Consts::ptrShowFPS = (DWORD)Packet::ReadDWord(Buffer, &position);
				} else if (ConstantName == "ptrPrintTextFunc") {
					//PrintText = (_PrintText*)Packet::ReadDWord(Buffer, &position);
					PrintText = (_PrintText*)0x4A3C00;
				} else if (ConstantName == "BLMax") {
					Consts::BLMax = (const int)Packet::ReadDWord(Buffer, &position);
				} else if (ConstantName == "BLDist") {
					Consts::BLDist = (const int)Packet::ReadDWord(Buffer, &position);
				} else if (ConstantName == "BLNameOffset") {
					Consts::BLNameOffset = (const int)Packet::ReadDWord(Buffer, &position);
				} else if (ConstantName == "BLLocationOffset") {
					Consts::BLLocationOffset = (const int)Packet::ReadDWord(Buffer, &position);
				} else if (ConstantName == "BLOnScreenOffset") {
					Consts::BLOnScreenOffset = (unsigned int)Packet::ReadDWord(Buffer, &position);
				} else if (ConstantName == "BLHPPercentOffset") {
					Consts::BLHPPercentOffset = (unsigned int)Packet::ReadDWord(Buffer, &position);
				} else if (ConstantName == "ptrCharX") {
					Consts::ptrCharX = (unsigned int*)Packet::ReadDWord(Buffer, &position);
				} else if (ConstantName == "ptrCharY") {
					Consts::ptrCharY = (unsigned int*)Packet::ReadDWord(Buffer, &position);
				} else if (ConstantName == "ptrCharZ") {
					Consts::ptrCharZ = (unsigned int*)Packet::ReadDWord(Buffer, &position);
				}
			}
			break;
		case 2: // Hook WND_PROC
			{
				BYTE Hook = Packet::ReadByte(Buffer, &position);
				if (Hook){
					WndProc = (WNDPROC)SetWindowLongPtr(TibiaWindowHandle, GWLP_WNDPROC, (LONG)WindowProc);
				} else {
					SetWindowLongPtr(TibiaWindowHandle, GWLP_WNDPROC, (LONG)WndProc);
				}
			}
			break;
		case 3: // Testing
			{
				Battlelist BL;
				BL.Reset();
				if (Battlelist::FindByName(&BL, "Fynn")) {
					PlayerText Creature;
					Creature.cB = 0x00;
					Creature.cG = 0x00;
					Creature.cR = 0xEE;
					Creature.CreatureId = (int)BL.ID();
					Creature.DisplayText = "PWNS";
					Creature.InUse = true;
					Creature.RelativeX = 0;
					Creature.RelativeY = -10;
					Creature.TextFont = 1;

					CreatureTexts.push_back(Creature);
				}
			}
			break;
		case 4: // DisplayText
			{
				int TextId = Packet::ReadByte(Buffer, &position);
				int PosX = Packet::ReadWord(Buffer, &position);
				int PosY = Packet::ReadWord(Buffer, &position);
				int ColorRed = Packet::ReadWord(Buffer, &position);
				int ColorGreen = Packet::ReadWord(Buffer, &position);
				int ColorBlue = Packet::ReadWord(Buffer, &position);
				int Font = Packet::ReadWord(Buffer, &position);
				string Text = Packet::ReadString(Buffer, &position);

				 char *lpText = (char*)Text.c_str();

				//lpText = (char*)malloc(Text.size()+1);
				strcpy(lpText, Text.c_str());

				SetText(TextId, true, PosX, PosY, ColorRed, ColorGreen, ColorBlue, Font, lpText);
			}
			break;
		case 5: //RemoveText
			{
				int TextId = Packet::ReadByte(Buffer, &position);
				if(TextId < MAX_TEXT)
					texts[TextId].used = false;
			}
			break;
		case 6: //Remove All
			{
				int i;
				for(i=0; i<MAX_TEXT; i++){
					texts[i].used = false;
				}
			}
			break;
		case 7: //Inject Display
			{
				/* Testing that every constant have a value */
				if(!Consts::ptrPrintFPS || !Consts::ptrPrintName || !Consts::ptrShowFPS) {
					MessageBoxA(0, "Error. All the constant doesn't contain a value", "Error", 0);
					break;
				}
				HookCall(Consts::ptrPrintName, (DWORD)&MyPrintName);
				HookCall(Consts::ptrPrintFPS, (DWORD)&MyPrintFps);
				Nop(0x44E68F, 6); //Showing the FPS all the time..
			}
			break;
		case 8: // Keyboard Enable/Disable
			{
				BYTE enabled = Packet::ReadByte(Buffer, &position);
				KeyboardEnabled = (enabled == 1);
				KeyboardSayMode = false;
			}
			break;
		case 9: // Keyboard Populate VK Entries
			{
				int entries = Packet::ReadDWord(Buffer, &position);
				if (KeyboardVKEntries){
					delete [] KeyboardVKEntries;
				}
				KeyboardVKEntries = new KeyboardVKEntry[entries];
				int i;
				for (i=0;i<entries;i++){
					KeyboardVKEntries[i].VirtualKeyOriginalCode = Packet::ReadByte(Buffer, &position);
					KeyboardVKEntries[i].VirtualKeyNewCode = Packet::ReadByte(Buffer, &position);
					KeyboardVKEntries[i].State = (KeyboardState)Packet::ReadByte(Buffer, &position);
				}
				KeyboardVKEntriesCount = entries;
			}
			break;
		case 0xA: //Set Text Above Player
			{
				int TextId = Packet::ReadByte(Buffer, &position);
				int PosX = Packet::ReadWord(Buffer, &position);
				int PosY = Packet::ReadWord(Buffer, &position);
				int ColorRed = Packet::ReadWord(Buffer, &position);
				int ColorGreen = Packet::ReadWord(Buffer, &position);
				int ColorBlue = Packet::ReadWord(Buffer, &position);
				int Font = Packet::ReadWord(Buffer, &position);
				string Text = Packet::ReadString(Buffer, &position);
			}
			break;
	}

}




void PipeThreadProc(HMODULE Module){
	DWORD br;
	if (WaitNamedPipeA(PipeName.c_str(), NMPWAIT_WAIT_FOREVER)) {
		PipeHandle = CreateFileA(PipeName.c_str(), GENERIC_READ | GENERIC_WRITE , 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
		PipeConnected = PipeHandle > 0;
		if (!PipeConnected){
			MessageBoxA(0, "Pipe connection failed!", "TibiaTekBot Injected DLL - Fatal Error", MB_ICONERROR);
			return;
		} else {
			do {
				EnterCriticalSection(&PipeReadCriticalSection);
				if (!ReadFile(PipeHandle, Buffer, 1024, &br, NULL))
					break;
				PipeOnRead();
				LeaveCriticalSection(&PipeReadCriticalSection);
			} while (true);
		}
	} else {
		MessageBoxA(0, "Failed waiting for pipe, maybe pipe is not ready?.", "TibiaTekBot Injected DLL - Fatal Error", 0);
	}
}




extern "C" bool APIENTRY DllMain (HMODULE hModule, DWORD reason, LPVOID reserved){
	switch (reason){
		case DLL_PROCESS_ATTACH:
        {
			if(!Consts::ptrPrintFPS || !Consts::ptrPrintName || !Consts::ptrShowFPS) {
					MessageBoxA(0, "Error. All the constant doesn't contain a value", "Error", 0);
					break;
				}
			HookCall(Consts::ptrPrintName, (DWORD)&MyPrintName);
			HookCall(Consts::ptrPrintFPS, (DWORD)&MyPrintFps);
			Nop(0x44E68F, 6); //Showing the FPS all the time..
			/*
			hMod = hModule;
			string CmdArgs = GetCommandLineA();
			int pos = CmdArgs.find("-pipe:");
			PipeName = "\\\\.\\pipe\\ttb" + CmdArgs.substr(pos + 6, 5);
			InitializeCriticalSection(&PipeReadCriticalSection);
			PipeConnected=false;
			PipeThread = CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)PipeThreadProc, hMod, NULL, NULL);*/
		}
        break;
		case DLL_PROCESS_DETACH:
		{
			TerminateThread(PipeThread, EXIT_SUCCESS);
			DeleteCriticalSection(&PipeReadCriticalSection);
		}
		break;
    }
    return true;
}
