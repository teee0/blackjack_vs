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
private:
    double bet;
public:
    InvalidBetException(double bet) : bet(bet) {}

    const char* what() const noexcept override {
        std::string message = "Pariul " + std::to_string(bet) + " este invalid!";
        return message.c_str();
    }
};

class CardDeckEmptyException : public BlackjackException {
public:
    const char* what() const noexcept override {
        return "Pachetul de cărți este gol!";
    }
};

class InvalidActionException : public BlackjackException {
private:
    std::string action;
public:
    InvalidActionException(const std::string& act) : action(act) {}

    const char* what() const noexcept override {
        return ("Acțiunea '" + action + "' este invalidă în această situație!").c_str();
    }
};

#endif