#include "players.h"

void RealPlayer::show_cards(int hand_index, bool hide_second_card) const
{
    hands[hand_index].show_cards();
}
void Dealer::show_cards(int hand_index, bool hide_second_card) const
{
    hands[hand_index].show_cards(hide_second_card);
}

bool RealPlayer::can_double_down(int hand_index)
{
    return deposit-hands[hand_index].wager() >= 0;
}
bool RealPlayer::can_surrender(int hand_index)
{
    return (hands[0].size() == 2 && hands.size() == 1);
}

void Player::hit(int hand_index)
{
    hands[hand_index].push(current_deck.give());
}
void Player::stand(int hand_index)
{
    hands[hand_index].stop();
}
int Player::checkState(int hand_index)
{ 
    /*0=nimic
      1=pierdut
      2=câștigat*/
    auto& hand = hands[hand_index];
    if      (hand.sum() > 21) {hand.set_last_state_text ("a pierdut!\n"); stop(); return 1;}
    else if (hand.size() == 5) hand.set_last_state_text ("a câștigat având 5 cărți!\n");
    else if (hand.sum() == 21) hand.set_last_state_text ("a câștigat cu un blackjack!\n");
    else if (hand.sum() + 10 == 21 && hand.hasAce() && hand.size() == 2) 
                               hand.set_last_state_text ("a câștigat cu un blackjack din prima!\n");  
    else return 0; 
    stop();
    return 2; 
}

RealPlayer::RealPlayer():Player()
{
    std::cout << "Nume: ";
    std::cin >> _name;
}

void RealPlayer::doubleDown(int hand_index)
{
    change_wager(hand_index, hands[hand_index].wager());
    hit(hand_index);
    stand(hand_index);
}
void RealPlayer::surrender()
{
    hasSurrendered = 1;
    hands[0].stop();
    hands[0].set_last_state_text ("chose to surrender!");
    add_to_deposit(hands[0].wager()/2);//n-are cum să fie alta decât prima
}
void RealPlayer::split(int hand_index)
{
    Hand new_hand;
    new_hand = hands[hand_index].split();
    hands.push_back(new_hand);
    deposit -= new_hand.wager();
    hit(hand_index);
    hit(hands_count()-1);//in caz ca se face split de mai multe ori
    //check deposit
    //poate has split
}

void RealPlayer::place_bet(int handIndex)
{
    double bet;
    std::cout<<"Câte jetoane pui în joc? (acum ai: " <<deposit<<")\n";
    while (!(std::cin >> bet) || bet < 5 || bet > deposit)
    {
        throw InvalidBetException(bet);
        std::cin.clear();
        std::cin.ignore(1000, '\n');
        std::cout << "Input invalid; Încercă iar\n";
    }
    change_wager(handIndex, bet);
}
void RealPlayer::add_to_deposit(double suma)
{
    deposit += suma;
    std::cout<<std::fixed<<std::setprecision(2)<<
    "Suma de "<<suma<<" a fost returnată.\n";
}

std::string actions = "\
[1]: Hit\n\
[2]: Stand\n\
[3]: Double down\n\
[4]: Split\n\
[5]: Surrender";
void RealPlayer::choice(int hand_index)
{   Hand hand = hands[hand_index];
    std::cout<<actions<<'\n';
    if (!hand.can_take_cards()) std::cout<<"Mâna "<<hand_index+1<<" nu mai poate lua cărți\n";
        else{
            bool canDo[5] = { 1, 1, can_double_down(hand_index), hand.can_split(), can_surrender() };

            std::cout<<"Alegere"<<(hands_count()>1?" mână "+std::to_string(hand_index+1):"")<<": ";
            int input;
            while (!(std::cin >> input) || input < 1 || input > 5 || !canDo[input-1]) {
                std::cin.clear();
                std::cin.ignore(1000, '\n');
                std::cout << "Input invalid; Încearcă iar\n";
            }
            switch (input) {
            case 1: hit(hand_index); break;
            case 2: stand(hand_index); break;
            case 3: doubleDown(hand_index); break;
            case 4: split(hand_index); break;
            case 5: surrender(); break;
            }

        }
}
void Dealer::choice(int hand_index)
{
    if (hands[hand_index].sum()<17 &&
        !(hands[hand_index].hasAce() && hands[hand_index].sum()+10>17 && hands[hand_index].sum()+10<=21)) 
        {
            std::cout<<name()<<" "<<"a tras o carte";
            hit();
        }
    else stand();
}

