#pragma once

class Engine
{
public:
	friend ThreadJob;

public:
	Engine();
	virtual ~Engine();
	static Engine* Instance();

	void Initialize(HINSTANCE hInstance, int renderWidth, int renderHeight);
	void Uninitialize();
	void Run();
	void FixWindow();
	void FixProjection();

	ENTITY* CreateEntity();
	void ReleaseEntity(ENTITY* pEntity);

	void GetCursor(DirectX::XMFLOAT2& pt);
	RAY ToRay(DirectX::XMFLOAT2& pt);

	static DirectX::XMFLOAT3 ApplyLighting(DirectX::XMFLOAT3& color, float intensity);
	static ui32 ToBGR(DirectX::XMFLOAT3& color);
	static DirectX::XMFLOAT3 ToColor(int r, int g, int b);
	static float Clamp(float v);
	static float Clamp(float v, float min, float max);
	static int Clamp(int v, int min, int max);

	static void CreateSpaceship(MESH& mesh);
	static void CreateCube(MESH& mesh);
	static void CreateCircle(MESH& mesh, float radius, int count = 6);

protected:
	bool Time();
	void Update();
	void Update_Physics();
	void Update_Purge();
	void Render();
	void Render_View();
	void Render_Sort();
	void Render_Box();
	void Render_Tile();
	void Render_Entity(int iTile);

	void Clear(DirectX::XMFLOAT3& color);
	void DrawSky();
	void Present();
	void DrawLine(int x0, int y0, float z0, int x1, int y1, float z1, DirectX::XMFLOAT3& color);
	void FillTriangle(DirectX::XMFLOAT3& v1, DirectX::XMFLOAT3& v2, DirectX::XMFLOAT3& v3, DirectX::XMFLOAT3& c1, DirectX::XMFLOAT3& c2, DirectX::XMFLOAT3& c3, TILE& tile);
	void Draw(ENTITY* pEntity, TILE& tile);

	virtual void OnStart() {}
	virtual void OnUpdate() {}
	virtual void OnExit() {}
	virtual void OnPreRender() {}
	virtual void OnPostRender() {}

	static LRESULT WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
	LRESULT OnWindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

private:
	static Engine* s_pEngine;

protected:
	// Thread
	int m_threadCount;
	ThreadJob* m_threads;
	std::atomic<int> m_nextTile;

	// Window
	HINSTANCE m_hInstance;
	HWND m_hWnd;
	int m_windowWidth;
	int m_windowHeight;

	// Surface
#ifdef GPU_PRESENT
	ID2D1Factory* m_pD2DFactory;
	ID2D1HwndRenderTarget* m_pRenderTarget;
	ID2D1Bitmap* m_pBitmap;
#else
	HDC m_hDC;
	BITMAPINFO m_bi;
#endif

	// Controller
	Keyboard m_keyboard;

	// Buffer
	int m_renderWidth;
	int m_renderHeight;
	int m_renderPixelCount;
	float m_renderWidthHalf;
	float m_renderHeightHalf;
	std::vector<uint32_t> m_colorBuffer;
	std::vector<float> m_depthBuffer;

	// Tile
	int m_tileWidth;
	int m_tileHeight;
	int m_tileRowCount;
	int m_tileColCount;
	int m_tileCount;
	std::vector<TILE> m_tiles;

	// Color
	bool m_sky;
	DirectX::XMFLOAT3 m_clearColor;
	DirectX::XMFLOAT3 m_groundColor;
	DirectX::XMFLOAT3 m_skyColor;

	// Light
	DirectX::XMFLOAT3 m_lightDir;
	float m_ambientLight;
	
	// Time
	DWORD m_systime;
	float m_time;
	float m_dt;
	DWORD m_fpsTime;
	int m_fpsCount;
	int m_fps;

	// Camera
	DirectX::XMFLOAT4X4 m_matProj;
	DirectX::XMFLOAT4X4 m_matView;
	DirectX::XMFLOAT4X4 m_matViewProj;
	TRANSFORM m_camera;

	// Entity
	std::vector<ENTITY*> m_entities;
	std::vector<ENTITY*> m_sortedEntities;
	int m_entityCount;
	std::vector<ENTITY*> m_bornEntities;
	int m_bornEntityCount;
	std::vector<ENTITY*> m_deadEntities;
	int m_deadEntityCount;
};
