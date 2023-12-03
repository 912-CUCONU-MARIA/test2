//
// Created by mara on 03.12.2023.
//
#include "../../../headers/server.h"
#include "../../../headers/utils.h"  // Adjust the path according to your project structure

/**PORT command
 *
 * @usage
 * sets new Data connection IP and PORT for the next command
*/
void Server::cmd_PORT(int controlConnectionfd, const vector<string>& args) {
    string IP;
    int PORT;

    // @todo
    /**PORT Command format
     *
     * Actually, the port command has a comma format during the transfer.
     * But for simplicity here, we have hold on to original format
     * for transfer as well.
     *
     * We can implement comma format if needed,
     * encode port string on client side and decode it on the server side.
     *
     * resolvePortCommandString(IP, PORT);
     *
    */
    cout << "Entered cmd_PORT function" << endl; // Log at the start of the function


//    IP = args[1];
//    PORT = atoi(args[2].c_str());
//
//    // Log the parsed IP and PORT
//    cout << "PORT Command Received - IP: " << IP << ", Port: " << PORT << endl;
//
//
//    setDataConnectionIP(IP);
//    setDataConnectionPortNumber(PORT);

// The PORT command argument is expected to be in the format: "127,0,0,1,204,173"
    string portCommand = args[1];

    // Split the command into its parts
    vector<string> portParts = splitString(portCommand, ",");

    // Reconstruct IP and calculate port
    IP = portParts[0] + "." + portParts[1] + "." + portParts[2] + "." + portParts[3];
    int p1 = stoi(portParts[4]);
    int p2 = stoi(portParts[5]);
    PORT = p1 * 256 + p2;

    cout << "PORT Command Received - IP: " << IP << ", Port: " << PORT << endl;

    setDataConnectionIP(IP);
    setDataConnectionPortNumber(PORT);

    // Additional logging for data connection status
    cout << "Attempting to set data connection - IP: " << getDataConnectionIP()
         << ", Port: " << getDataConnectionPortNumber() << endl;

    string res("PORT Command Successful.");
    res += "\nNew Data IP is : " + string(getDataConnectionIP());
    res += "\nNew Data Port is : " + to_string(getDataConnectionPortNumber());

    // @logging
    // logServerConfiguration();

    Send(controlConnectionfd, res, 200);
}
