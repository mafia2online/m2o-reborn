/** @file CCore.cpp
*  @brief Our mod entry point.
*
*  This file is supposed to join our various classes
*  and stuff together, so it'll work nicely and tight!
*
*  Non game releated stuff will be intialized here, game related stuff will be handled by the underlying game class,
*  only initialize mod related stuff here.
*
*  @author MyU (myudev0@gmail.com)
*  @todo Improve our get method for the app directory
*/

#include <stdafx.h>
#include "CCore.h"
#include <CTitleState.h>

CCore::CCore() :
	m_debuglog(true, true)
{

}

CCore::~CCore()
{

}

void CCore::OnAttach(HMODULE module)
{
	m_module = module;

	// plox maybe integrate in some util class latr kthx
	char szRunPath[MAX_PATH] = { '\0' };
	GetModuleFileName(module, szRunPath, MAX_PATH);
	m_strappdir.Set(szRunPath);
	size_t pos = m_strappdir.GetSTLString().rfind("\\");
	m_strappdir.GetSTLString().erase(pos, std::string::npos);
	

	SString strLogPath(m_strappdir + "\\debug_log.txt");

	if (m_debuglog.Open(strLogPath))
	{
		m_debuglog.Writeln("Succesfully started %s compiled on %s %s.", MOD_NAME, __DATE__, __TIME__);
	}

	// todo: check return values and scream like a little baby
	m_graphicsmanager.Init();
	m_game.Initialize();

	m_statemanager.AddState(States::Title, new CTitleState);

	m_statemanager.ActivateState(States::Title); // GO IN THE TITLE STATE! IMMEDIATEALY!
}
