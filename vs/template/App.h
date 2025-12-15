#pragma once

class App : public cpu_engine
{
public:
	App();
	virtual ~App();

	static App* GetInstance() { return s_pApp; }

	void OnStart() override;
	void OnUpdate() override;
	void OnExit() override;
	void OnPreRender() override;
	void OnPostRender() override;

	static void MyPixelShader(cpu_ps_io& io);

private:
	inline static App* s_pApp = nullptr;
};
