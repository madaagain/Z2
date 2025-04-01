/*
** EPITECH PROJECT, 2024
** lib
** File description:
** MyTest
*/

#pragma once

#include <exception>
#include <string>

namespace Lib
{
    class Critical : public std::exception {
    public:

        Critical(const std::string errorMessage) : _errorMessage("Critical Error: " + errorMessage){};
        ~Critical() = default;

        const char *what() const noexcept override { return _errorMessage.c_str(); };

    private:

        const std::string _errorMessage;
    };
} // namespace Lib
