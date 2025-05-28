#ifndef CARDS_
#define CARDS_

#include <string>
#include <iostream>
#include <iomanip> //setw, setfill
#include <vector>
#include <algorithm> //shuffle
#include <random> //random_device
#include <format>
#include <list>
#include <utility> //pair

#include "blackjack_exception.h"

template <typename T>
auto find_next(const std::list<T>& set, const T& val) {
    auto it = std::find(std::begin(set), std::end(set), val);
    if (it != set.end()) it++;
    return it;
}

struct SpelledOutFormat {
    static const std::string print_suits[4];
    static std::string format(int suit_id) {
        return print_suits[suit_id];
    }
};

struct UnicodeFormat {
    static const std::string print_suits[4];
    static std::string format(int suit_id) {
        return print_suits[suit_id];
    }
};


template <typename SuitFormat = UnicodeFormat>//posibil sa nu mearga in terminalul din windows folosind unicode dinafara ascii
class Card
{
    static const std::string print_values[13];
    int number;
    int suit;
    int real_value;
public:
    Card(int number,int suit);
    bool operator==(const Card& c) const
    {
        return real_value==c.real_value;
    }

    void show() const;
    int get_value() const {return real_value;}
};

class Deck
{
    std::vector<Card<>> cards;
public:
    Deck();
    Deck(Deck&);
    void shuffle();
    Card<> give();
    void show_cards() const
    {    for (auto card : cards)
        card.show(), std::cout<<' ';
        std::cout<<std::endl;
    };
};

class Hand
{
private:
    std::vector<Card<>> cards;
    bool _can_take_cards = 1;
    bool _hasSplit = 0;
    bool _hasAce = 0;
    std::string _last_state_text="joacă";
    double _wager = 0;
public:
    Hand(): cards() {}
    //modificare
    void pop();
    void push(Card<>);
    Hand split();
    
    //detalii
    int sum()  const;
    int size() const {return cards.size();}

    
    //gettere
    std::string last_state_text(int hand_index=0) const {return _last_state_text;}
    double wager()                                const {return _wager;}
    bool can_take_cards()                         const {return _can_take_cards;}
    bool can_split()                              const;
    bool hasAce()                                 const {return _hasAce;}
    
    //settere
    void set_last_state_text(std::string new_text) {_last_state_text=new_text;}
    void change_wager(double new_wager) {_wager=new_wager;}
    void stop();

    //afișare
    void show_cards(bool hide_second_card = false) const;
    friend std::ostream& operator<< (std::ostream &, const Hand&);
};

#endif
