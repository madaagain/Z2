/*
** EPITECH PROJECT, 2023
** B-YEP-400-PAR-4-1-zappy-alexis.mer
** File description:
** client.cpp
*/

#include <asio/read_until.hpp>
#include "Client.hpp"
#include "Window.hpp"
#include "GameData.hpp"
#include "ParserServer.hpp"
#include <unistd.h>
#include <chrono>
#include <thread>

int Client::connectToServer()
{
    if (_port == 0)
        Lib::Critical("invalide Port");

    asio::error_code ec;
    asio::ip::tcp::resolver resolver(_io_context);
    auto endpoints = resolver.resolve(_ip, std::to_string(_port));
    asio::connect(_sockfd, endpoints, ec);

    if (ec)
        Lib::Critical(ec.message());

    std::cout << "Successfully connected to server." << std::endl;

    return 0;
}

int Client::infiniteLoop()
{
    GameData gameData;
    ParserServer Parser;
    std::string dataBuff;
    bool isGuiRunning = false;
    std::shared_ptr<Window> window = NULL;

    asio::write(_sockfd, asio::buffer("GRAPHIC\n"));
    std::this_thread::sleep_for(std::chrono::microseconds(10000));
    asio::write(_sockfd, asio::buffer("msz\n"));
    std::this_thread::sleep_for(std::chrono::microseconds(10000));
    asio::write(_sockfd, asio::buffer("sgt\n"));
    std::this_thread::sleep_for(std::chrono::microseconds(10000));
    asio::write(_sockfd, asio::buffer("mct\n"));
    while (1) {
        FD_ZERO(&readfd);
        FD_SET(getSockfd(), &readfd);
        FD_SET(0, &readfd);
        functionSelect();
        std::string cmd;

        if (FD_ISSET(0, &readfd)) {
            if (!std::getline(std::cin, cmd)) {
                break;
            } else {
                cmd += "\n";
                asio::write(_sockfd, asio::buffer(cmd));
            }
        }

        if (FD_ISSET(getSockfd(), &readfd)) {
            std::string data;
            asio::error_code ec;
            asio::streambuf buff;
            asio::read_until(_sockfd, buff, '\n', ec);
            if (ec) {
                if (ec == asio::error::eof) {
                    std::cout << "Connection closed by server" << std::endl;
                    break;
                } else {
                    std::cerr << "Error on receive: " << ec.message() << std::endl;
                    functionClose(getSockfd());
                    return ERROR;
                }
            }
            std::istream istr(&buff);
            dataBuff.append(std::istreambuf_iterator<char>(istr), std::istreambuf_iterator<char>());
            size_t pos;
            while ((pos = dataBuff.find('\n')) != std::string::npos) {
                std::string completeLine = dataBuff.substr(0, pos + 1);
                dataBuff.erase(0, pos + 1);
                std::cerr << completeLine;
                Parser.parse(completeLine, gameData);
            }
        }
        if (gameData.getMapHeight() > 0 && gameData.getMapWidth() > 0 && !isGuiRunning) {
            isGuiRunning = true;
            window = std::make_shared<Window>(gameData);
            gameData.window = window;
        }
        if (window != NULL) {
            window->run(gameData);
        }
        if (isGuiRunning && (window == NULL || !window->isOpen()))
            break;
    }
    functionClose(getSockfd());
    return 0;
}
