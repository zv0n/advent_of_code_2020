#include <cstring>
#include <fstream>
#include <iostream>
#include <sstream>
#include <unordered_set>
#include <vector>

class Cup {
  public:
    Cup() = delete;
    Cup( long id ) : _id( id ) {}

    void setNext( Cup *cup ) {
        _next = cup;
    }
    long getId() {
        return _id;
    }
    Cup *getNext() {
        return _next;
    }

  private:
    long _id;
    Cup *_next = nullptr;
};

class CupCircle {
  public:
    CupCircle() = delete;

    CupCircle( std::vector<long> cups, size_t total_count = 0 ) {
        // input cups go from 1 to 9
        max = cups.size();
        min = 1;
        for ( long i = min; i <= max; i++ ) {
            _cups.emplace_back( i );
        }
        while ( max < (long)total_count ) {
            max++;
            _cups.emplace_back( max );
        }

        _head = &_cups[cups[0] - 1];
        for ( size_t i = 0; i < cups.size() - 1; i++ ) {
            _cups[cups[i] - 1].setNext( &_cups[cups[i + 1] - 1] );
        }
        if ( total_count == 0 ) {
            // total_count == 0 means that we only want the input cups
            _cups[cups.back() - 1].setNext( _head );
            return;
        } else {
            _cups[cups.back() - 1].setNext( &_cups[cups.size()] );
        }
        for ( size_t i = cups.size(); i < _cups.size() - 1; i++ ) {
            _cups[i].setNext( &_cups[i + 1] );
        }
        _cups.back().setNext( _head );
    }

    void performStep() {
        long forbidden[3];

        Cup *move_start = _head->getNext();
        forbidden[0] = move_start->getId();

        Cup *move_end = move_start->getNext();
        forbidden[1] = move_end->getId();

        move_end = move_end->getNext();
        forbidden[2] = move_end->getId();

        _head->setNext( move_end->getNext() );

        auto destination = _head->getId();
        do {
            destination -= 1;
            if ( destination < min )
                destination = max;
        } while ( destination == forbidden[0] || destination == forbidden[1] || destination == forbidden[2] );
        Cup *destination_cup = &_cups[destination - 1];

        move_end->setNext( destination_cup->getNext() );
        destination_cup->setNext( move_start );
        _head = _head->getNext();
    }

    std::string getResult() {
        Cup *cur_cup = &_cups[0];
        cur_cup = cur_cup->getNext();

        std::string ret{};
        for ( size_t i = 1; i < _cups.size(); i++ ) {
            ret += std::to_string( cur_cup->getId() );
            cur_cup = cur_cup->getNext();
        }
        return ret;
    }

    size_t getResultP2() {
        Cup *cur_cup = &_cups[0];
        return cur_cup->getNext()->getId() *
               cur_cup->getNext()->getNext()->getId();
    }

  private:
    Cup *_head;
    std::vector<Cup> _cups;
    long max;
    long min;
};

std::vector<long> getCups( std::ifstream &file ) {
    std::vector<long> cups{};

    std::string str{};
    while ( std::getline( file, str ) ) {
        for ( auto &c : str ) {
            cups.push_back( c - '0' );
        }
    }

    return cups;
}

void playRounds( CupCircle &circle, size_t rounds ) {
    for ( size_t i = 0; i < rounds; i++ ) {
        circle.performStep();
    }
}

int main() {
    std::ifstream input_file( "input" );
    auto input = getCups( input_file );

    CupCircle circle( input );
    playRounds( circle, 100 );

    std::cout << "The final cup circle looks as follows: \033[93;1m"
              << circle.getResult() << "\033[0m" << std::endl;

    CupCircle circleP2( input, 1000000 );
    playRounds( circleP2, 10000000 );

    std::cout << "The product of cups with stars in them is \033[93;1m"
              << circleP2.getResultP2() << "\033[0m" << std::endl;
}
