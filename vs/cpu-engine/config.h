#pragma once

// Enable CONFIG_GPU for improved stretching when window size != render size or if you want to use V-Sync
//#define CONFIG_GPU

// Enable CONFIG_MT if you want to use Multi-Threading
#ifdef _DEBUG
	//#define CONFIG_MT
#else
	#define CONFIG_MT
#endif

// Enable CONFIG_MT_DEBUG if you want to check Multi-Threading
#ifdef CONFIG_MT
	//#define CONFIG_MT_DEBUG
#endif

// Enable CONFIG_WIREFRAME if you want to draw edges for debugging
//#define CONFIG_WIREFRAME
