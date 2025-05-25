#include "blackjack.h"


void swap(Runda& r1, Runda& r2) noexcept {
    std::swap(r1, r2);
}
Runda& Runda::operator=(Runda ob) {
    if (this != &ob)
        swap(*this, ob);
    return *this;
}
Runda::Runda(const Runda& ob) {
    for (auto p : ob.jucatori)
        jucatori.push_back(p->clone());
}

void Runda::start()
{
    std::cout << std::setfill('~') << std::setw(40) << "BlackJack" << std::setw(40) <<" "<< std::endl;
    
    //nr. of players
    //for(no) cin etc
    int nr_jucatori;
    std::cout<<"Câți joacă: ", std::cin>>nr_jucatori;
    for(int i=1; i<=nr_jucatori; i++)
    {
        std::cout<<"\n("<<i<<") ";
        jucatori.push_back(new RealPlayer());
        dynamic_cast<RealPlayer*>(jucatori.back())->place_bet();
    }
    jucatori.push_back(new Dealer("dealer"));
}

void Runda::first_round()
{
    
    for(auto jucator : jucatori){
        jucator->hit(), jucator->hit();
        
        std::cout << (jucator->name()) << ": ";
        jucator->show_cards(true);  
    }
}


enum states{PLAYING, LOST, WON};

void Runda::game_loop()
{
    while(1)
    {
        std::cout << "\n" << std::setfill('~') << std::setw(40) << "" << std::setw(40) <<"\n";
        for(auto jucator : jucatori)
        {
            std::cout << '\n' << jucator->name() << ":\n";
            jucator->show_cards(false);

            int state = jucator->checkState();

            if (state == LOST)
            {
                std::cout << jucator->last_state_text() << '\n';
                jucator->stop();
            }
            else if (state == WON)
            {
                std::cout << jucator->last_state_text() << '\n';
                jucator->stop();
                if (jucator->name() != "dealer")
                {
                    RealPlayer* rp = dynamic_cast<RealPlayer*>(jucator);
                    rp->add_to_deposit(rp->wager());
                }
            }
            else jucator->choice(jucatori);
            //alegerea jucatorului
            std::cout << '\n' << jucator->name() << ":\n";
            jucator->show_cards(false);
            if(jucator->checkState()!=0)
            {
                std::cout<<jucator->last_state_text()<<'\n';
            }

        }
        //else-uri de last loop
        if(jucatori.back()->checkState()==1)
        {
            for(auto jucator:jucatori){
            if(jucator->checkState()!=1)
            {
                std::cout<<jucator->name()<<": ";
                std::cout<<" a câștigat!\n";
                if(jucator->name() != "dealer")
                {
                    RealPlayer* rp = dynamic_cast<RealPlayer*>(jucator);
                    rp->add_to_deposit(rp->wager());
                }
            }
            
            }
            return;
        }
        if(is_stalled() )
        {
            bool castiga_dealeru=true;
            for(int i=0; i<jucatori.size()-1; i++)
            {
                auto jucator = jucatori[i];
                std::cout<<jucator->name();
                if(jucator->checkState()!=1 && jucator->score() > jucatori.back()->score())
                {
                    std::cout <<" a câștigat!\n";
                    if(jucator->name() != "dealer")
                    {
                        RealPlayer* rp = dynamic_cast<RealPlayer*>(jucator);
                        rp->add_to_deposit(rp->wager());
                    }
                    castiga_dealeru=false;
                }
                else
                    std::cout<< " a pierdut!\n";
            }   
            if(castiga_dealeru) std::cout<<"dealerul câștigă!\n";
            return;
        } 
    }
        
}
bool Runda::is_stalled()
{
    bool rez=0;
    for (auto& jucator: jucatori)
        rez|=jucator->can_take_cards();
    return rez;
}
