#ifndef PLAYERS_
#define PLAYERS_

#include "cards.h"

extern Deck current_deck;

class Player
{
public:
    std::vector<Hand> hands/*={{1,1},{2,4},{12,2}}*/;
    std::string _name;
public:
    Player(std::string _name="player"): hands(1), _name(_name){}
    ~Player(){}

    virtual void show_cards(int hand_index = 0, bool hide_second_card = false) const=0;
    std::string name() {return _name;}
    int hands_count(){return hands.size();}
    bool can_take_cards(){
        bool rez=0;
        for(auto hand: hands) rez|=hand.can_take_cards();
        return rez;
    }
    int score(int hand_index = 0) {return hands[hand_index].sum();}
    void stop(int hand_index = 0) {hands[hand_index].stop();}

    void hit(int hand_index = 0);
    void stand(int hand_index = 0);

    virtual void choice(int hand_index = 0) = 0;
    int checkState(int hand_index = 0);
    std::string last_state_text(int hand_index=0) {return hands[hand_index].last_state_text();}
    void set_last_state_text(int hand_index, std::string new_text) {hands[hand_index].set_last_state_text(new_text);}

    void lose_hand(int hand_index=0){hands.erase(hands.begin()+hand_index);}
};

class Dealer : public Player
{
    public:
    void choice(int hand_index = 0);
    void show_cards(int hand_index=0, bool hide_second_card=false) const;
    Dealer(std::string _name="player"): Player(_name){}
};

class RealPlayer : public Player
{
private:
    int deposit=5000;
    bool hasSurrendered = 0;
public:
    RealPlayer();
    //acțiuni
    void doubleDown(int hand_index = 0);
    void surrender();
    void split(int hand_index = 0);
    void choice(int hand_index = 0);
    //detalii
    bool can_double_down(int hand_index = 0);
    bool can_surrender(int hand_index = 0);
    //puncte
    void add_to_deposit(double);
    double wager(double hand_index = 0){ return hands[hand_index].wager(); }//da nu mere e de aici
    void change_wager(int hand_index, double new_wager)
    {
        deposit -= new_wager-hands[hand_index].wager();
        hands[hand_index].change_wager(new_wager);
    }
    void place_bet(int handIndex = 0);
    //afișări
    void show_cards(int hand_index, bool hide_second_card=false) const;
};
#endif
