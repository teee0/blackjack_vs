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
        jucator->show_cards(0,true);  
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
            int nr_maini=jucator->hands_count();
            for(int i=0; i < nr_maini; i++)
            {  
                if(jucator->hands_count() > 1)
                    std::cout << "\tMâna " << i+1 << ": " ;
                jucator->show_cards(i);

                int state = jucator->checkState(i);

                if (state == LOST)
                {
                    std::cout << jucator->last_state_text(i) << '\n';
                    jucator->stop(i);
                }
                else if (state == WON)
                {
                    std::cout << jucator->last_state_text(i) << '\n';
                    jucator->stop(i);
                    if (jucator->name() != "dealer")
                    {
                        RealPlayer* rp = dynamic_cast<RealPlayer*>(jucator);
                        rp->add_to_deposit(rp->wager(i));
                    }
                }
                else jucator->choice(i);
                //alegerea jucatorului
            }
            std::cout << '\n' << jucator->name() << ":\n";
            for(int i=0; i < jucator->hands_count(); i++)
            {
                if(jucator->hands_count() > 1)
                    std::cout << "\tMâna " << i+1 << ": " ;
                jucator->show_cards(i);
                if(jucator->checkState(i)!=0)
                {
                    std::cout<<jucator->last_state_text(i)<<'\n';
                }
            }
            
        }
        //else-uri de last loop
        if(jucatori.back()->checkState()==1)
        {
            for(auto jucator:jucatori){
                for(int i=0; i < jucator->hands_count(); i++)
                {
                    if(jucator->checkState(i)!=1)
                    {
                        std::cout<<jucator->name()<<": ";
                        if(jucator->hands_count() > 1)
                            std::cout << "\tMâna " << i+1 << ": " ;
                        std::cout<<" a câștigat!\n";
                        if(jucator->name() != "dealer")
                        {
                            RealPlayer* rp = dynamic_cast<RealPlayer*>(jucator);
                            rp->add_to_deposit(rp->wager(i));
                        }
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
                for (int hand_index = 0; hand_index < jucator->hands_count(); hand_index++) 
                {
                    std::cout<<jucator->name();
                    if(jucator->checkState(hand_index)!=1 && jucator->score(hand_index) > jucatori.back()->score())
                    {
                        
                        std::cout <<" a câștigat!\n";
                        if(jucator->name() != "dealer")
                            {
                                RealPlayer* rp = dynamic_cast<RealPlayer*>(jucator);
                                rp->add_to_deposit(rp->wager(hand_index));
                            }
                        castiga_dealeru=false;
                    }
                    else
                        std::cout<< " a pierdut!\n";
                }
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
