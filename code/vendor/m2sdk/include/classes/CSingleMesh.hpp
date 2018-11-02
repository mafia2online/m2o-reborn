/** @file CSingleMesh.hpp
*  @brief Game's class
*
*
*  @author Tyldar (darreugne@gmail.com)
*/

#pragma once

namespace M2
{
	class ICRenderSkinnedVisual
	{
	public:
		pad(C_RenderSkinnedVisual, pad0, 0x64);
		DWORD m_dwLastRenderTime;
	};
	class C_RenderSkinnedVisual
	{
	public:
	};

	class ICSingleMesh
	{
	public:
		pad(C_SingleMesh, pad0, 0x6C);
		C_RenderSkinnedVisual *m_pRenderSkinnedVisual;
	};

	class C_SingleMesh : public ICSingleMesh
	{
	public:
	};
};
