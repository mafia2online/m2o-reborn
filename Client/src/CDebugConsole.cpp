#include <stdafx.h>
#include <CCore.h>
#include <GwenManager.h>
#include "CDebugConsole.h"

void CDebugConsole::RegisterComponents(Gwen::Controls::Canvas * pCanvas)
{
	Gwen::Controls::WindowControl *base = new Gwen::Controls::WindowControl(pCanvas);
	base->SetBounds(10, 10, 600, 400);
	base->SetTitle(L"Debug console ЧФчфв");
	
	m_plistboxctrl = new Gwen::Controls::ListBox(base);
	m_plistboxctrl->Dock(Gwen::Pos::Top);
	m_plistboxctrl->SetHeight(m_plistboxctrl->GetParent()->Height() - 70);

	CCore::Instance().GetLogger().SetCallback(
		[=](const char *pszText)->void
	{
		m_plistboxctrl->AddItem(pszText);
		m_plistboxctrl->ScrollToBottom();
	}
	);

	m_inputbox = new Gwen::Controls::TextBox(base);
	m_inputbox->Dock(Gwen::Pos::Bottom);
	m_inputbox->SetHeight(30);
	m_inputbox->onReturnPressed.Add(this, &CDebugConsole::OnSubmit);

	Focus();
}

void CDebugConsole::OnSubmit(Gwen::Controls::Base* pControl)
{
	Gwen::Controls::TextBox* textbox = (Gwen::Controls::TextBox*)(pControl);
	
	if (textbox->GetText().m_String[0] == '/')
	{
		if (!CommandProcessor::ProcessCommand(textbox->GetText().m_String))
		{
			m_plistboxctrl->AddItem("> Unknown command.");
			m_plistboxctrl->ScrollToBottom();
		}
	}
	textbox->SetText("");
	m_inputbox->SetKeyboardInputEnabled(false);

}

void CDebugConsole::Focus()
{
	m_inputbox->Focus();
	m_inputbox->SetKeyboardInputEnabled(true);
}

// dirty mafaka
std::vector<CommandProcessor::stCommand>	CommandProcessor::commands;
bool CommandProcessor::RegisterCommand(std::string command, CommandProcedure function)
{
	if (command.length() <= 0 || function == nullptr)
		return false;

	if (commands.size() > 0)
		if (std::find(commands.begin(), commands.end(), command) != commands.end())
			return false;

	stCommand	cmd;
	cmd.command = command;
	cmd.func = function;
	commands.push_back(cmd);

	return true;
}
bool CommandProcessor::ProcessCommand(std::string command)
{
	if (command.length() <= 0)
		return false;
	if (commands.size() <= 0)
		return false;

	std::string cmd = &command[1], params = "";
	std::size_t firstspace = cmd.find_first_of(' ');
	if (firstspace != std::string::npos)
	{
		cmd.erase(firstspace, cmd.length());

		std::size_t notaspace = command.find_first_not_of(' ', firstspace + 1);
		if (notaspace != std::string::npos)
			params = &command[notaspace];
	}

	std::vector<stCommand>::iterator	iter;
	if ((iter = std::find(commands.begin(), commands.end(), cmd)) == commands.end())
		return false;
	iter->func(params);

	return true;
}