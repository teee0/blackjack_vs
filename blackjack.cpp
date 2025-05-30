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
            if (jucator->last_state_text() == "joacă")
            {
                std::cout << '\n' << jucator->name() << ":\n";
                jucator->show_cards(false);

                int state = jucator->checkState();
                if (state == PLAYING) 
                {
                    jucator->choice(jucatori);//alegerea jucatorului
                    std::cout << '\n' << jucator->name() << ":\n";
                    jucator->show_cards(false);
                }
                if(jucator->checkState() != PLAYING) {
                    std::cout << jucator->last_state_text() << '\n';
                }
            }
        }
        //else-uri de last loop
        if(jucatori.back()->checkState()==1)
        {
            for(auto jucator:jucatori)
                if(jucator->checkState()!=1)//inclusiv fara dealer
                    jucator->set_last_state_text("a câștigat");

            end(); return;
        }
        if(is_stalled() )
        {
            bool castiga_dealeru=true;
            for (auto jucator : jucatori)
            {
                if (jucator->name() != "dealer")
                {
                    if (jucator->checkState()!=1 && jucator->score() >= jucatori.back()->score())
                    {
                        jucator->set_last_state_text("a câștigat");
                        castiga_dealeru=false;
                    }
                    else jucator->set_last_state_text("a pierdut");
                }
            }   
            jucatori.back()->set_last_state_text((castiga_dealeru)?"a câștigat":"a pierdut");
            end(); return;
        } 
    }
        
}

void Runda::end()
{
    std::cout << "\n" << std::setfill('~') << std::setw(40) << "" << std::setw(40) <<"\n";
    for(auto jucator : jucatori)
    {
        std::cout << '\n' << jucator->name() <<": "<< jucator->last_state_text() << "!\n";
        jucator->show_cards(false);

        if (jucator->name() != "dealer" 
            && jucator->last_state_text() != "a pierdut"  
            && jucator->last_state_text() != "a dat surrender"
        ){
            RealPlayer* rp = dynamic_cast<RealPlayer*>(jucator);
            rp->add_to_deposit(rp->wager());
        }
    }
}

bool Runda::is_stalled()
{
    bool rez=0;
    for (auto& jucator: jucatori)
        rez|=jucator->can_take_cards();
    return !rez;
}
