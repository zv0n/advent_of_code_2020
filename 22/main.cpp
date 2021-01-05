#include <algorithm>
#include <cstring>
#include <fstream>
#include <iostream>
#include <sstream>
#include <unordered_map>
#include <vector>

enum PlayerWon {
    PLAYER1,
    PLAYER2,
};

class Player {
  public:
    Player() = delete;
    Player( std::vector<int> deck ) : _deck( deck ) {
        std::reverse( _deck.begin(), _deck.end() );
    }

    int getCard() {
        int ret = _deck.back();
        _deck.pop_back();
        return ret;
    }

    void won( int mine, int others ) {
        _deck.insert( _deck.begin(), mine );
        _deck.insert( _deck.begin(), others );
    }

    bool hasCards() {
        return !_deck.empty();
    }

    size_t deckSize() {
        return _deck.size();
    }

    std::vector<int> subDeck( int size ) const {
        std::vector<int> ret{};
        for ( size_t i = _deck.size() - 1; i >= _deck.size() - size; i-- ) {
            if ( i == (size_t)-1 )
                break;
            ret.push_back( _deck[i] );
        }
        return ret;
    }

    std::string deckToString() const {
        std::string ret{};
        for ( size_t i = _deck.size() - 1; i > 0; i-- ) {
            ret += _deck[i] + 'A';
        }
        ret += _deck[0] + 'A';
        return ret;
    }

    size_t deckValue() {
        size_t multiplier = 0;
        size_t ret = 0;
        for ( auto &val : _deck ) {
            multiplier++;
            ret += val * multiplier;
        }
        return ret;
    }

  private:
    std::vector<int> _deck;
};

std::pair<std::vector<int>, std::vector<int>> getDecks( std::ifstream &file ) {
    std::vector<int> player1{};
    std::vector<int> player2{};
    int playerIndex = -1;

    std::string str{};
    while ( std::getline( file, str ) ) {
        if ( !std::strncmp( "Player", str.c_str(), 6 ) ) {
            playerIndex++;
            continue;
        }
        if ( str == "" )
            continue;
        switch ( playerIndex ) {
        case 0:
            player1.push_back( std::stoi( str.c_str() ) );
            break;
        case 1:
            player2.push_back( std::stoi( str.c_str() ) );
        default:
            break;
        }
    }

    return { player1, player2 };
}

void combat( Player &player1, Player &player2 ) {
    while ( player1.hasCards() && player2.hasCards() ) {
        int p1card = player1.getCard();
        int p2card = player2.getCard();
        if ( p1card > p2card ) {
            player1.won( p1card, p2card );
        } else {
            player2.won( p2card, p1card );
        }
    }
}

PlayerWon recursiveCombat( Player &player1, int player1_card, Player &player2,
                           int player2_card, bool initial = false ) {
    Player *p1sub;
    Player *p2sub;
    Player p1sub_real( { 0 } );
    Player p2sub_real( { 0 } );
    if ( initial ) {
        p1sub = &player1;
        p2sub = &player2;
    } else {
        p1sub_real = Player( player1.subDeck( player1_card ) );
        p2sub_real = ( player2.subDeck( player2_card ) );
        p1sub = &p1sub_real;
        p2sub = &p2sub_real;
    }
    std::unordered_map<std::string, int> p1history{};
    std::unordered_map<std::string, int> p2history{};
    while ( p1sub->hasCards() && p2sub->hasCards() ) {
        auto p1string = p1sub->deckToString();
        auto p2string = p2sub->deckToString();
        if ( p1history[p1string] != 0 || p2history[p2string] != 0 ) {
            return PLAYER1;
        }
        p1history[p1sub->deckToString()] = 1;
        p2history[p2sub->deckToString()] = 1;

        int p1card = p1sub->getCard();
        int p2card = p2sub->getCard();

        if ( p1card <= (int)p1sub->deckSize() &&
             p2card <= (int)p2sub->deckSize() ) {
            auto winner = recursiveCombat( player1, p1card, player2, p2card );
            if ( winner == PLAYER1 ) {
                p1sub->won( p1card, p2card );
            } else {
                p2sub->won( p2card, p1card );
            }
        } else {
            if ( p1card > p2card ) {
                p1sub->won( p1card, p2card );
            } else {
                p2sub->won( p2card, p1card );
            }
        }
    }
    if ( p1sub->hasCards() )
        return PLAYER1;
    return PLAYER2;
}

int main() {
    std::ifstream input_file( "input" );
    auto decks = getDecks( input_file );

    Player player1( decks.first );
    Player player2( decks.second );

    combat( player1, player2 );

    size_t res = 0;
    std::string winner_str = "";
    if ( player1.hasCards() ) {
        res = player1.deckValue();
        winner_str = "Player 1";
    } else {
        res = player2.deckValue();
        winner_str = "Player 2";
    }

    std::cout << "The winner's (\033[91;1m" << winner_str
              << "'s\033[0m) deck has value \033[93;1m" << res << "\033[0m"
              << std::endl;

    player1 = Player( decks.first );
    player2 = Player( decks.second );

    auto winner = recursiveCombat( player1, 0, player2, 0, true );

    if ( winner == PLAYER1 ) {
        res = player1.deckValue();
        winner_str = "Player 1";
    } else {
        res = player2.deckValue();
        winner_str = "Player 2";
    }
    std::cout << "The recursive combat winner's (\033[91;1m" << winner_str
              << "'s\033[0m) deck has value \033[93;1m" << res << "\033[0m"
              << std::endl;
}
