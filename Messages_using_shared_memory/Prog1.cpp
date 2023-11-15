#include <Windows.h>
#include <tchar.h>
#include <iostream>
#include <string>
#include <shellapi.h>
using namespace std;

//HFONT hFont = NULL;

LRESULT CALLBACK SoftWareMainProcedure(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp);
WNDCLASS NewWindowClass(HBRUSH BGColor,
	HCURSOR Cursor,
	HINSTANCE hInst,
	HICON Icon,
	LPCWSTR Name,
	WNDPROC Procedure);

int WINAPI WinMain(
	_In_ HINSTANCE hInst,
	_In_opt_ HINSTANCE hPrevInst,
	_In_ LPSTR     args,
	_In_ int       ncmdshow
)
{
	WNDCLASS SoftwareMainClass = NewWindowClass((HBRUSH)COLOR_WINDOW,
		LoadCursor(NULL, IDC_ARROW),
		hInst, LoadIcon(NULL, IDI_QUESTION),
		L"MainWndClass", SoftWareMainProcedure);

	if (!RegisterClassW(&SoftwareMainClass)) { return -1; }

	MSG SoftwareMainMessage = { 0 };

	HWND hWindow = CreateWindow(L"MainWndClass", L"LR2_FirstProgram", WS_OVERLAPPEDWINDOW
		| WS_VISIBLE, 100, 100, 800, 400, NULL, NULL, NULL, NULL);

	if (!hWindow) {
		MessageBox(NULL, L"Не получилось создать окно!", L"Ошибка", MB_OK);
		return NULL;
	}

	while (GetMessage(&SoftwareMainMessage, NULL, NULL, NULL))
	{
		TranslateMessage(&SoftwareMainMessage);
		DispatchMessage(&SoftwareMainMessage);
	}


	return 0;
}

WNDCLASS NewWindowClass(HBRUSH BGColor,
	HCURSOR Cursor,
	HINSTANCE hInst,
	HICON Icon,
	LPCWSTR Name,
	WNDPROC Procedure)
{
	WNDCLASS NWC = { 0 };

	NWC.hCursor = Cursor;
	NWC.hIcon = Icon;
	NWC.hInstance = hInst;
	NWC.lpszClassName = Name;
	NWC.hbrBackground = BGColor;
	NWC.lpfnWndProc = Procedure;

	return NWC;
};

LRESULT CALLBACK SoftWareMainProcedure(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp)
{
	HDC hDeviceContext;
	PAINTSTRUCT paintStruct;
	RECT rectPlace;
	HFONT hFont;
	HBRUSH greenBrush;

	INT nArgs = 0;
	LPWSTR lpCommandLine = GetCommandLine();
	LPWSTR* lpArgs = CommandLineToArgvW(lpCommandLine, &nArgs);

	int lenght_message = 15;
	static int size_font = 45;
	static char param2[32]  = "Hello\0";
	static int len;
	static int param3 = 0;
	//COLOREF Green = ;

	string message;
	string size_font_string;

	static HWND hStatic1;
	static HWND hStatic2;
	static HWND hStatic3;

	static LPCWSTR fontName = NULL;

	//// Приём сообщений
	//if (msg == WM_USER + 2) {
	//	size_font = wp;

	//	MessageBoxA(hWnd, "Команда пришла", to_string(wp).c_str(), MB_OK);
	//}
	////if (msg == WM_USER + 3) param2 = wp;
	//if (msg == WM_USER + 4) param3 = wp;

		

	// Действия
	switch (msg)
	{
	case WM_CREATE:
		if (nArgs >= 2) {
			CreateWindow(
				L"static", L"Параметры:", WS_CHILD | WS_VISIBLE | WS_BORDER, 0, 10, 200, 20, hWnd, NULL, NULL, NULL);

			size_t i;
			if (nArgs >= 2) {
				size_font = _wtoi(lpArgs[1]);
				hStatic1 = CreateWindowA(
					"static", to_string(size_font).c_str(), WS_CHILD | WS_VISIBLE | WS_BORDER | ES_RIGHT, 0, 10 + 20, 200, 20, hWnd, NULL, NULL, NULL);
			}
			if (nArgs >= 3)
			{
				wcstombs_s(&i, param2, 32, lpArgs[2], wcslen(lpArgs[2]));
				len = wcslen(lpArgs[2]);
				param2[len] = '\0';
				hStatic2 = CreateWindowA(
					"static", param2, WS_CHILD | WS_VISIBLE | WS_BORDER | ES_RIGHT, 0, 10 + 40, 200, 20, hWnd, NULL, NULL, NULL);
			}
			if (nArgs >= 4) {
				param3 = _wtoi(lpArgs[3]);
				hStatic3 = CreateWindowA(
					"static", to_string(param3).c_str(), WS_CHILD | WS_VISIBLE | WS_BORDER | ES_RIGHT, 0, 10 + 60, 200, 20, hWnd, NULL, NULL, NULL);

			}
		}

		//hFont = CreateFont(size_font, 0, 0, 0, 0, 0, 0, 0, DEFAULT_CHARSET, 0, 0, 0, 0, fontName);

		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	case WM_PAINT:
		hDeviceContext = BeginPaint(hWnd, &paintStruct);

		GetClientRect(hWnd, &rectPlace);
		
		switch (param3)
		{
		case 1: SetTextColor(hDeviceContext, RGB(0, 0, 255));		break;
		case 2: SetTextColor(hDeviceContext, RGB(0, 255, 0));		break;
		case 3: SetTextColor(hDeviceContext, RGB(255, 0, 0));		break;
		case 4: SetTextColor(hDeviceContext, RGB(115, 50, 130));	break;
		case 5: SetTextColor(hDeviceContext, RGB(175, 0, 255));		break;
		default: SetTextColor(hDeviceContext, RGB(0, 0, 0));		break;
			break;
		}

		hFont = CreateFont(size_font, 0, 0, 0, 0, 0, 0, 0, DEFAULT_CHARSET, 0, 0, 0, 0, fontName);
		SelectObject(hDeviceContext, hFont);

		lenght_message = strlen(param2);
		DrawTextA(hDeviceContext, param2, lenght_message, &rectPlace, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
		EndPaint(hWnd, &paintStruct);
	
		break;
	case WM_KEYDOWN:
		switch (wp)
		{
		case VK_UP:
			size_font = size_font + 5;
			size_font_string = to_string(size_font);

			//if (hFont) DeleteObject(hFont);
			//hFont = CreateFont(size_font, 0, 0, 0, 0, 0, 0, 0, DEFAULT_CHARSET, 0, 0, 0, 0, L"Mason Chronicles");

			fontName = L"Mason Chronicles";

			MessageBoxA(NULL, (LPCSTR)size_font_string.c_str(), "Размер изменён", MB_OK);
			InvalidateRect(hWnd, NULL, TRUE);
			UpdateWindow(hWnd);
			break;
		case VK_DOWN:
			size_font = size_font - 5;
			size_font_string = to_string(size_font);

			//if (hFont) DeleteObject(hFont);
			//hFont = CreateFont(size_font, 0, 0, 0, 0, 0, 0, 0, DEFAULT_CHARSET, 0, 0, 0, 0, L"Arial");

			fontName = L"Arial";

			MessageBoxA(NULL, (LPCSTR)size_font_string.c_str(), "Размер изменён", MB_OK);
			InvalidateRect(hWnd, NULL, TRUE);
			UpdateWindow(hWnd);
			break;

		default:

			break;
		}
		break;
	case  WM_USER + 2:
		size_font = wp;
		//MessageBoxA(hWnd, "Пришёл параметр 1", "Параметр 1", MB_OK);
		SetWindowTextA(hStatic1, to_string(size_font).c_str());
		InvalidateRect(hWnd, NULL, TRUE);
		break;
	//case  WM_USER + 3:
	//{//param2 = lp;
	//	char* temp = (char*)lp;
	//	//MessageBoxA(hWnd, lp, L"Параметр 2", MB_OK);
	//	SetWindowTextA(hStatic2, temp);
	//	InvalidateRect(hWnd, NULL, TRUE);
	//	break;
	//}
	case  WM_USER + 4:
		param3 = wp;
		//MessageBoxA(hWnd, "Пришёл параметр 3", "Параметр 3", MB_OK);
		SetWindowTextA(hStatic3, to_string(param3).c_str());
		InvalidateRect(hWnd, NULL, TRUE);
		break;
	case  WM_USER + 5:
	{
		//MessageBoxA(hWnd, " Я тут", "Разделяемая память", MB_OK);
		char* shared_file_name = (char*)"my_shared_memory";

		// open shared memory file
		HANDLE shared_file_handler = OpenFileMapping(
			FILE_MAP_ALL_ACCESS,
			NULL,
			(LPCWSTR)shared_file_name);

		if (shared_file_handler)
		{
			LPVOID lp_base = MapViewOfFile(
				shared_file_handler,
				FILE_MAP_ALL_ACCESS,
				0,
				0,
				0);

			// copy shared data from memory
			const unsigned long buff_size = 4096;
			char* share_buffer = (char*)lp_base;
			strcpy_s(param2, (char*)lp_base);

			//MessageBoxA(hWnd, share_buffer, "Разделяемая память", MB_OK);
			//MessageBoxA(hWnd, param2, "Разделяемая память", MB_OK);

			// close share memory file
			UnmapViewOfFile(lp_base);
			CloseHandle(shared_file_handler);
			//param2 = *share_buffer;
			InvalidateRect(hWnd, NULL, TRUE);
		}

		SetWindowTextA(hStatic2, param2);
		break;
	}

	default:
		return DefWindowProc(hWnd, msg, wp, lp);
		break;
	}
	return NULL;
}