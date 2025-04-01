/*
** EPITECH PROJECT, 2023
** B-YEP-400-PAR-4-1-zappy-alexis.mer
** File description:
** client.hpp
*/

#pragma once

    #define ERROR (84)

#include <string>
#include <iostream>

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/select.h>
#include <unistd.h>
#include <stdio.h>

#include <asio.hpp>


#include "error/Critical.hpp"

class Client {
    public:

        Client() : _port(-1), _ip(""), _io_context(), _sockfd(_io_context) {}
        ~Client() = default;

        // to connect to the server
        int connectToServer();

        // to communicate with server
        int infiniteLoop();

        // Getter
        std::string getIP() const { return _ip; };
        int getPort() const { return _port; };
        int getSockfd() { return _sockfd.native_handle(); }

        // Setter
        void setPort(int port) { _port = port; return; };
        void setIp(std::string ip) { _ip = ip; return; };

    private:
        // Encapsulation
        void functionSelect() {
            struct timeval timeout;
            timeout.tv_sec = 0;
            timeout.tv_usec = 0;
            if (select(getSockfd() + 1, &readfd, NULL, NULL, &timeout) < 0)
                throw Lib::Critical(": select failed");
            return; };
        void functionClose(int sockfd) {
            if (close(sockfd) == -1)
                throw Lib::Critical("Close fail");
            return; };

        int _port;
        std::string _ip;
        asio::io_context _io_context;
        asio::ip::tcp::socket _sockfd;
        struct sockaddr_in _client_info;
        fd_set readfd;
};
