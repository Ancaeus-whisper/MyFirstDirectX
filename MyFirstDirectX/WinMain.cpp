#include<Windows.h>
#include"Window.h"
#include<string>
#include<sstream>
/*
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
			SetWindowText(hWnd, "�봥�������ĵĺڰ�");
		}break;
		case WM_KEYUP
		:if (wParam == 'F')
		{
			SetWindowText(hWnd, "����Ͷ��");
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
*/
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
	Window wnd(800,600,"MyFirstDirectX"); //����������
	//��Ϣ������
	MSG msg;
	BOOL gResult;
	while ((gResult=GetMessage(&msg, nullptr, 0, 0)) > 0) 
	{
		TranslateMessage(&msg);//������Ϣ
		DispatchMessageW(&msg);//����Ϣ���͸�������
		if (wnd.kbd.KeyIsPressed(VK_SPACE)) 
		{
			MessageBox(nullptr,"Something Happen!","SpaceWasPressed",MB_OK);
		}
		while (!wnd.mouse.IsEmpty()) 
		{
			const auto e = wnd.mouse.Read();
			switch (e.GetType()) 
			{
			case Mouse::Event::Type::Enter:
			case Mouse::Event::Type::Move:			
			{
				std::ostringstream oss;
				oss << "Move to Pos(" << e.GetPosX() << "," << e.GetPosY() << ")";
				wnd.SetTitle(oss.str());
				break;
			}
			case Mouse::Event::Type::Leave:
			{
				wnd.SetTitle("Gone!");
				break;
			}
			}
		}
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
	catch (const AncaeusException& e) //�׳��쳣
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
	