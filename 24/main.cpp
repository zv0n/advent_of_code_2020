#include <cstring>
#include <fstream>
#include <iostream>
#include <sstream>
#include <unordered_map>
#include <vector>

class Floor {
  public:
    Floor() = delete;
    Floor( long min_x, long min_y, long max_x, long max_y ) {
        _floor.resize( max_x - min_x + 1 );
        for ( auto &column : _floor ) {
            column.resize( max_y - min_y + 1 );
        }
        center = { -min_x, -min_y };
    }
    Floor( long min_x, long min_y, long max_x, long max_y, long days )
        : Floor( min_x - 2 * days, min_y - days, max_x + 2 * days,
                 max_y + days ) {}

    void flip( std::pair<long, long> &coordinates ) {
        auto x = center.first + coordinates.first;
        auto y = center.second + coordinates.second;
        _floor[x][y] = !_floor[x][y];
    }

    long blackTiles() {
        long ret = 0;
        for ( auto &column : _floor ) {
            for ( auto tile : column ) {
                if ( tile )
                    ret++;
            }
        }
        return ret;
    }

    void live() {
        auto _floor_copy = _floor;
        for ( size_t i = 0; i < _floor.size(); i++ ) {
            for ( size_t j = 0; j < _floor[i].size(); j++ ) {
                auto neighbours = getNeighbours( i, j );
                if ( _floor[i][j] && ( neighbours == 0 || neighbours > 2 ) ) {
                    _floor_copy[i][j] = false;
                } else if ( !_floor[i][j] && neighbours == 2 ) {
                    _floor_copy[i][j] = true;
                }
            }
        }
        _floor = _floor_copy;
    }

  private:
    size_t getNeighbours( long x, long y ) {
        size_t ret = 0;
        if ( x > 1 && _floor[x - 2][y] )
            ret++;
        if ( x > 0 && y > 0 && _floor[x - 1][y - 1] )
            ret++;
        if ( x > 0 && y < (long)_floor[0].size() - 1 && _floor[x - 1][y + 1] )
            ret++;
        if ( x < (long)_floor.size() - 2 && _floor[x + 2][y] )
            ret++;
        if ( x < (long)_floor.size() - 1 && y > 0 && _floor[x + 1][y - 1] )
            ret++;
        if ( x < (long)_floor.size() - 1 && y < (long)_floor[0].size() - 1 &&
             _floor[x + 1][y + 1] )
            ret++;
        return ret;
    }
    std::vector<std::vector<bool>> _floor{};
    std::pair<long, long> center;
};

std::vector<std::pair<long, long>> getInstructions( std::ifstream &file ) {
    std::vector<std::pair<long, long>> ret{};
    long min_x = 0;
    long min_y = 0;
    long max_x = 0;
    long max_y = 0;

    std::string str{};
    while ( std::getline( file, str ) ) {
        long x = 0;
        long y = 0;
        for ( size_t i = 0; i < str.length(); i++ ) {
            auto x_addition = 2;
            if ( str[i] == 's' ) {
                x_addition = 1;
                y += 1;
                i++;
            } else if ( str[i] == 'n' ) {
                x_addition = 1;
                y -= 1;
                i++;
            }
            if ( str[i] == 'e' ) {
                x -= x_addition;
            } else if ( str[i] == 'w' ) {
                x += x_addition;
            } else {
                std::cerr << "UNKNOWN INSTRUCTION! '" << str[i] << "'"
                          << std::endl;
            }
        }
        ret.emplace_back( x, y );
        if ( x < min_x )
            min_x = x;
        else if ( x > max_x )
            max_x = x;

        if ( y < min_y )
            min_y = y;
        else if ( y > max_y )
            max_y = y;
    }
    ret.emplace_back( min_x, min_y );
    ret.emplace_back( max_x, max_y );

    return ret;
}

int main() {
    std::ifstream input_file( "input" );
    auto instructions = getInstructions( input_file );
    auto max = instructions.back();
    instructions.pop_back();
    auto min = instructions.back();
    instructions.pop_back();

    auto days = 100;
    Floor floor( min.first, min.second, max.first, max.second, days );
    for ( auto &instruction : instructions ) {
        floor.flip( instruction );
    }
    std::cout << "There are \033[93;1m" << floor.blackTiles()
              << "\033[0m black tiles" << std::endl;

    for ( int i = 0; i < days; i++ ) {
        floor.live();
    }
    std::cout << "After " << days << " days there are \033[93;1m"
              << floor.blackTiles() << "\033[0m black tiles" << std::endl;
}
