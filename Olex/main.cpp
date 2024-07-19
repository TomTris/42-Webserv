

#include "webserv.hpp"

void launching(std::vector<Server>& servers) {
    // Create kqueue
    int kq = kqueue();
    if (kq == -1) {
        perror("kqueue");
        exit(EXIT_FAILURE);
    }

    // Register all server sockets with kqueue
    struct kevent change;
    for (size_t i = 0; i < servers.size(); i++) {
        int serverFd = servers[i].serverFd;

        // Ensure the serverFd is valid
        if (serverFd <= 0) {
            std::cerr << "Invalid server file descriptor: " << serverFd << std::endl;
            continue;
        }

        // Set up EV_SET
        EV_SET(&change, serverFd, EVFILT_READ, EV_ADD, 0, 0, NULL);
        if (kevent(kq, &change, 1, NULL, 0, NULL) == -1) {
            perror("kevent (register server socket)");
            exit(EXIT_FAILURE);
        }
    }

    struct kevent events[servers.size() * 2];
    while (true) {
        int num_events = kevent(kq, NULL, 0, events, servers.size() * 2, NULL);
        if (num_events == -1) {
            perror("kevent");
            exit(EXIT_FAILURE);
        }

        for (int i = 0; i < num_events; ++i) {
            int fd = events[i].ident;

            // Check if this is a server socket
            bool is_server_socket = false;
            for (size_t j = 0; j < servers.size(); j++) {
                if (fd == servers[j].serverFd) {
                    is_server_socket = true;
                    break;
                }
            }

            if (is_server_socket) {
                // Accept new connection
                struct sockaddr_in client_addr;
                socklen_t client_len = sizeof(client_addr);
                int new_socket = accept(fd, (struct sockaddr *)&client_addr, &client_len);
                if (new_socket < 0) {
                    perror("accept");
                    continue;
                }

                printf("New connection on socket fd %d\n", new_socket);

                // Register new socket with kqueue
                EV_SET(&change, new_socket, EVFILT_READ, EV_ADD, 0, 0, NULL);
                if (kevent(kq, &change, 1, NULL, 0, NULL) == -1) {
                    perror("kevent (register client socket)");
                    close(new_socket);
                    continue;
                }
            } else if (events[i].filter == EVFILT_READ) {
                // Handle client data
                char buffer[1024];
                ssize_t bytes_read = read(fd, buffer, sizeof(buffer));
                if (bytes_read < 0) {
                    perror("read");
                    close(fd);
                } else if (bytes_read == 0) {
                    // Connection closed by client
                    printf("Client closed connection on socket fd %d\n", fd);
                    close(fd);
                } else {
                    // Process received data
                    printf("Received %zd bytes from socket fd %d:\n%.*s\n", bytes_read, fd, (int)bytes_read, buffer);
                    // Continue to monitor the socket for more data
                    EV_SET(&change, fd, EVFILT_READ, EV_ADD, 0, 0, NULL);
                    if (kevent(kq, &change, 1, NULL, 0, NULL) == -1) {
                        perror("kevent (re-register client socket)");
                        close(fd);
                    }
                }
            }
        }
    }

    close(kq);
    // Close server sockets
    for (size_t i = 0; i < servers.size(); i++) {
        close(servers[i].serverFd);
    }
}


int main()
{
    std::vector<server_t> s;
    if (parse("/Users/obrittne/Desktop/webSerc/Configs/default.conf", s))
        return 1;
    std::vector<Server> servers;
    for (int i = 0; i < s.size(); i++)
    {
        servers.push_back(Server(s[i]));
    }
    launching(servers);
}
//test