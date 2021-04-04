#pragma once

#include <LyreEngine.h>
#include <Core/WindowsWnd.h>

class WinApp : public Lyre::CApplication
{
public:
	WinApp(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow)
	{
		Lyre::CWindowsWnd* wnd = GetWindow<Lyre::CWindowsWnd>();
		wnd->SetProperties(
			{
				L"Lyre Engine",
				1280, 720,
				100, 50
			}
		);
		wnd->Init(hInstance, nCmdShow);
	}

};

