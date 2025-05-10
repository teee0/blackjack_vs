#include "blackjack.h"



int main()
{
    Deck d;
    current_deck = d;
    
    Runda prima_runda;
    prima_runda.start();
    prima_runda.first_round();
    prima_runda.game_loop();

    return 0;
}
