/*
cl main.cpp kernel32.lib gdi32.lib user32.lib ole32.lib d2d1.lib dwrite.lib /EHsc /DEBUG /Z7
cl main.cpp kernel32.lib gdi32.lib user32.lib ole32.lib d2d1.lib dwrite.lib /EHsc 
*/

//Window Header Files
#include <windows.h>

//C Runtime Header Files
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <wchar.h>
#define _USE_MATH_DEFINE
#include <cmath>
#include <cstdio>
#include <fcntl.h>
#include <io.h>

//C++ Runetime Header Files
#include <string>
#include <vector>
#include <iostream>
#include <map>

//Direct2d headers
#include <d2d1.h>
#include <d2d1helper.h>
#include <dwrite.h>
#include <wincodec.h>

float cameraOffsetX = 0;
float cameraOffsetY = 0;

//Game headers
#include "colours.cpp"
#include "font.cpp"
#include "geometry.cpp"
#include "particles.cpp"
#include "stars.cpp"
#include "entities.cpp"
#include "game.cpp"

//Declaring additional functions for releasing interfaces
template<class Interface>
inline void SafeRelease(Interface** ppInterfaceToRelease)
{
	if(*ppInterfaceToRelease != NULL)
	{
		(*ppInterfaceToRelease)->Release();
		(*ppInterfaceToRelease) = NULL;
	}
}

//Additional Macros to use later
#ifndef Assert
#if defined( DEBUG ) || defined( _DEBUG )
#define Assert(b) do {if (!(b)){OutputDebugStringA("Assert: " #b "\n");}} while(0)
#else
#define Assert(b)
#endif //DEBUG || _DEBUG
#endif

#ifndef HINST_THISCOMPONENT
EXTERN_C IMAGE_DOS_HEADER __ImageBase;
#define HINST_THISCOMPONENT ((HINSTANCE)&__ImageBase)
#endif

int64_t counts_per_sec = 0;
float sec_per_count = 0;
int64_t prev_time = 0; 
float delta = 0;
int64_t current_time = 0;

//CONST GLOBALS
CONST INT ASTROID_MAX_COUNT = 1;

//Declaring methods for initalising the class, creating and discarding resources, 
//handling the message loop, rendering content and the window procedure
class DemoApp
{
public:
	//Creation and destruction operators.
	DemoApp();
	~DemoApp();

	//Register the window class and call methods for instantiating drawing resources
	HRESULT Initialize();

	//Process and dispatch messages
	void RunMessageLoop();

	//Check keyboard inputs for events
	void RunEventLoop();

	//Logic update loop
	static void UpdateLoop();

private:
	//Initialize device-independent resources
	HRESULT CreateDeviceIndependentResources();

	//Initialize device-dependent resources
	HRESULT CreateDeviceResources();

	//Release device-dependent resources
	void DiscardDeviceResources();

	//Draw Content
	HRESULT OnRender();

	//Resize the render target.
	void OnResize(UINT width, UINT height);

	//The windows procedure
	static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

	HWND m_hwnd;
	ID2D1Factory* m_pDirect2dFactory;
	ID2D1HwndRenderTarget* m_pRenderTarget;
};

float addition = 0;

//Default Constructor
DemoApp::DemoApp() :
	m_hwnd(NULL),
	m_pDirect2dFactory(NULL),
	m_pRenderTarget(NULL)
{}

//Default Destructor
DemoApp::~DemoApp()
{
	SafeRelease(&m_pDirect2dFactory);
	SafeRelease(&m_pRenderTarget);
}

//Message handler
void DemoApp::RunMessageLoop()
{
	MSG msg;

	while(GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);

		InvalidateRect(m_hwnd, NULL, false);
	}
}

//Event handler
void DemoApp::RunEventLoop()
{
	printf("Test\n");
} 

//
HRESULT DemoApp::Initialize()
{
	HRESULT hr;

	//Initialize device-independent resrouces, such as Direct2D Factory
	hr = CreateDeviceIndependentResources();

	if(SUCCEEDED(hr))
	{
		//Register the window class
		WNDCLASSEX wcex = {sizeof(WNDCLASSEX)};
		wcex.style = CS_HREDRAW | CS_VREDRAW;
		wcex.lpfnWndProc = DemoApp::WndProc;
		wcex.cbClsExtra = 0;
		wcex.cbWndExtra = sizeof(LONG_PTR);
		wcex.hInstance = HINST_THISCOMPONENT;
		wcex.hbrBackground = NULL;
		wcex.lpszMenuName = NULL;
		wcex.hCursor = LoadCursor(NULL, IDI_APPLICATION);
		wcex.lpszClassName = "D2DDemoApp";

		RegisterClassEx(&wcex);

		//In terms of using correct DPI, to create a window at a specific size like this,
		//The procedure is to first create the window hidden, then we get the DPI from
		//the HWND (which will be assigned by whichever monitor the window is created on)
		//Then we use SetWindowPos to resize it to the correct DPI-Scaled size, then we use
		//ShowWindow to show it

		m_hwnd = CreateWindow(
			"D2DDemoApp",
			"Direct2D demo application",
			WS_OVERLAPPEDWINDOW,
			CW_USEDEFAULT,
			CW_USEDEFAULT,
			0,
			0,
			NULL,
			NULL,
			HINST_THISCOMPONENT,
			this);

		if(m_hwnd)
		{
			//Because the SetWindowPos function takes its size in pixels,
			//We obtain the windows DPI and use it to scale the window size
			float dpi = GetDpiForWindow(m_hwnd);

			SetWindowPos(
				m_hwnd,
				NULL,
				NULL,
				NULL,
				static_cast<int>(ceil(640.f * dpi / 96.f)),
				static_cast<int>(ceil(480.f * dpi / 96.f)),
				SWP_NOMOVE);

			ShowWindow(m_hwnd, SW_SHOWNORMAL);
			UpdateWindow(m_hwnd);
		}
	}

	return hr;
}

HRESULT DemoApp::CreateDeviceIndependentResources()
{
	HRESULT hr = S_OK;

	//Create a Direct2D factory
	hr = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &m_pDirect2dFactory);

	return hr;
}

//Create the windows device-dependent resources, a render target and two brushes
HRESULT DemoApp::CreateDeviceResources()
{
	HRESULT hr = S_OK;

	//Check if render target exists
	if(!m_pRenderTarget)
	{
		//Retrieve the size of the client area and create a ID2D1HwndRenderTarget the same size
		RECT rc;
		GetClientRect(m_hwnd, &rc);

		D2D1_SIZE_U size = D2D1::SizeU(
			rc.right - rc.left,
			rc.bottom - rc.top);

		//Create a Direct2D render target
		hr = m_pDirect2dFactory->CreateHwndRenderTarget(
			D2D1::RenderTargetProperties(),
			D2D1::HwndRenderTargetProperties(m_hwnd, size),
			&m_pRenderTarget);

		//Initalise the Colours
		if(SUCCEEDED(hr)){COLOURS::Init(m_pRenderTarget);}
	}

	return hr;
}

void DemoApp::DiscardDeviceResources()
{
	SafeRelease(&m_pRenderTarget);
	COLOURS::Deinit();
}

//
HRESULT DemoApp::OnRender()
{
	HRESULT hr = S_OK;

	hr = CreateDeviceResources();

	if(SUCCEEDED(hr))
	{
		//Set the render target and set the indentity matrix
		m_pRenderTarget->BeginDraw();
		m_pRenderTarget->SetTransform(D2D1::Matrix3x2F::Identity());
		m_pRenderTarget->Clear(D2D1::ColorF(D2D1::ColorF::Black));

		//Retrieve the drawing area
		D2D1_SIZE_F rtSize = m_pRenderTarget->GetSize();

		Game::Render(m_pRenderTarget);



		//EndDraw returns HRESULT to show if it successfully drew
		hr = m_pRenderTarget->EndDraw();

		//Error with drawing render target needs to be recreated
		if(hr == D2DERR_RECREATE_TARGET)
		{
			hr = S_OK;
			DiscardDeviceResources();
		}
	}

	return hr;
}

void DemoApp::OnResize(UINT width, UINT height)
{
	if(m_pRenderTarget)
	{
		//This method can fail but its okay to ignore the error here as the error will be returned at EndDraw
		m_pRenderTarget->Resize(D2D1::SizeU(width, height));
	}
}

POINT mousePos;

//Basic windows message handling, if no message is found use basic windows event handling
LRESULT CALLBACK DemoApp::WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	LRESULT result = 0;

	InvalidateRect(hwnd, NULL, false);

	if(GetCursorPos(&mousePos) == 0){printf("GetCursorPos Error\n");}
	if(ScreenToClient(hwnd, &mousePos) == 0){printf("ScreenToClient ERROR\n");} //WEIRDNESS

	UpdateLoop();

	if(message == WM_CREATE)
	{
		LPCREATESTRUCT pcs = (LPCREATESTRUCT) lParam;
		DemoApp *pDemoApp = (DemoApp*)pcs->lpCreateParams;

		::SetWindowLongPtrW(
			hwnd,
			GWLP_USERDATA,
			reinterpret_cast<LONG_PTR>(pDemoApp));

		result = 1;
	}
	else
	{
		DemoApp *pDemoApp = reinterpret_cast<DemoApp*>(static_cast<LONG_PTR>(
			::GetWindowLongPtrW(
				hwnd,
				GWLP_USERDATA)));

		bool wasHandled = false;

		if(pDemoApp)
		{
			switch(message)
			{
				case WM_SIZE:
				{
					UINT width = LOWORD(lParam);
					UINT height = HIWORD(lParam);
					pDemoApp->OnResize(width,height);	
					result = 0;
					wasHandled = true;
					break;
				}

				case WM_DISPLAYCHANGE:
				{
					InvalidateRect(hwnd, NULL, false);	
					result = 0;
					wasHandled = true;
					break;
				}

				case WM_PAINT:
				{
					pDemoApp->OnRender();
					ValidateRect(hwnd, NULL);	
					result = 0;
					wasHandled = true;
					break;
				}

				case WM_DESTROY:
				{
					PostQuitMessage(0);
					result = 1;
					wasHandled = true;
					break;
				}

				case WM_KEYDOWN:
				{
					wasHandled = Game::HandleInput(wParam, true);
					break;
				}
				
				case WM_KEYUP:
				{
					wasHandled = Game::HandleInput(wParam, false);
					break;
				}
			}
		}

		if(!wasHandled)
		{
			result = DefWindowProc(hwnd, message, wParam, lParam);
		}
	}

	return result;
}

void DemoApp::UpdateLoop()
{
	QueryPerformanceCounter((LARGE_INTEGER*)&current_time);
	delta = (current_time - prev_time) * sec_per_count;

	Game::Update(delta);

	QueryPerformanceCounter((LARGE_INTEGER*)&prev_time);
}

//Win32API does not by default have a console association, even when opened using the console
void RedirectIOToConsole()
{
	if(AttachConsole(ATTACH_PARENT_PROCESS) == false) return;

	HANDLE ConsoleOutput = GetStdHandle(STD_OUTPUT_HANDLE);
	int SystemOutput = _open_osfhandle(intptr_t(ConsoleOutput), _O_TEXT);

	//Checks if output is a console and not redirected to file
	if(isatty(SystemOutput)==false) return; // return if its not a tty

	FILE *COutputHandle = _fdopen(SystemOutput, "w");

	//Get STDERR handle
	HANDLE ConsoleError = GetStdHandle(STD_ERROR_HANDLE);
	int SystemError = _open_osfhandle(intptr_t(ConsoleError), _O_TEXT);
	FILE* CErrorHandle = _fdopen(SystemError, "w");

	//Get STDIN handle
	HANDLE ConsoleInput = GetStdHandle(STD_INPUT_HANDLE);
	int SystemInput = _open_osfhandle(intptr_t(ConsoleInput), _O_TEXT);
	FILE* CInputHandle = _fdopen(SystemInput, "r");

	//make cout, wcout, cin, wcin, wcerr, cerr, wclog, clog point to console
	std::ios::sync_with_stdio(true);

	//Redirect the CRT standard input, output and error handles to the console
	freopen_s(&CInputHandle, "CONIN$", "r", stdin);
	freopen_s(&COutputHandle, "CONOUT$", "w", stdout);
	freopen_s(&CErrorHandle, "CONOUT$", "w", stderr);

	//Clear the error state for each of the C++ standard stream objects.
	std::wcout.clear();
	std::cout.clear();
	std::wcerr.clear();
	std::cerr.clear();
	std::wcin.clear();
	std::cin.clear();
}

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	//Redirect printed output to console
	RedirectIOToConsole();
	//Use HeapSetInformation to specify that the process should terminate
	//if the heap manager detects an error in any heap used by the process.
	//The return value is ignored, because we want to continue running in
	//the unlikely event that HeapSetInformation fails
	HeapSetInformation(NULL, HeapEnableTerminationOnCorruption, NULL, 0);

	//Setup randomness
	std::srand(std::time(nullptr));



	if(SUCCEEDED(CoInitialize(NULL)))
	{
		DemoApp app;

		//Setup Framerate and Deltatime
		QueryPerformanceFrequency((LARGE_INTEGER*)&counts_per_sec);
		sec_per_count = 1.0f / (float) counts_per_sec;
		QueryPerformanceCounter((LARGE_INTEGER*)&prev_time);

		if(SUCCEEDED(app.Initialize()))
		{
			Game::Init();

			app.RunMessageLoop();
			app.RunEventLoop();
		}
		CoUninitialize();
	}
}