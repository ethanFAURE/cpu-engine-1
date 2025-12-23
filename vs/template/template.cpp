#include "stdafx.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR, int cmdShow)
{
	if ( cpu::InitializeEngine() )
	{
		App application;
		application.Initialize(hInstance, 1024, 576);
		application.Run();
	}
	cpu::UninitializeEngine();
	return 0;
}
