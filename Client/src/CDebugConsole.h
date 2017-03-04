#pragma once
#include <Gwen/Controls/ListBox.h>
#include <Gwen/Controls/TextBox.h>
#include <Gwen/Controls/WindowControl.h>

class	CommandProcessor
{
public:
	using CommandProcedure = std::function<void(const std::string&)>;
	struct stCommand
	{
		bool operator==(const std::string &string) const
		{
			return _stricmp(string.c_str(), command.c_str()) == 0;
		};
		std::string			command;
		CommandProcedure	func;
	};

	static bool RegisterCommand(std::string command, CommandProcedure function);
	static bool ProcessCommand(std::string command);
	static std::vector<stCommand>	commands;
};

class CDebugConsole : public Singleton<CDebugConsole>, public Gwen::Event::Handler
{
public:
	void RegisterComponents(Gwen::Controls::Canvas *pCanvas);

	void OnSubmit(Gwen::Controls::Base* pControl);

	void Focus();
	bool HasFocus() { return m_inputbox->HasFocus(); }

private:
	Gwen::Controls::ListBox *m_plistboxctrl = nullptr;
	Gwen::Controls::TextBox *m_inputbox = nullptr;
};
