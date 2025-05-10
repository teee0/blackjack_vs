#ifndef BLACKJACK_
#define BLACKJACK_

#include "players.h"


class Runda
{
private:
    std::vector<Player*> jucatori;
public:
    Runda(): jucatori(){}
    ~Runda() {
        for (auto p : jucatori) delete p;
    }
    void start();
    void first_round();
    void game_loop();
    bool is_stalled();
};

#endif // BLACKJACK_
