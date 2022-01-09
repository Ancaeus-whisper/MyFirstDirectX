#include<Windows.h>
#include"Window.h"
#include<string>
#include<sstream>

LRESULT CALLBACK WndProc(HWND hWnd,UINT msg,WPARAM wParam,LPARAM lParam)
{
	switch (msg)
	{
		case WM_QUIT
		:PostQuitMessage(77);
			break;
		case WM_KEYDOWN
		:if (wParam == 'F')
		{
			SetWindowText(hWnd, "请触碰我内心的黑暗");
		}break;
		case WM_KEYUP
		:if (wParam == 'F')
		{
			SetWindowText(hWnd, "弃明投暗");
		}break;
		case WM_CHAR
		: 
		{
			static std::string title;
			title.push_back(wchar_t(wParam));
			SetWindowText(hWnd,title.c_str());
		}
		break;
		case WM_LBUTTONDOWN
		:
		{
			POINTS pt = MAKEPOINTS(lParam);
			std::ostringstream oss;
			oss << "(" << pt.x << "," << pt.y << ")";
			SetWindowText(hWnd,oss.str().c_str());

		}break;
	}
	return DefWindowProc(hWnd,msg,wParam,lParam);
}

int CALLBACK WinMain
(
	HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR lPCmdLine,
	int CmdShow
) 
{
	try 
	{
	Window wnd(800,300,"MyFirstDirectX");

	//消息处理函数
	MSG msg;
	BOOL gResult;
	while ((gResult=GetMessage(&msg, nullptr, 0, 0)) > 0) 
	{
		TranslateMessage(&msg);//处理消息
		DispatchMessageW(&msg);//将消息发送给处理函数
	}
	
	if (gResult == -1) 
	{ 
		return -1; 
	}
	else 
	{ 
		return msg.wParam; 
	}
	}
	catch (const AncaeusException e) 
	{
		MessageBox(nullptr,e.what(),e.GetType(),MB_OK|MB_ICONEXCLAMATION);
	}
	catch (const std::exception e) 
	{
		MessageBox(nullptr,e.what(),"Standard Error", MB_OK|MB_ICONEXCLAMATION);
	}
	catch (...) 
	{
		MessageBox(nullptr, "No details available", "Unknown Exception", MB_OK | MB_ICONEXCLAMATION);
	}
	}
	