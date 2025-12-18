#include "stdafx.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR, int cmdShow)
{
	if ( InitializeEngine() )
	{
		App app;
		app.Initialize(hInstance, 320, 200, 1.0f, true);		// AMIGA NTSC
		//app.Initialize(hInstance, 512, 288, 1.0f, true);
		//app.Initialize(hInstance, 1024, 576);
		app.Run();
	}
	UninitializeEngine();
	return 0;
}
