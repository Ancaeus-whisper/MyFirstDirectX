#include "Keyboard.h"

bool Keyboard::KeyIsPressed(const char keyCode) const noexcept
{
	return keyStatus[keyCode];
}

Keyboard::Event Keyboard::ReadKey() noexcept
{
	if (keyBuffer.size() > 0u)
	{
		Event value = keyBuffer.front();
		keyBuffer.pop();
		return value;
	}
	else return Keyboard::Event();
}

bool Keyboard::KeyIsEmpty() const noexcept
{
	return keyBuffer.empty();
}

void Keyboard::FlushKey() noexcept
{
	keyBuffer = std::queue<Event>();
}

char Keyboard::ReadChar() noexcept
{
	if (charBuffer.size() > 0)
	{
		char value = charBuffer.front();
		charBuffer.pop();
		return value;
	}
	else return 0;
}

bool Keyboard::CharIsEmpty() const noexcept
{
	return charBuffer.empty();
}

void Keyboard::FlushChar() noexcept
{
	charBuffer = std::queue<char>();
}

void Keyboard::Flush() noexcept
{
	FlushKey();
	FlushChar();
}

void Keyboard::EnableAutoRepeat() noexcept
{
	autoRepeatEnable = true;
}

void Keyboard::DisableAutoRepeat() noexcept
{
	autoRepeatEnable = false;
}

bool Keyboard::AutoRepeatIsEnable() noexcept
{
	return autoRepeatEnable;
}

void Keyboard::OnKeyPressed(const char keyCode) noexcept
{
	keyStatus[keyCode] = true;
	keyBuffer.push(Keyboard::Event(Keyboard::Event::Type::Press,keyCode));
	TrimBuffer(keyBuffer);
}

void Keyboard::OnKeyReleased(const char keyCode) noexcept
{
	keyStatus[keyCode] = false;
	keyBuffer.push(Keyboard::Event(Keyboard::Event::Type::Release, keyCode));
	TrimBuffer(keyBuffer);
}

void Keyboard::OnChar(const char Char) noexcept
{
	charBuffer.push(Char);
	TrimBuffer(charBuffer);
}

void Keyboard::ClearState() noexcept
{
	keyStatus.reset();
}
