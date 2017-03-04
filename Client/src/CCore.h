#pragma once
#include <CGame.h>
#include <Shared\Common.h>
#include <Shared\FileLogger.h>
#include <CGraphicsManager.h>
#include <CMPStateManager.h>

class CCore : public Singleton<CCore>
{
public:
	CCore();
	~CCore();

	void OnAttach(HMODULE module);
	void ExitGame(SString strreason);

public:
	SString& GetModAppDir() { return m_strappdir; }
	SString& GetModFilesDir() { return m_strfilesdir; }
	IO::CFileLogger& GetLogger() { return m_debuglog; }
	CGraphicsManager& GetGraphics() { return m_graphicsmanager; }
	CMPStateManager& GetStateManager() { return m_statemanager; }

private:
	HMODULE m_module;
	SString m_strappdir;
	SString m_strfilesdir;
	CGraphicsManager m_graphicsmanager;
	CMPStateManager m_statemanager;

	IO::CFileLogger m_debuglog;
};