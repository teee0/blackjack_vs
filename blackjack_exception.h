#ifndef BLACKJACK_EX
#define BLACKJACK_EX
#include <iostream>
#include <exception>
#include <string>

class BlackjackException : public std::exception {
public:
    const char* what() const noexcept override {
        return "Eroare generală!";
    }
};

class InvalidBetException : public BlackjackException {
public:
    const char* what() const noexcept override {
        return "Pariul este invalid!";
    }
};

class EmptyDeckException : public BlackjackException {
public:
    const char* what() const noexcept override {
        return "Pachetul de cărți este gol!";
    }
};

class InvalidActionException : public BlackjackException {
public:
    const char* what() const noexcept override {
        return "Acțiunea este invalidă în această situație!";
    }
};

#endif