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

class CDebugConsole : public Singleton<CDebugConsole>
{
public:

    void Focus();
    bool HasFocus() { return false; }

private:
};



void CDebugConsole::Focus()
{
    // todo focus
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
