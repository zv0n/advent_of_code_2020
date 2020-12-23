#include <cstring>
#include <fstream>
#include <iostream>
#include <sstream>
#include <unordered_set>
#include <vector>

class CupCircle {
  public:
    CupCircle() = delete;

    CupCircle( std::vector<long> cups, size_t total_count = 0 ) {
        max = total_count == 0 ? cups.size() : total_count;
        _cups.resize(max);
        for ( size_t i = 0; i < cups.size() - 1; i++ ) {
            _cups[cups[i] - 1] = cups[i+1] - 1;
        }
        for ( long i = cups.size(); i < max; i++ ) {
            _cups[i] = i+1;
        }

        _head = cups[0] - 1;
        if(total_count == 0) {
            _cups[cups.back() - 1] = _head;
        } else {
            _cups[cups.back() - 1] = cups.size();
            _cups.back() = _head;
        }
    }

    void performSteps(size_t steps) {
        long move_start = 0;
        long forbidden = 0;
        long move_end = 0;
        for(size_t i = 0; i < steps; i++) {
            move_start = _cups[_head];
            forbidden = _cups[move_start];
            move_end = _cups[forbidden];

            _cups[_head] = _cups[move_end];

            auto destination = _head;
            do {
                destination--;
                if ( destination < 0 )
                    destination = max - 1;
            } while ( destination == move_start || destination == forbidden || destination == move_end );

            _cups[move_end] = _cups[destination];
            _cups[destination] = move_start;
            _head = _cups[_head];
        }
    }

    std::string getResult() {
        auto cur_cup = _cups[0];

        std::string ret{};
        for ( size_t i = 1; i < _cups.size(); i++ ) {
            ret += std::to_string( cur_cup + 1 );
            cur_cup = _cups[cur_cup];
        }
        return ret;
    }

    size_t getResultP2() {
        return (_cups[0] + 1) * (_cups[_cups[0]] + 1);
    }

  private:
    long _head;
    std::vector<long> _cups;
    long max;
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

int main() {
    std::ifstream input_file( "input" );
    auto input = getCups( input_file );

    CupCircle circle( input );
    circle.performSteps(100);

    std::cout << "The final cup circle looks as follows: \033[93;1m"
              << circle.getResult() << "\033[0m" << std::endl;

    CupCircle circleP2( input, 1000000 );
    circleP2.performSteps(10000000);

    std::cout << "The product of cups with stars in them is \033[93;1m"
              << circleP2.getResultP2() << "\033[0m" << std::endl;
}
