#include "cards.h"
Deck current_deck;

template <typename SuitFormat>
const std::string Card<SuitFormat>::print_values[13] = {
    "A","2","3","4","5","6","7","8","9","10","J","Q","K"
};

const std::string SpelledOutFormat::print_suits[4] = {
    "Spade","Heart","Diamond","Club"
};

const std::string UnicodeFormat::print_suits[4] = 
{
    "♠","♥","♦","♣"
};
template <typename SuitFormat>
Card<SuitFormat>::Card(int number, int suit): number(number), suit(suit)
{
    if(number>10) real_value = 10;
    else real_value = number;
}

template <typename SuitFormat>
void Card<SuitFormat>::show() const
{
    std::cout << print_values[number-1] << SuitFormat::format(suit-1);
}

template <typename SuitFormat>
std::ostream& operator<< (std::ostream &out, const Card<SuitFormat>& ob)
{
    ob.show();//inlocuieste eventual
    return out;
}

Deck::Deck()
{
    for(int iValue=1; iValue<=13; iValue++)
        for(int iSuit=1; iSuit<=4; iSuit++)
            cards.push_back(Card<>(iValue,iSuit));
    shuffle();
}
Deck::Deck(Deck& ob)
{
    cards=ob.cards;
}

Card<> Deck::give()
{
    if (cards.empty())
            throw EmptyDeckException();
    Card<>card = cards.back();
    cards.pop_back();
    return card;
}

void Deck::shuffle()
{
     std::shuffle(cards.begin(), cards.end(), std::random_device());
}

void Hand::pop()
{
	cards.pop_back();
}

void Hand::push(Card<>c)
{
	cards.push_back(c);
    if(c.get_value()==1) _hasAce=true;
}
int  Hand::sum() const {
    int s=0;
    for (auto card : cards)
        s+=card.get_value();
    return s;
}
void Hand::stop()
{
    _can_take_cards=0;
}

Hand Hand::split()
{
    Hand new_hand;
    new_hand.push(cards[1]), pop();
    new_hand.change_wager(wager());
    return new_hand;
}
bool Hand::can_split() const
    {
        return !_hasSplit &&
                size()==2 &&
                cards[0]==cards[1];
    } 
//afișări
void Hand::show_cards(bool hide_second_card) const
{
    for (int i=0; i<cards.size(); i++)
        {
            if(hide_second_card && i==1) 
                std::cout<<"#?";
            else 
                cards[i].show();
            std::cout<<' ';
        }
    std::cout<<std::endl;
}

std::ostream& operator<< (std::ostream &out, const Hand& ob)
{
    ob.show_cards();//inlocuieste eventual
    return out;
}

template <typename T>
bool isSorted(const T& min, const T& val, const T& max) {
    return val > min && val < max;
}
template <>
bool isSorted(const Card<>& val, const Card<>& min, const Card<>& max) {
    return val.number > min.number && val.number < max.number;
}