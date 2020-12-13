#include <algorithm>
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>

std::pair<size_t, std::vector<size_t>> getBuses( std::ifstream &file ) {
    std::vector<size_t> ret{};
    std::string str;
    std::getline( file, str );
    size_t timestamp = std::stoll( str );
    size_t tmp;
    while ( std::getline( file, str ) ) {
        tmp = 0;
        for ( auto &c : str ) {
            if ( c == ',' ) {
                ret.push_back( tmp );
                tmp = 0;
            } else if ( c == 'x' ) {
                tmp = 0;
            } else {
                tmp *= 10;
                tmp += c - '0';
            }
        }
        ret.push_back( tmp );
    }
    return { timestamp, ret };
}

size_t findEarliestBus( const std::pair<size_t, std::vector<size_t>> &input ) {
    size_t min_id = 0;
    size_t min_wait = -1;
    for ( auto &bus : input.second ) {
        if ( bus == 0 )
            continue;
        size_t since_last = input.first % bus;
        size_t till_next = bus - since_last;
        if ( till_next < min_wait ) {
            min_id = bus;
            min_wait = till_next;
        }
    }
    return min_id * min_wait;
}

size_t
findMagicTimestamp( const std::pair<size_t, std::vector<size_t>> &input ) {
    std::vector<std::pair<size_t, size_t>> minutes{};
    size_t minute = -1;
    for ( auto &bus : input.second ) {
        minute++;
        if ( bus == 0 )
            continue;
        minutes.emplace_back( bus - ( minute % bus ), bus );
    }
    size_t timestamp = 0;
    size_t multiplier = minutes[0].second;
    for ( size_t i = 1; i < minutes.size(); i++ ) {
        while ( ( timestamp % minutes[i].second ) != minutes[i].first ) {
            timestamp += multiplier;
        }
        multiplier *= minutes[i].second;
    }
    return timestamp;
}

int main() {
    std::ifstream input_file( "input" );
    auto input = getBuses( input_file );

    auto res = findEarliestBus( input );
    std::cout << "The earliest bus to the airport has ID \033[93;1m" << res
              << "\033[0m" << std::endl;
    auto res2 = findMagicTimestamp( input );
    std::cout << "The earliest timestamp when all busses leave one after "
                 "another is \033[93;1m"
              << res2 << "\033[0m" << std::endl;
}
