//
// Created by mara on 02.12.2023.
//
#include "../../../headers/server.h"

// QUIT - terminate connection for this client

void Server::cmd_QUIT(int controlConnectionfd) {
    Send(controlConnectionfd, "Terminating Connection", 221);
    close(controlConnectionfd);
    exit(0);
}
