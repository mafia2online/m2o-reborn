#pragma once
#include <CMPStateManager.h>

class CTitleState : public IMPState
{
public:
	void InitializeResources(void *userptr) override;

	void Activate(void *userptr) override {};
	void Deactivate(void *userptr) override {};
	void Tick(void *userptr) override {};

	void Render(void *userptr) override;
};