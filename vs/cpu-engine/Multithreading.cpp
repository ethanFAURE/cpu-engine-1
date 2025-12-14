#include "stdafx.h"

void ThreadJob::OnCallback()
{
	Engine* pEngine = Engine::Instance();
	m_quitRequest = false;

	// Thread
	while ( true )
	{
		// Waiting next job
		WaitForSingleObject(m_hEventStart, INFINITE);
		if ( m_quitRequest )
			break;

		// Job
		while ( true )
		{
			int index = pEngine->m_nextTile.fetch_add(1, std::memory_order_relaxed);
			if ( index>=m_count )
				break;

			pEngine->Render_Tile(index);

#ifdef CONFIG_MT_DEBUG
			pEngine->Present();
			Sleep(100);
#endif
		}

		// End
		SetEvent(m_hEventEnd);
	}
}
