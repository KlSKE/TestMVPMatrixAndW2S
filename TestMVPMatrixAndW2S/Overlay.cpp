#include "pch.h"
#include "Overlay.h"

void OverlayFunctions::CreateClass(WNDPROC winProc, char* windowName, HINSTANCE hInstance) {
	sprintf_s(Overlay.Name, "%s", windowName);

	Overlay.Class.cbClsExtra = 0;
	Overlay.Class.cbSize = sizeof(WNDCLASSEX);
	Overlay.Class.cbWndExtra = 0;
	Overlay.Class.hbrBackground = (HBRUSH)CreateSolidBrush(RGB(0, 0, 0));
	Overlay.Class.hCursor = LoadCursor(0, IDC_ARROW);
	Overlay.Class.hIcon = LoadIcon(0, IDI_APPLICATION);
	Overlay.Class.hIconSm = LoadIcon(0, IDI_APPLICATION);
	Overlay.Class.hInstance = hInstance;
	Overlay.Class.lpfnWndProc = winProc;
	Overlay.Class.lpszClassName = Overlay.Name;
	Overlay.Class.lpszMenuName = Overlay.Name;
	Overlay.Class.style = CS_HREDRAW | CS_VREDRAW;

	if(!RegisterClassEx(&Overlay.Class)) {
		exit(1);
	}
}

void OverlayFunctions::GetTargetWindow(LPCSTR windowName) {
	Target.Window = FindWindow(0, windowName);

	if(Target.Window) {
		Target.Checked = true;
		GetWindowRect(Target.Window, &Target.Size);

		Overlay.Width = Target.Size.right - Target.Size.left;
		Overlay.Height = Target.Size.bottom - Target.Size.top;

		Target.Style = GetWindowLong(Target.Window, GWL_STYLE);

		if(Target.Style & WS_BORDER) {
			Target.Size.top += 23;
			Overlay.Height -= 23;
		}

		MoveWindow(Overlay.Window, Target.Size.left, Target.Size.top, Overlay.Width, Overlay.Height, true);
		Overlay.Margin = {0, 0, Overlay.Width, Overlay.Height};
	}
}

void OverlayFunctions::CreateWindowOverlay(HINSTANCE hInstance) {
	Overlay.Window = CreateWindowEx(WS_EX_TOPMOST | WS_EX_LAYERED | WS_EX_TRANSPARENT, Overlay.Name, Overlay.Name, WS_POPUP, 1, 1, Overlay.Width, Overlay.Height, 0, 0, hInstance, 0);

	SetLayeredWindowAttributes(Overlay.Window, RGB(0, 0, 0), 255, LWA_COLORKEY | LWA_ALPHA);
	ShowWindow(Overlay.Window, SW_SHOW);
	DwmExtendFrameIntoClientArea(Overlay.Window, &Overlay.Margin);
}