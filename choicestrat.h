#ifndef CHOICE_STRATEGY_
#define CHOICE_STRATEGY_

#include <list>

class Player;
class RealPlayer;
class Dealer;

class ChoiceStrategy {
public:
    virtual ~ChoiceStrategy() = default;
    virtual void choice(Player* p, std::list<Player*>& jucatori) = 0;
};

class RealPlayerStrategy : public ChoiceStrategy {
public:
    void choice(Player* p, std::list<Player*>& jucatori);
};

class DealerStrategy : public ChoiceStrategy {
public:
    void choice(Player* p, std::list<Player*>& jucatori);
};

#endif 