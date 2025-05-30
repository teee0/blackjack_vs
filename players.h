#ifndef PLAYERS_
#define PLAYERS_

#include "cards.h"
#include "choicestrat.h"

extern Deck current_deck;

class Player
{
protected:
    Hand hand;
    std::string _name;
    ChoiceStrategy* strategy;
    friend class RealPlayerStrategy;
public:
    Player(std::string _name="player"): _name(_name){}
    virtual ~Player() = default;
    virtual Player* clone() const = 0;

    //detalii
    virtual void show_cards(bool hide_second_card = false) const=0;

    std::string name() {return _name;}
    bool can_take_cards();//de schimbat cu comentariuu

    int score() ;
    void stop() {hand.stop();}
    void hit();
    void stand();
    void choice(std::list<Player*>& jucatori)
    {
        if (strategy)
            strategy->choice(this, jucatori);
    }

    int checkState();

    std::string last_state_text() {return hand.last_state_text();}

    void set_last_state_text(std::string new_text) {hand.set_last_state_text(new_text);}
};

class Dealer : public Player
{
    friend class DealerStrategy;
    public:
    Dealer(std::string _name="player"): Player(_name)
    {
        strategy = new DealerStrategy();
    }
    Player* clone() const;
    
    void show_cards(bool hide_second_card) const;
    
};

class RealPlayer : public Player
{
protected:
    int  deposit = 5000;
    bool hasSurrendered = 0;
    int  split_count = 0;
    RealPlayer* jucator_asociat;
public:
    RealPlayer();
    Player* clone() const;
    //acțiuni
    void doubleDown();
    void surrender();// de inclocuit cu partea comentata
    virtual void split(std::list<Player*>& jucatori);
    //detalii
    bool can_double_down();
    bool can_surrender();
    //puncte
    void add_to_deposit(double);
    double wager(){ return hand.wager(); }
    void change_wager(double new_wager);
    void place_bet();
    //afișări
    void show_cards(bool hide_second_card) const;
};

class GhostPlayer : public RealPlayer
{
private:
public:
    GhostPlayer(RealPlayer* rp, Hand hand);
    friend void insert_player(GhostPlayer* p, std::list<Player*>& jucatori);
};
 
class PlayerFactory {
public:
    virtual ~PlayerFactory() = default;
    virtual Player* createPlayer() = 0;
};

class RealPlayerFactory : public PlayerFactory {
public:
    Player* createPlayer() { return new RealPlayer();}
};

class DealerFactory : public PlayerFactory {
public:
    Player* createPlayer() { return new Dealer("dealer"); }
};

class GhostPlayerFactory : public PlayerFactory {
    RealPlayer* jucator_asociat;
    Hand hand;
public:
    GhostPlayerFactory(RealPlayer* rp, Hand h) : jucator_asociat(rp), hand(h) {}
    Player* createPlayer() { return new GhostPlayer(jucator_asociat, hand); }
};

#endif
