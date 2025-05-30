#include "players.h"

void RealPlayer::show_cards(bool hide_second_card) const
{
    hand.show_cards();
}

void Dealer::show_cards(bool hide_second_card) const
{
    hand.show_cards(hide_second_card);
}

bool RealPlayer::can_double_down()
{
    return deposit-hand.wager() >= 0;
}
bool RealPlayer::can_surrender()
{
    return (hand.size() == 2 && split_count == 0);
    //pt un ghost player v-a returna tot timpul fals
    //preferabil de inlocuit cu un split_count transmis prin referinta
}


void Player::hit()
{
    try{
        hand.push(current_deck.give());
    }
    catch(EmptyDeckException er)
    {
        Deck new_deck;
        current_deck=new_deck;
        hand.push(current_deck.give());
    }
}

void Player::stand()
{
    hand.stop();
}

int Player::score() 
{
    int s = hand.sum();
    if(hand.hasAce() && s+10<=21) return s+10; 
    return s;
}


int Player::checkState()
{ 
    /*0=nimic
      1=pierdut
      2=câștigat*/
    if (hand.sum() > 21) {
        hand.set_last_state_text ("a pierdut"); 
        stop(); 
        return 1;
    }
    else if (hand.size() == 5) 
        hand.set_last_state_text ("a câștigat având 5 cărți");
    else if (hand.sum() == 21 || (hand.sum() + 10 == 21 && hand.hasAce())) 
        hand.set_last_state_text ("a câștigat cu un blackjack");
    else if (hand.sum() + 10 == 21 && hand.hasAce() && hand.size() == 2) 
        hand.set_last_state_text ("a câștigat cu un blackjack din prima");  
    else return 0; 
    stop();

    return 2; 
}

RealPlayer::RealPlayer():Player(), jucator_asociat(this)
{
    std::cout << "Nume: ";
    std::cin >> _name;
    
}
Player* RealPlayer::clone() const
{
    return new RealPlayer(*this);
}
Player* Dealer::clone() const
{
    return new Dealer(*this);
}

void RealPlayer::doubleDown()
{
    change_wager(hand.wager());
    hit();
    stand();
}
void RealPlayer::surrender()
{
    hasSurrendered = 1;
    hand.stop();
    hand.set_last_state_text ("chose to surrender!");
    add_to_deposit(hand.wager()/2);//n-are cum să fie alta decât prima
}

void RealPlayer::split(std::vector<Player*>& jucatori)
{
    jucator_asociat->split_count++;

    Hand new_hand;
    new_hand = hand.split();

    GhostPlayer *gp = new GhostPlayer(jucator_asociat, new_hand);
    add_player(gp, jucatori);

    deposit -= new_hand.wager();
    hit();
    gp->hit();
    //check deposit
}
void RealPlayer::place_bet()
{
    double bet;
    std::cout<<"Câte jetoane pui în joc? (acum ai: " <<deposit<<")\n";
    while(true)
    {
        try{
            if(!(std::cin >> bet) || bet > deposit) throw InvalidBetException();
            break;
        }
        catch(InvalidBetException er)
        {
            std::cin.clear();
            std::cin.ignore(1000, '\n');
            std::cout << er.what()<< '\n';
        }
    }

    change_wager(bet);
}
void RealPlayer::add_to_deposit(double suma)
{
    deposit += suma;
    std::cout<<std::fixed<<std::setprecision(2)<<
    "Suma de "<<suma<<" a fost returnată.\n";
}

void RealPlayer::change_wager(double new_wager)
{
    deposit -= new_wager-hand.wager();
    hand.change_wager(new_wager);
}
std::string actions = "\
[1]: Hit\n\
[2]: Stand\n\
[3]: Double down\n\
[4]: Split\n\
[5]: Surrender";

void RealPlayer::choice(std::vector<Player*>& jucatori)
{
    std::cout<<actions<<'\n';
    if (!hand.can_take_cards())
    {
        std::cout<<_name<<" nu mai poate lua cărți\n";
        for (auto jucator : jucatori)
            std::cout<<jucator->name()<<" "<<jucator->can_take_cards()<< "\n";
    }
    else{
        bool canDo[5] = { 1, 1, can_double_down(), hand.can_split(), can_surrender() };


        std::cout<<"Alegere"<<" "<<_name<<": ";
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
        case 1: hit(); break;
        case 2: stand(); break;
        case 3: doubleDown(); break;
        case 4: split(jucatori); break;
        case 5: surrender(); break;
        }

    }
}


void Dealer::choice(std::vector<Player*>& jucatori)
{
    if (hand.sum()<17 &&
        !(hand.hasAce() && hand.sum()+10>17 && hand.sum()+10<=21)) 
        {
            std::cout<<name()<<" "<<"a tras o carte";
            hit();
        }
    else stand();
}

bool Player::can_take_cards()
{
    return hand.can_take_cards();
}


GhostPlayer::GhostPlayer(RealPlayer* rp, Hand hand) 
{
    jucator_asociat = rp;
    this->hand = hand;
    split_count = -1;
    _name = _name + " mâna " + std::to_string(split_count+1);
}

void add_player(GhostPlayer* p, std::vector<Player*>& jucatori)
{
    auto it = std::find(jucatori.begin(), jucatori.end(), p->jucator_asociat);
    if (it != jucatori.end()) jucatori.insert(it + 1, p);
    else                      throw "nu e bun add_player";//jucatori.push_back(p);
}