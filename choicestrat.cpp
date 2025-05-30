#include "choicestrat.h"
#include "players.h"
#include "blackjack_exception.h" 
#include <iostream>

std::string actions = "\
[1]: Hit\n\
[2]: Stand\n\
[3]: Double down\n\
[4]: Split\n\
[5]: Surrender";



void RealPlayerStrategy::choice(Player* p, std::list<Player*>& jucatori) {
        RealPlayer* rp = dynamic_cast<RealPlayer*>(p);
        if (!rp) return;

        std::cout<<actions<<'\n';
        if (!rp->hand.can_take_cards())
        {
            std::cout<<rp->_name<<" nu mai poate lua cărți\n";
            for (auto jucator : jucatori)
                std::cout<<jucator->name()<<" "<<jucator->can_take_cards()<< "\n";
        }
        else{
            bool canDo[5] = { 1, 1, rp->can_double_down(), rp->hand.can_split(), rp->can_surrender() };

            std::cout<<"Alegere"<<" "<<rp->_name<<": ";
            int input;
            while(true)
            {
                try{
                    if(!(std::cin >> input) || input < 1 || input > 5 || !canDo[input-1]) throw InvalidActionException();
                    break;
                }
                catch(InvalidActionException er)
                {
                    std::cin.clear();
                    std::cin.ignore(1000, '\n');
                    std::cout << er.what()<< '\n';
                }
            }
            switch (input) {
            case 1: rp->hit(); break;
            case 2: rp->stand(); break;
            case 3: rp->doubleDown(); break;
            case 4: rp->split(jucatori); break;
            case 5: rp->surrender(); break;
            }

        }
    }

void DealerStrategy::choice(Player* p, std::list<Player*>& jucatori) {
        Dealer* d = dynamic_cast<Dealer*>(p);
        if (!d) return;

        if (d->hand.sum() < 17 &&
            !(d->hand.hasAce()
              && d->hand.sum() + 10 > 17
              && d->hand.sum() + 10 <= 21))
        {
            std::cout << d->name() << " a tras o carte\n";
            d->hit();
        }
        else {
            d->stand();
        }
    }