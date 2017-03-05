/** @file CHud.cpp
*  @brief Game's CHud wrapper
*
*
*  @author Tyldar (darreugne@gmail.com)
*/

#pragma once
#include "CommonHeaders.h"

namespace M2
{
	class ICHud
	{

	};
	class CHud : public GameClassWrapper<CHud, ICHud, 0x1CBA618>
	{

	};
};