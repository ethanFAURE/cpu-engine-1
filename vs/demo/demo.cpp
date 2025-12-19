#include "stdafx.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR, int cmdShow)
{
	if ( InitializeEngine() )
	{
		App application;
		
		// AMIGA
		//application.Initialize(hInstance, 320, 200, 1.0f, true);		// AMIGA NTSC
		//application.Initialize(hInstance, 320, 256, 1.0f, true);		// AMIGA PAL

		// RETRO
		application.Initialize(hInstance, 512, 288, 1.0f, true);		// fullscreen

		// MODERN
		//application.Initialize(hInstance, 1024, 576);

		// FULL HD (please use release)
		//application.Initialize(hInstance, 1920, 1080);

		application.Run();
	}
	UninitializeEngine();
	return 0;
}
