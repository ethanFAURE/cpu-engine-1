#pragma once

class Game : public Engine
{
public:
	Game();
	virtual ~Game();

	void OnStart() override;
	void OnUpdate() override;
	void OnExit() override;
	void OnPreRender() override;
	void OnPostRender() override;

	static void MyPixelShader(PS_IO& io);

private:

	// Resources
	FONT m_font;
	MESH m_meshShip;
	MESH m_meshMissile;
	MESH m_meshSphere;
	TEXTURE m_texture;

	// UI
	SPRITE* m_pSprite;

	// Shader
	MATERIAL m_materialShip;
	MATERIAL m_materialMissile;

	// 3D
	ENTITY* m_pShip;
	std::list<ENTITY*> m_missiles;
	float m_missileSpeed;
	ENTITY* m_pBall;
};
