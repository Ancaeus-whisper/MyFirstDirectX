#pragma once
#include "WinDefine.h"
#include<string>
#include"AncaeusException.h"

class Window 
{
	public:
		class Exception:public AncaeusException
		{
		public:
			Exception(int line,const char* file,HRESULT hr)noexcept;
			const char* what()const noexcept override;
			virtual const char* GetType()const noexcept;
			static std::string TranslateErrorCode(HRESULT hr)noexcept;
			HRESULT GetErrorCode()const noexcept;
			std::string GetErrorString()const noexcept;
		private:
			HRESULT hr;
		};

	private:
	class WindowClass 
	{
	public:
		static const char* GetName() noexcept;
		static const HINSTANCE GetInstance()noexcept;
	private:
			WindowClass() noexcept;
			~WindowClass();
			WindowClass(const WindowClass&) = delete;
			WindowClass& operator = (const WindowClass&) = delete;
		   WNDCLASSEX wc;
		   static constexpr const char* wndClassName = "Direct3DTest";
		   static WindowClass wndClass;
		   HINSTANCE hInst;
	};
public:Window(int width,int height,const char* name)noexcept;
	  ~Window();
	  Window(const Window&) = delete;
	  Window& operator=(const Window&) = delete;
private:
	static LRESULT CALLBACK HandleMessageSetup(HWND hWnd,UINT msg,WPARAM wParam,LPARAM lParam);
	static LRESULT CALLBACK HandleMessageThunk(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
	LRESULT HandleMsg(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;
private:
	int width;
	int height;
	HWND hWnd;
};

#define CHWND_EXCEPT(hr) Window::Exception(__LINE__,__FILE__,hr)