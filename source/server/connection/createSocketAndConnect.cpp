//
// Created by mara on 02.12.2023.
//

#include "../../../headers/server.h"


/**create socket and connect to host
 *
 * @usage
 * creates a socket, and simply tried to connect to host
 *
 * @returns
 * the connection file descriptor
*/
int createSocketAndConnectToHost(const char* host, int portNumber) {
    std::string portStr = std::to_string(portNumber);
    const char* port = portStr.c_str();
    struct addrinfo hints;
    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;

    struct addrinfo* res;
    int addr_status = getaddrinfo(host, port, &hints, &res);
    if (addr_status != 0) {
        printError("[ERROR] : Cannot get info from getaddrinfo()");
        return -1;
    }

    int sock_fd;
    int connect_status;
    const int maxAttempts = 5;
    const int delaySeconds = 2;

    for (int attempt = 0; attempt < maxAttempts; ++attempt) {
        if (attempt > 0) {
            sleep(delaySeconds); // Delay between attempts
        }

        for (struct addrinfo* p = res; p != NULL; p = p->ai_next) {
            sock_fd = socket(p->ai_family, p->ai_socktype, p->ai_protocol);
            if (sock_fd < 0) {
                continue; // Try next address
            }

            connect_status = connect(sock_fd, p->ai_addr, p->ai_addrlen);
            if (connect_status == 0) {
                freeaddrinfo(res); // Free resources before returning
                return sock_fd; // Connection successful
            }

            close(sock_fd); // Close this socket before next attempt
        }
    }

    freeaddrinfo(res); // Free resources if all attempts fail
    printError("[SERVER] : Failed to Connect to HOST after retries");
    return -1; // Return error code
}


//int createSocketAndConnectToHost(const char* host, int portNumber) {
//    /**Converting port number to const char*
//     *
//     * Because all system calls expect it in const char* format.
//    */
//    //const char* port = to_string(portNumber).c_str();
//    std::string portStr = std::to_string(portNumber);
//    const char* port = portStr.c_str();
//
//
//    /**hints is of type : struct addrinfo
//     *
//     * These are hints you provide to the `getaddrinfo()` function.
//     * Fill it up as much as possible and just pass it to `getaddrinfo()`
//     * It will load up remaining values and give us a perfect structure
//     * Which can then be used in other system calls.
//    */
//    struct addrinfo hints;
//
//    /**Wiping the structure initially
//     *
//     * Setting all the memory bits to 0 using memset
//    */
//    memset(&hints, 0, sizeof hints);
//
//    /**AF_UNSPEC :over: AF_INET
//     *
//     * AF_UNSPEC makes us protocol unspecific
//     * It will then handle IPv4 and well as IPv6
//     *
//     * Traditional AF_INET will only handle IPv4 addresses
//    */
//    hints.ai_family = AF_UNSPEC;
//
//    /**SOCK_STREAM
//     *
//     * Other popular type of sockets are : SOCK_DGRAM
//    */
//    hints.ai_socktype = SOCK_STREAM;
//
//    /**getaddrinfo() :over: gethostbyname() and getservbyname()
//     *
//     * getaddrinfo() is an excellent function that will return information
//     * on a particular host name (such as its IP address) and
//     * load up a struct sockaddr for you, taking care of the gritty details
//     * (like if it's IPv4 or IPv6.)
//     *
//     * Note that, this `res` is extremely useful and helps in other system calls.
//     * Consequently, making them IP version independent
//     *
//     * Here, we will provide `host` ourself, so no AI_PASSIVE flag is used,
//     * like we did in server
//    */
//    struct addrinfo* res;
//    int addr_status = getaddrinfo(host, port, &hints, &res);
//    if (addr_status != 0) {
//        printError("[ERROR] : Cannot get info from getaddrinfo()");
//        throw std::runtime_error("Exiting : Cannot resolve HOST");
//    }
//
//    /**Sockfd is the socket descriptor
//     *
//    */
//    int sock_fd;
//
//    /**Loop through results, bind to first one we can
//     *
//     * getaddrinfo returns a linkedlist of results
//     * majorly all the publicly visible IP addresses of the host and related info
//     * (eg. for google.com, it emits its IPv4 [Node 1] and IPv6 address[Node 2])
//    */
//    struct addrinfo* p;
//    for (p = res; p != NULL; p = p->ai_next) {
//        /** Create the socket
//         *
//        */
//        sock_fd = socket(p->ai_family, p->ai_socktype, p->ai_protocol);
//        if (sock_fd < 0) {
//            printError("[SERVER] : Cannot Open Socket");
//            continue;  // try next
//        }
//
//        /**No Bind necessary.
//         *
//         * This is because, we don't care what port we use to connect to server.
//         * System will choose this for us, and convey to server.
//         *
//         * Moreover, we don't want to listen. We just want to connect.
//         * So we can do that directly without binding
//        */
//
//        /* Connect to the HOST
//        *
//        */
//        int connect_status = connect(sock_fd, p->ai_addr, p->ai_addrlen);
//        if (connect_status != 0) {
//            printError("[SERVER] : Cannot Connect to this HOST IP");
//
//            char s[INET6_ADDRSTRLEN];
//            get_ip_str((const struct sockaddr*)p->ai_addr, s, sizeof s);
//            fprintf(stdout, "[SERVER:CONNECTION:FAIL] Can't Connect to  %s\n", s);
//
//            close(sock_fd);  // close this socket
//            continue;		 // try another socket
//        }
//
//        // Connect to the first one we can
//        break;
//    }
//
//    // No Connection happened in the above loop
//    if (p == NULL) {
//        printError("[SERVER] : Failed to Connect to HOST");
//        // returning will end the program and res will automatically be freed
//        throw std::runtime_error("Exiting : SERVER failed to establish connection");
//    }
//
//    // Print out IP address
//    char s[INET6_ADDRSTRLEN];
//    get_ip_str((const struct sockaddr*)p->ai_addr, s, sizeof s);
//    fprintf(stdout, "\n[SERVER-INITIATED:CONNECTION] Connected to  %s\n", s);
//
//    /* Don't need the structure with address info any more
//     *
//     * This is not called before the above `if condition` to prevent
//     * overriding the value of `errno` by this system call.
//     */
//    freeaddrinfo(res);
//
//    /** return the sockfd
//     *
//     * This is a socket descriptor [SOCKET]
//     * currently binded [BIND]
//     * to the supplied Server's IP address [HOST]
//     * and the supplied Server's port in the arguments [PORT]
//    */
//    return sock_fd;
//}