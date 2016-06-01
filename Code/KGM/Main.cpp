#include <Windows.h>
#include "Globals.h"
#include "CKGM.h"

int WINAPI					WinMain(HINSTANCE hInstance, HINSTANCE hPreviousInstance, LPSTR pCommandLine, int iCommandShow)
{
	g_pKGM = new CKGM;
	g_pKGM->init();
	g_pKGM->openWindow();
	g_pKGM->processWindows();
	g_pKGM->uninit();
	return 0;
}