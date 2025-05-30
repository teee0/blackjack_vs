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
    Runda(const Runda&);
    friend void swap(Runda& r1, Runda& r2) noexcept;
    Runda& operator=(Runda ob);

    void start();
    void end();
    void first_round();
    void game_loop();
    bool is_stalled();
};

#endif // BLACKJACK_
