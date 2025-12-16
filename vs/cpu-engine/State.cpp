#include "stdafx.h"

cpu_fsm_base::cpu_fsm_base()
{
	dead = false;
	index = -1;
	sortedIndex = -1;

	state = -1;
	pending = -1;
	globalTotalTime = 0.0f;
	totalTime = 0.0f;
}

cpu_fsm_base::~cpu_fsm_base()
{
}

void cpu_fsm_base::ToState(int to)
{
	pending = to;
}
