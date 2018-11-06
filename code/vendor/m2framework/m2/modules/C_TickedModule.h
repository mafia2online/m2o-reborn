#pragma once

#include <stdint.h>

class C_TickedModule {
public:
	virtual ~C_TickedModule() = default;
	virtual int32_t GetTypeID() = 0;
	virtual const char* GetDebugName() = 0;
	virtual void RegisterCustoms() = 0;

    // 2 more unused subs below
};
