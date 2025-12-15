#include "stdafx.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR, int cmdShow)
{
	App app;
	app.Initialize(hInstance, 1024, 576);
	app.Run();
	app.Uninitialize();
	return 0;
}
