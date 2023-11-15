#include <Windows.h>
#include <tchar.h>
#include <iostream>
#include <string>

#include <thread>
#include <chrono>
using namespace std;

#pragma comment(linker,"\"/manifestdependency:type='win32' \
name='Microsoft.Windows.Common-Controls' version='6.0.0.0' \
processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")

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

	HWND hWindow = CreateWindow(L"MainWndClass", L"LR2_SecondProgram", WS_OVERLAPPEDWINDOW
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

	static HWND hEdit1;
	static HWND hEdit2;
	static HWND hEdit3;
	static HWND hEdit4;
	static HWND hEdit5;

	static HWND hStatic1;
	static HWND hStatic2;
	static HWND hStatic3;

	static int color_area;

	HWND h = FindWindow(L"MainWndClass", L"LR2_FirstProgram");

	switch (msg)
	{
	case WM_CREATE:
	{
		HWND hBtn1 = CreateWindow(
			L"Button",
			L"Запуск программы 1",
			WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
			0, 0, 150, 30,
			hWnd, reinterpret_cast<HMENU>(1), NULL, NULL
		);
		HWND hBtn2 = CreateWindow(
			L"Button",
			L"В ПР1 Текст",
			WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
			150, 95, 150, 30,
			hWnd, reinterpret_cast<HMENU>(2), NULL, NULL
		);
		HWND hBtn3 = CreateWindow(
			L"Button",
			L"В ПР1 Шрифт",
			WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
			150, 45, 150, 30,
			hWnd, reinterpret_cast<HMENU>(3), NULL, NULL
		);
		HWND hBtn4 = CreateWindow(
			L"Button",
			L"В ПР1 Цвет",
			WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
			150, 145, 150, 30,
			hWnd, reinterpret_cast<HMENU>(4), NULL, NULL
		);

		hEdit1 = CreateWindow(
			L"eDit", L"45", WS_CHILD | WS_VISIBLE | WS_BORDER | ES_LEFT, 0, 50, 150, 20, hWnd, NULL, NULL, NULL);
		CreateWindow(
			L"static", L"Размер шрифта", WS_CHILD | WS_VISIBLE, 0, 30, 150, 20, hWnd, NULL, NULL, NULL);

		hEdit2 = CreateWindow(
			L"eDit", L"Hello world!!!", WS_CHILD | WS_VISIBLE | WS_BORDER | ES_LEFT, 0, 100, 150, 20, hWnd, NULL, NULL, NULL);
		CreateWindow(
			L"static", L"Текст", WS_CHILD | WS_VISIBLE, 0, 80, 150, 20, hWnd, NULL, NULL, NULL);

		hEdit3 = CreateWindow(
			L"eDit", L"1", WS_CHILD | WS_VISIBLE | WS_BORDER | ES_LEFT, 0, 150, 150, 20, hWnd, NULL, NULL, NULL);
		CreateWindow(
			L"static", L"Номер цвета шрифта", WS_CHILD | WS_VISIBLE, 0, 130, 150, 20, hWnd, NULL, NULL, NULL);

		// Дубль параметров
		CreateWindow(
			L"static", L"Параметры", WS_CHILD | WS_VISIBLE | ES_CENTER, 0, 180, 150, 20, hWnd, NULL, NULL, NULL);
		hStatic1 = CreateWindow(
			L"static", L"45", WS_CHILD | WS_VISIBLE | WS_BORDER |  ES_CENTER, 0, 180, 150, 20, hWnd, NULL, NULL, NULL);
		hStatic2 = CreateWindow(
			L"static", L"Hello world!!!", WS_CHILD | WS_VISIBLE | WS_BORDER | ES_CENTER, 0, 200, 150, 20, hWnd, NULL, NULL, NULL);
		hStatic3 = CreateWindow(
			L"static", L"1", WS_CHILD | WS_VISIBLE | WS_BORDER | ES_CENTER, 0, 220, 150, 20, hWnd, NULL, NULL, NULL);

		break;
	}
	case WM_COMMAND:
	{
		switch (LOWORD(wp))
		{
		case 1:	// Запуск программы ПР1
		{
			char param1[15] = "";
			char param2[32] = "";
			char param3[15] = "";
			GetWindowTextA(hEdit1, param1, 14);
			SetWindowTextA(hStatic1, param1);

			GetWindowTextA(hEdit2, param2, 31);
			SetWindowTextA(hStatic2, param2);

			GetWindowTextA(hEdit3, param3, 14);
			SetWindowTextA(hStatic3, param3);
			string launch_Prog1 = "start";
			string path = "D:\\Мои_программы\\ЛР2_1\\Debug\\ЛР2_1_Prog1.exe";

			system((launch_Prog1 + " " + path + " " + param1 + " \"" + param2 + "\" " + param3).c_str());
			//system("start D:\\Мои_программы\\ЛР2_1\\Debug\\ЛР2_1_Prog1.exe 15");
			//MessageBoxA(NULL, (launch_Prog1 + " " + path + " " + param1).c_str(), "App", MB_OK);
			break;
		}
		case 2:	//Текст в ПР1
		{
			char buf[32];
			GetWindowTextA(hEdit2, buf, 31);
			SetWindowTextA(hStatic2, buf);

			// define shared data
			char* shared_file_name = (char*)"my_shared_memory";
			unsigned long buff_size = 4096;
			//char share_buffer[] = "greetings, hello world";
			//MyData share_buffer("Tom", 18);


			// create shared memory file
			HANDLE dump_file_descriptor = CreateFile((LPCWSTR)shared_file_name,
				GENERIC_READ | GENERIC_WRITE,
				FILE_SHARE_READ | FILE_SHARE_WRITE,
				NULL,
				OPEN_ALWAYS, // open exist or create new, overwrite file
				FILE_ATTRIBUTE_NORMAL,
				NULL);

			if (dump_file_descriptor == INVALID_HANDLE_VALUE)
				cout << "create file error" << endl;

			HANDLE shared_file_handler = CreateFileMapping(
				dump_file_descriptor, // Use paging file - shared memory
				NULL,                 // Default security attributes
				PAGE_READWRITE,       // Allow read and write access
				0,                    // High-order DWORD of file mapping max size
				buff_size,            // Low-order DWORD of file mapping max size
				(LPCWSTR)shared_file_name);    // Name of the file mapping object

			if (shared_file_handler)
			{
				// map memory file view, get pointer to the shared memory
				LPVOID lp_base = MapViewOfFile(
					shared_file_handler,  // Handle of the map object
					FILE_MAP_ALL_ACCESS,  // Read and write access
					0,                    // High-order DWORD of the file offset
					0,                    // Low-order DWORD of the file offset
					buff_size);           // The number of bytes to map to view


				// copy data to shared memory
				memcpy(lp_base, &buf, sizeof(buf));

				FlushViewOfFile(lp_base, buff_size); // can choose save to file or not

				HWND h = FindWindow(L"MainWndClass", L"LR2_FirstProgram");
				SendMessage(h, WM_USER + 5, 0, 0);

				// process wait here for other task to read data
				//cout << "already write to shared memory, wait ..." << endl;
				//this_thread::sleep_for(chrono::seconds(0.5));

				// close shared memory file
				UnmapViewOfFile(lp_base);
				CloseHandle(shared_file_handler);
				CloseHandle(dump_file_descriptor);
				//unlink(shared_file_name);
			}
			break;
		}
		case 3:	// Шрифт ПР1
		{
			char param1[15] = "";
			GetWindowTextA(hEdit1, param1, 14);
			SetWindowTextA(hStatic1, param1);
			int intPar1 = atoi(param1);
			//HWND h = FindWindow(L"MainWndClass", L"LR2_FirstProgram");
			SendMessage(h, WM_USER + 2, intPar1, lp);
			break;
		}
		case 4: // Цвет ПР1
		{
			char param3[15] = "";
			GetWindowTextA(hEdit3, param3, 14);
			SetWindowTextA(hStatic3, param3);
			int intPar3 = atoi(param3);
			//HWND h = FindWindow(L"MainWndClass", L"LR2_FirstProgram");
			SendMessage(h, WM_USER + 4, intPar3, lp);
			break;
		}
		
		}
		break;
	}
	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	case WM_PAINT:
	{
		hDeviceContext = BeginPaint(hWnd, &paintStruct);
		GetClientRect(hWnd, &rectPlace);
		RECT recta1 = { 300, 50, 500, 50 + 20 };	// стр-ра, определяющая размер клиентской области
		RECT recta2 = { 300, 100, 550, 100 + 20 };	// стр-ра, определяющая размер клиентской области
		RECT recta3 = { 300, 150, 500, 150 + 20 };	// стр-ра, определяющая размер клиентской области


		hFont = CreateFont(20, 0, 0, 0, 0, 0, 0, 0, DEFAULT_CHARSET, 0, 0, 0, 0, L"Courier New");
		SelectObject(hDeviceContext, hFont);

		DrawTextA(hDeviceContext, "SendMessage", 11, &recta1, DT_SINGLELINE | DT_LEFT | DT_VCENTER);
		DrawTextA(hDeviceContext, "Распределяемая память", 21, &recta2, DT_SINGLELINE | DT_LEFT | DT_VCENTER);
		DrawTextA(hDeviceContext, "SendMessage", 11, &recta3, DT_SINGLELINE | DT_LEFT | DT_VCENTER);

		EndPaint(hWnd, &paintStruct);
		break;
	}

	case WM_KEYDOWN:
		switch (wp)
		{
		case VK_LEFT:
			//HWND h = FindWindow(L"MainWndClass", L"LR2_FirstProgram");
			SendMessage(h, WM_KEYDOWN, VK_UP, 0);
			MessageBoxA(NULL, "VK_LEFT", "VK_LEFT", MB_OK);
			break;
		case VK_RIGHT:
			//HWND h = FindWindow(L"MainWndClass", L"LR2_FirstProgram");
			SendMessage(h, WM_KEYDOWN, VK_DOWN, 0);
			MessageBoxA(NULL, "VK_RIGHT", "VK_RIGHT", MB_OK);
			break;

		default:
			break;
		}
		break;

	default:
		return DefWindowProc(hWnd, msg, wp, lp);
		break;
	}
	return NULL;
}