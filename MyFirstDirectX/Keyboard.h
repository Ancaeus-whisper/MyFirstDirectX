#pragma once
#include<queue>
#include<bitset>

class Keyboard 
{
	friend class Window;
	public:
		class Event
		{
			public:
				enum class Type
				{
				Press,
				Release,
				Invalid
				};
			private:
				Type type;
				unsigned char keyCode;
			public:
				Event()noexcept 
					:type(Type::Invalid),keyCode(0u) {}
				Event(Type type, char code) noexcept
					:type(type), keyCode(code) {}
				bool IsPress(char keyCode) { return type == Type::Press; }
				bool IsRelease(char keyCode) { return type == Type::Release; }
				bool IsInvalid(char keyCode) { return type == Type::Invalid; }
				unsigned char GetCode() { return keyCode; }
		};
	public:
		Keyboard()noexcept=default;
		Keyboard(const Keyboard&) = delete;
		Keyboard& operator =(const Keyboard&)= delete;

		//按键事件
		bool KeyIsPressed(const char keyCode)const noexcept;
		Event ReadKey()noexcept;
		bool KeyIsEmpty()const noexcept;
		void FlushKey()noexcept;

		//字符事件
		char ReadChar()noexcept;
		bool CharIsEmpty()const noexcept;
		void FlushChar()noexcept;
		void Flush()noexcept;

		//自动重复设置
		void EnableAutoRepeat()noexcept;
		void DisableAutoRepeat()noexcept;
		bool AutoRepeatIsEnable()noexcept;
	
	private:
		void OnKeyPressed(const char keyCode)noexcept;
		void OnKeyReleased(const char keyCode)noexcept;
		void OnChar(const char Char)noexcept;
		void ClearState()noexcept;
		template<typename T>
		void TrimBuffer(std::queue<T>& buffer)noexcept;

	private:
		static constexpr unsigned int nKeys = 256u; //按键状态储存区域的大小
		static constexpr unsigned int nBuffer = 16u; //按键缓存区的大小
		bool autoRepeatEnable = false;
		std::bitset<nKeys> keyStatus;
		std::queue<Event> keyBuffer;
		std::queue<char> charBuffer;
};

template<typename T>
inline void Keyboard::TrimBuffer(std::queue<T>& buffer) noexcept
{
	while (buffer.size()>nBuffer) 
	{
		buffer.pop();
	}
}
