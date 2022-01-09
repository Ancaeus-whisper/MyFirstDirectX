#include "AncaeusException.h"
#include<sstream>

AncaeusException::AncaeusException(int line,const char* file) noexcept
	:line(line),file(file)
{
}

const char* AncaeusException::what() const noexcept
{
	std::ostringstream oss;
	oss << GetType() << std::endl
		<< GetOriginString();
	whatBuffer = oss.str();
	return whatBuffer.c_str();
}

const char*  AncaeusException::GetType() const noexcept
{
	return "AncaeusException";
}

int AncaeusException::GetLine() const noexcept
{
	return line;
}

const std::string& AncaeusException::GetFile() const noexcept
{
	return file;
}

std::string AncaeusException::GetOriginString() const noexcept
{
	std::ostringstream oss;
	oss << "[FILE]" << file << std::endl
		<< "[LINE]" << line;
	return oss.str();
}
