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
#include <CGameState.h>
#include "ExceptionHandler.h"

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
	

	SString strLogPath(m_strappdir + "\\logs\\debug_log.txt");

	if (m_debuglog.Open(strLogPath))
	{
		m_debuglog.Writeln("Succesfully started %s compiled on %s %s.", MOD_NAME, __DATE__, __TIME__);
	}

	if (m_graphicsmanager.Init() == false) {
		ExitGame("Unable to init Graphics Manager");
	}

	if (CGame::Instance().Initialize() == false) {
		ExitGame("Unable to init game instance");
	}

	if (ExceptionHandler::Install() == false)
		ExitGame("Unable to install exception handler");

	m_statemanager.AddState(States::Menu, new CTitleState);
	m_statemanager.ActivateState(States::Menu); // GO IN THE TITLE STATE! IMMEDIATEALY!
}

void CCore::ExitGame(SString strreason)
{
	MessageBoxA(nullptr, strreason.GetCStr(), "Well.. Something went wrong!", MB_OK);
	exit(0);
}
