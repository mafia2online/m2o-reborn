#pragma once

namespace ExceptionHandler
{
	bool Install();
	long WINAPI ExceptionHandlerCallback(_EXCEPTION_POINTERS *pExceptionInfo);
};
