#pragma once

namespace Input
{
	void Init();
	void Update();

	double GetMouseX();
	double GetMouseY();
	bool KeyPressed(char c);
	bool KeyDown(char c);
	bool KeyDownLastFrame(char c);
	void CenterMouse();
	bool LeftMouseDown();
	bool RightMouseDown();
	bool LeftMousePressed();
	bool RightMousePressed();

	void DisableCursor();
	void HideCursor();
	void ShowCursor();
}
