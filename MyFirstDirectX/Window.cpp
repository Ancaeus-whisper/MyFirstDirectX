#include "Window.h"
#include"resource.h"
#include <sstream>
#include<string>

Window::WindowClass Window::WindowClass::wndClass;

Window::WindowClass::WindowClass() noexcept 
	:hInst(GetModuleHandle(nullptr)) 
{
	//创建窗口过程类
	WNDCLASSEX wc = { 0 };
	wc.cbSize = sizeof(wc);
	wc.style = CS_OWNDC;
	wc.lpfnWndProc = HandleMessageSetup;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = GetInstance();
	wc.hIcon = static_cast<HICON>(LoadIcon(hInst,MAKEINTRESOURCE(IDI_ICON1)));
	wc.hCursor = nullptr;
	wc.hbrBackground = nullptr;
	wc.lpszMenuName = nullptr;
	wc.lpszClassName = GetName();
	wc.hIconSm = static_cast<HICON>(LoadIcon(hInst, MAKEINTRESOURCE(IDI_ICON1)));
	RegisterClassEx(&wc);
}

Window::WindowClass::~WindowClass() 
{
	UnregisterClass(wndClassName,GetInstance());
}

const HINSTANCE Window::WindowClass::GetInstance() noexcept
{
	return wndClass.hInst;
}

const char* Window::WindowClass::GetName()noexcept 
{
	return wndClassName;
}

Window::Window(int width, int height, const char* name):width(width),height(height)
{
	RECT wr;
	wr.left =100;
	wr.right = 100 + width;
	wr.top = 100;
	wr.bottom = 100 + height;
	if (AdjustWindowRect(&wr, WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU, FALSE)==0) 
	{
		throw ACWND_LAST_EXCEPT();
	}
	//创建窗口句柄
	hWnd = CreateWindow
	(
		WindowClass::GetName(),name,
		WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		wr.right-wr.left,wr.bottom-wr.top,
		nullptr,nullptr,
		WindowClass::GetInstance(),this
	);
	if (hWnd == nullptr) 
	{
		throw ACWND_LAST_EXCEPT();
	}
	//渲染窗口
	ShowWindow(hWnd, SW_SHOWDEFAULT);
}

Window::~Window()
{
	DestroyWindow(hWnd);
}

void Window::SetTitle(const std::string title)
{
	if (SetWindowText(hWnd, title.c_str())==0)
	{
		throw ACWND_LAST_EXCEPT();
	}
}

LRESULT Window::HandleMessageSetup(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	if(msg == WM_NCCREATE) 
	{
		const CREATESTRUCTW* const pCreate = reinterpret_cast<CREATESTRUCTW*>(lParam);
		Window* const pWnd = static_cast<Window*>(pCreate->lpCreateParams);
		SetWindowLongPtr(hWnd,GWLP_USERDATA,reinterpret_cast<LONG_PTR>(pWnd));
		SetWindowLongPtr(hWnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(Window::HandleMessageThunk));
		return pWnd->HandleMsg(hWnd, msg, wParam, lParam);
	}
	return DefWindowProc(hWnd,msg,wParam,lParam);
}

LRESULT Window::HandleMessageThunk(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	Window* const pWnd = reinterpret_cast<Window*>(GetWindowLongPtr(hWnd,GWLP_USERDATA));
	return pWnd->HandleMsg(hWnd,msg,wParam,lParam);
}

LRESULT Window::HandleMsg(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept
{
	switch (msg)
	{
		case WM_CLOSE
			:PostQuitMessage(0);
			return 0;

		case WM_KILLFOCUS:
			kbd.ClearState();
			break;
		/*
		按键处理部分在此定义
		*/
		case WM_SYSKEYDOWN:
		case WM_KEYDOWN:
			if(!(lParam&0x40000000)||kbd.AutoRepeatIsEnable()) //解释说明，lParam的第30位保存了之前是否连按该键的信息，为了获得是否连按的信息，将lParam与0100，{0...0}相与
			{
			kbd.OnKeyPressed(static_cast<unsigned char>(wParam));
			}
			break;
		case WM_SYSKEYUP:
		case WM_KEYUP:
			kbd.OnKeyReleased(static_cast<unsigned char>(wParam));
			break;
		case WM_CHAR:
			kbd.OnChar(static_cast<unsigned char>(wParam));
			break;
		/*
		鼠标事件在此定义
		*/
		case WM_MOUSEMOVE:
		{
			const POINTS pt = MAKEPOINTS(lParam);
			//这里需要对于鼠标是否在屏幕中进行判断，如果出了屏幕，就要停止鼠标指针的移动
			if (pt.x >= 0 && pt.x < width && pt.y>=0 && pt.y < height)
			{
				mouse.OnMouseMove(pt.x, pt.y);
				if (!mouse.IsInWindow())
				{
					SetCapture(hWnd);
					mouse.OnMouseEnter();
				}
			}
			//如果鼠标按键还没松开，说明玩家还在操作
			else {
				if (wParam & (MK_LBUTTON | MK_RBUTTON))
				{
					mouse.OnMouseMove(pt.x, pt.y);
				}
				else
				{
					ReleaseCapture();
					mouse.OnMouseLeave();
				}
			}
			break;
		}
		case WM_LBUTTONDOWN:
		{
			const POINTS pt = MAKEPOINTS(lParam);
			mouse.OnLeftPressed(pt.x,pt.y);
			break;
		}
		case WM_LBUTTONUP:
		{
			const POINTS pt = MAKEPOINTS(lParam);
			mouse.OnLeftReleased(pt.x, pt.y);
			break;
		}
		case WM_RBUTTONDOWN:
		{
			const POINTS pt = MAKEPOINTS(lParam);
			mouse.OnRightPressed(pt.x, pt.y);
			break;
		}
		case WM_RBUTTONUP:
		{
			const POINTS pt = MAKEPOINTS(lParam);
			mouse.OnRightReleased(pt.x, pt.y);
			break;
		}
		case WM_MOUSEWHEEL:
		{
			const POINTS pt = MAKEPOINTS(lParam);
			mouse.OnWheelDelta(pt.x,pt.y, GET_WHEEL_DELTA_WPARAM(wParam));
			break;
		}

	}
	return DefWindowProc(hWnd, msg, wParam, lParam);
}

Window::Exception::Exception(int line, const char* file, HRESULT hr) noexcept
	:AncaeusException(line,file),hr(hr)
{
}

const char* Window::Exception::what() const noexcept
{
	std::ostringstream oss;
	oss <<std::endl<< "[ERROR CODE]"<<GetErrorCode()<<std::endl
		<<"[DESCRIPTION]"<<GetErrorString()<<std::endl
		<<GetOriginString();
	whatBuffer += oss.str();
	return whatBuffer.c_str();
}

const char* Window::Exception::GetType() const noexcept
{
	return "WindowException";
}

std::string Window::Exception::TranslateErrorCode(HRESULT hr) noexcept
{
	char* pMsgBuf = nullptr;
	DWORD nMsgLen = FormatMessage(
	FORMAT_MESSAGE_ALLOCATE_BUFFER|FORMAT_MESSAGE_IGNORE_INSERTS
		|FORMAT_MESSAGE_FROM_SYSTEM,nullptr,hr,MAKELANGID(LANG_NEUTRAL,SUBLANG_DEFAULT)
		,reinterpret_cast<LPSTR>(&pMsgBuf),0,nullptr
	);
	if (nMsgLen == 0) 
	{
		return "Unidentified error code";
	}
	std::string errorString=pMsgBuf;
	LocalFree(pMsgBuf);
	return errorString;
}

HRESULT Window::Exception::GetErrorCode() const noexcept
{
	return hr;
}

std::string Window::Exception::GetErrorString() const noexcept
{
	return TranslateErrorCode(hr);
}
