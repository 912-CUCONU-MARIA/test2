//
// Created by mara on 02.12.2023.
//

#include "../../headers/server.h"

/**server config logger
 * Logs server's current configuration.
*/
void Server::logServerConfiguration() {
    logs("Server config is : ");
    logv(controlConnectionIP);
    logv(controlConnectionPortNumber);
    logv(dataConnectionIP);
    logv(dataConnectionPortNumber);
    logv(backlogsPermitted);
}

/**print Info
 *
 * print user's custom message
*/
void Server::printInfo(const char* msg) {
    fprintf(stdout, "\n(%s) [INFO] : %s\n", getClientUsername().c_str(), msg);
}

/**print Info : overloaded
 *
 * print user's custom message with a value
*/
void Server::printInfo(const char* msg, int value) {
    printInfo((string(msg) + to_string(value)).c_str());
}

/**print Error Message
 *
 * print the string version of `errno` along with user's custom message
*/
void printError(const char* msg) {
    int saved_errno = errno;
    fprintf(stderr, "\n[ERROR] : %s \n[ERROR code %d ] : %s\n", msg, saved_errno,
            strerror(saved_errno));
}
