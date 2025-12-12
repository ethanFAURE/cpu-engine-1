#pragma once

class Keyboard
{
public:
	enum
	{
		_NONE,
		_DOWN,
		_UP,
		_PUSH,
	};

public:
	Keyboard();
	virtual ~Keyboard();

	bool IsKey(int key);
	bool IsKeyDown(int key);
	bool IsKeyUp(int key);
	void Update();

protected:
	byte m_keys[256];
};
