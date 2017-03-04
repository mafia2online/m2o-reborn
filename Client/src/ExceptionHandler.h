#pragma once

namespace ExceptionHandler
{
	void Install();
	long WINAPI ExceptionHandlerCallback(_EXCEPTION_POINTERS *pExceptionInfo);
};
