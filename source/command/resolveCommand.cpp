//
// Created by mara on 02.12.2023.
//

#include "../../headers/server.h"

/**Resolve the incoming token into a command.
 *
 * Whenever a new command is to be implemented, its entry must also be done here.
*/
Server::Command Server::resolveCommand(const string& incomingCommandToken) {
    string cmd(incomingCommandToken);
    std::transform(cmd.begin(), cmd.end(), cmd.begin(),
                   std::ptr_fun<int, int>(std::toupper));

    cout << "Resolving command: " << cmd << endl; // Log the command being resolved

    if (cmd.compare("USER") == 0)
        return Command::USER;
    if (cmd.compare("PASS") == 0)
        return Command::PASS;
    if (cmd.compare("QUIT") == 0)
        return Command::QUIT;
    if (cmd.compare("PORT") == 0)
    {
        cout << "(resolveCommand.cpp) Command resolved to PORT"<< endl; // Log the command being resolved
        return Command::PORT;}
    if (cmd.compare("LIST") == 0)
        return Command::LIST;
    return Command::INVALID;
}
