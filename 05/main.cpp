#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>

int rows[] = { 0b01000000, 0b00100000, 0b00010000, 0b00001000,
               0b00000100, 0b00000010, 0b00000001 };
int cols[] = { 0b00000100, 0b00000010, 0b00000001 };

class BoardingPass {
  public:
    BoardingPass( const std::string &input ) {
        for ( int i = 0; i < 7; i++ ) {
            if ( input[i] == 'B' )
                row |= rows[i];
        }
        for ( int j = 7; j < 10; j++ ) {
            if ( input[j] == 'R' )
                col |= cols[j - 7];
        }
    }

    int getID() const {
        return row * 8 + col;
    }

  private:
    int row = 0;
    int col = 0;
};

std::vector<BoardingPass> getBoardingPasses( std::ifstream &file ) {
    std::vector<BoardingPass> ret{};
    std::string str;
    while ( std::getline( file, str ) ) {
        ret.emplace_back( str );
    }
    return ret;
}

int findMaxID( const std::vector<BoardingPass> &passes ) {
    int max = 0;
    for ( auto &pass : passes ) {
        auto id = pass.getID();
        if ( id > max )
            max = id;
    }
    return max;
}

int findMySeat( const std::vector<BoardingPass> &passes ) {
    std::vector<bool> seats( 1024, false );
    for ( auto &pass : passes ) {
        seats[pass.getID()] = true;
    }
    for ( size_t i = 1; i < seats.size(); i++ ) {
        if ( !seats[i] && seats[i - 1] && seats[i + 1] )
            return i;
    }
    return -1;
}

int main() {
    std::ifstream input_file( "input" );
    auto passes = getBoardingPasses( input_file );
    auto res = findMaxID( passes );
    std::cout << "Boarding pass with highest ID has ID #\033[93;1m" << res
              << "\033[0m" << std::endl;
    auto seat = findMySeat( passes );
    std::cout << "I have found my seat! It is #\033[93;1m" << seat << "\033[0m"
              << std::endl;
}
