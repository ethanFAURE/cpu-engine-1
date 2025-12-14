#include "stdafx.h"

MATERIAL::MATERIAL()
{
#ifdef _DEBUG
	lighting = GOURAUD;
#else
	lighting = LAMBERT;
#endif

	ps = nullptr;
	color = WHITE;
	values = nullptr;
}
