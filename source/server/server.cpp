//
// Created by mara on 02.12.2023.
//
#include "../../headers/server.h"

/**Protocol Interpretor for server
 *
 * Classic FTP has two parts.
 * The PI to interpret commands, and the data connection to transfer files.
 *
*/
void Server::initiateProtocolInterpreter(int controlConnectionfd) {
    // begin
    //todo Send(controlConnectionfd, "J's FTP-Server ");
    Send(controlConnectionfd, "Mara's Security FTP-Server is Ready", 220);

    // authentication
    authenticateClient(controlConnectionfd);
    if (getClientAuthenticationStatus() == false) {
        cmd_QUIT(controlConnectionfd);
        return;
    }

    /**Previous command tracking
     *
     * PORT command changes the datadump site for the next command only.
     * So we track the previous command, and reset the dump site after one command
     *
    */
    Command previousCommand = Command::INVALID;
    Command currentCommand = Command::INVALID;

    // command interpreter loop
    while (true) {
        // receive command from client
        string commandString;
        Recv(controlConnectionfd, commandString);
        // @logging
        fprintf(stdout, "\n(%s) [INFO][REQUEST] : %s", getClientUsername().c_str(), commandString.c_str());

        // tokenize command
        vector<string> tokens = commandTokenizer(commandString);

        // resolve command type from first word
        Command commandType = resolveCommand(tokens.front());

        switch (commandType) {

            case Command::QUIT: {
                cmd_QUIT(controlConnectionfd);
            } break;
            case Command::LIST: {
                cmd_LIST(controlConnectionfd, tokens);
            } break;
            case Command::PORT: {
                cout << "(server.cpp) Command is PORT"<< endl; // Log the command
                cmd_PORT(controlConnectionfd, tokens);
            } break;
            default: { cmd_INVALID(controlConnectionfd); } break;
        }

        // previous command tracking
        previousCommand = currentCommand;
        currentCommand = commandType;
//todo what does this do
        // reset PORT to default after one command
        if (previousCommand == Command::PORT) {
            resetDataConnectionIP();
            resetDataConnectionPortNumber();
        }

    }  // end:while

}  // end:function