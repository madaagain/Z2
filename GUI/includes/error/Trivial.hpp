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
    class Trivial : public std::exception {
    public:

        Trivial(const std::string errorMessage) : _errorMessage("Trivial Error: " + errorMessage){};
        ~Trivial() = default;

        const char *what() const noexcept override { return _errorMessage.c_str(); };

    private:

        const std::string _errorMessage;
    };
} // namespace Lib
