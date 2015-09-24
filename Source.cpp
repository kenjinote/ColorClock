#pragma comment(lib,"dwmapi")
#include <windows.h>

TCHAR szClassName[] = TEXT("Window");

LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	static HFONT hFont;
	switch (msg)
	{
	case WM_CREATE:
		hFont = CreateFont(64, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, TEXT("Consolas"));
		SetTimer(hWnd, 0x1234, 1000, 0);
		break;
	case WM_TIMER:
		InvalidateRect(hWnd, 0, 1);
		break;
	case WM_ERASEBKGND:
	{
		SYSTEMTIME st;
		GetLocalTime(&st);
		const BYTE red = (BYTE)(st.wHour / 24.0 * 255.0);
		const BYTE green = (BYTE)(st.wMinute / 60.0 * 255.0);
		const BYTE blue = (BYTE)(st.wSecond / 60.0 * 255.0);
		RECT rect;
		GetClientRect(hWnd, &rect);
		const HFONT hOldFont = (HFONT)SelectObject((HDC)wParam, hFont);
		const COLORREF OldTextColor = SetTextColor((HDC)wParam, RGB(255, 255, 255));
		const COLORREF OldBkColor = SetBkColor((HDC)wParam, RGB(red, green, blue));
		TCHAR szText[7];
		wsprintf(szText, TEXT("%02X%02X%02X"), red, green, blue);
		SIZE size;
		GetTextExtentPoint32((HDC)wParam, szText, lstrlen(szText), &size);
		ExtTextOut((HDC)wParam, (rect.right - size.cx) / 2, (rect.bottom - size.cy) / 2, ETO_OPAQUE, &rect, szText, 6, 0);
		SetBkColor((HDC)wParam, OldTextColor);
		SetBkColor((HDC)wParam, OldBkColor);
		SelectObject((HDC)wParam, hOldFont);
	}
	return 1;
	case WM_DESTROY:
		KillTimer(hWnd, 0x1234);
		DeleteObject(hFont);
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, msg, wParam, lParam);
	}
	return 0;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPreInst, LPSTR pCmdLine, int nCmdShow)
{
	MSG msg;
	WNDCLASS wndclass = {
		0,
		WndProc,
		0,
		0,
		hInstance,
		0,
		LoadCursor(0, IDC_ARROW),
		0,
		0,
		szClassName
	};
	RegisterClass(&wndclass);
	HWND hWnd = CreateWindow(
		szClassName,
		TEXT("Color Clock"),
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,
		0,
		CW_USEDEFAULT,
		0,
		0,
		0,
		hInstance,
		0
		);
	ShowWindow(hWnd, SW_SHOWDEFAULT);
	UpdateWindow(hWnd);
	while (GetMessage(&msg, 0, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return msg.wParam;
}
