class CommandProcessor
{
public:
    using CommandProcedure = std::function<void(const std::string&)>;
    struct stCommand
    {
        bool operator==(const std::string &string) const
        {
            return _stricmp(string.c_str(), command.c_str()) == 0;
        };
        std::string         command;
        CommandProcedure    func;
    };

    static bool RegisterCommand(std::string command, CommandProcedure function);
    static bool ProcessCommand(std::string command);
    static std::vector<stCommand>   commands;
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


void CDebugConsole::RegisterComponents(Gwen::Controls::Canvas * pCanvas)
{
    Gwen::Controls::WindowControl *base = new Gwen::Controls::WindowControl(pCanvas);
    base->SetBounds(10, 10, 600, 300);
    base->SetTitle("Debug console сука");

    m_plistboxctrl = new Gwen::Controls::ListBox(base);
    m_plistboxctrl->Dock(Gwen::Pos::Top);
    m_plistboxctrl->SetHeight(m_plistboxctrl->GetParent()->Height() - 70);

    librg::events::add(librg::events::on_log, [=](librg::events::event_t* evt) {
        auto event = (librg::events::event_log_t*) evt;
        m_plistboxctrl->AddItem(event->output);
        m_plistboxctrl->ScrollToBottom();
    });

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
    m_inputbox->SetText("/");
    m_inputbox->SetCursorPos(1);
    m_inputbox->SetCursorEnd(1);
}

// dirty mafaka
std::vector<CommandProcessor::stCommand>    CommandProcessor::commands;
bool CommandProcessor::RegisterCommand(std::string command, CommandProcedure function)
{
    if (command.length() <= 0 || function == nullptr)
        return false;

    if (commands.size() > 0)
        if (std::find(commands.begin(), commands.end(), command) != commands.end())
            return false;

    stCommand   cmd;
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

    std::vector<stCommand>::iterator    iter;
    if ((iter = std::find(commands.begin(), commands.end(), cmd)) == commands.end())
        return false;
    iter->func(params);

    return true;
}
