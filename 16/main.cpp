#include <algorithm>
#include <fstream>
#include <iostream>
#include <sstream>
#include <unordered_set>
#include <vector>

class Limitation {
  public:
    Limitation() = delete;
    Limitation( const std::pair<int, int> &range_a,
                const std::pair<int, int> &range_b, const std::string &name )
        : _range_a( range_a ), _range_b( range_b ), _name( name ) {}

    bool isInRange( int num ) const {
        return ( num >= _range_a.first && num <= _range_a.second ) ||
               ( num >= _range_b.first && num <= _range_b.second );
    }

    const std::string &getName() const {
        return _name;
    }

  private:
    std::pair<int, int> _range_a;
    std::pair<int, int> _range_b;
    std::string _name;
};

std::tuple<std::vector<Limitation>, std::vector<int>,
           std::vector<std::vector<int>>>
getInput( std::ifstream &file ) {
    std::vector<Limitation> limits{};
    std::vector<int> myTicket;
    std::vector<std::vector<int>> otherTickets;
    std::string str;

    while ( std::getline( file, str ) ) {
        if ( str == "" )
            break;
        auto name_end = str.find( ":" );
        auto name = str.substr( 0, name_end );
        int a_1, a_2, b_1, b_2;
        char c;
        std::stringstream ss( str.substr( name_end + 1 ) );
        ss >> a_1;
        ss >> c;
        ss >> a_2;
        ss >> str;
        ss >> b_1;
        ss >> c;
        ss >> b_2;
        limits.emplace_back( std::pair<int, int>( a_1, a_2 ),
                             std::pair<int, int>( b_1, b_2 ), name );
    }
    std::getline( file, str ); // your ticket
    std::getline( file, str );
    std::stringstream my_ss( str );
    int my_value;
    char my_c;
    while ( my_ss >> my_value ) {
        myTicket.push_back( my_value );
        my_ss >> my_c;
    }
    std::getline( file, str ); // empty
    std::getline( file, str ); // nearby tickets

    while ( std::getline( file, str ) ) {
        std::vector<int> tmp{};
        int value;
        char c;
        std::stringstream ss( str );
        while ( ss >> value ) {
            tmp.push_back( value );
            ss >> c;
        }
        otherTickets.emplace_back( tmp );
    }

    return { limits, myTicket, otherTickets };
}

std::pair<size_t, std::vector<std::vector<int>>>
validateTickets( const std::vector<Limitation> &limits,
                 const std::vector<std::vector<int>> &tickets ) {
    size_t ret_num = 0;
    std::vector<std::vector<int>> valid_tickets{};
    for ( auto &ticket : tickets ) {
        bool valid = true;
        for ( auto &value : ticket ) {
            bool at_least_one = false;
            for ( auto &limit : limits ) {
                if ( limit.isInRange( value ) ) {
                    at_least_one = true;
                    break;
                }
            }
            if ( !at_least_one ) {
                ret_num += value;
                valid = false;
            }
        }
        if ( valid )
            valid_tickets.push_back( ticket );
    }
    return { ret_num, valid_tickets };
}

std::pair<size_t, std::vector<int>>
fieldOrder( const std::vector<Limitation> &limits,
            const std::vector<std::vector<int>> &valid_tickets,
            const std::vector<int> &your_ticket ) {
    std::vector<int> possibilities_initial{};
    std::vector<std::unordered_set<int>> possibilities;
    for ( size_t i = 0; i < limits.size(); i++ ) {
        possibilities_initial.push_back( i );
    }
    for ( size_t i = 0; i < limits.size(); i++ ) {
        possibilities.emplace_back( possibilities_initial.begin(),
                                    possibilities_initial.end() );
    }

    for ( auto &ticket : valid_tickets ) {
        for ( size_t i = 0; i < ticket.size(); i++ ) {
            for ( size_t j = 0; j < limits.size(); j++ ) {
                if ( !limits[j].isInRange( ticket[i] ) )
                    possibilities[i].erase( j );
            }
        }
    }

    std::vector<int> order{};
    order.resize( limits.size() );
    bool all_empty = false;
    while ( !all_empty ) {
        all_empty = true;
        for ( size_t i = 0; i < possibilities.size(); i++ ) {
            if ( possibilities[i].size() != 0 )
                all_empty = false;
            if ( possibilities[i].size() == 1 ) {
                order[i] = *possibilities[i].begin();
                for ( auto &possibility : possibilities ) {
                    possibility.erase( order[i] );
                }
            }
        }
    }

    size_t res = 1;
    for ( size_t i = 0; i < order.size(); i++ ) {
        if ( limits[order[i]].getName().substr( 0, 9 ) == "departure" )
            res *= your_ticket[i];
    }
    return { res, order };
}

int main() {
    std::ifstream input_file( "input" );
    auto input = getInput( input_file );
    auto &limits = std::get<0>( input );
    auto &my_ticket = std::get<1>( input );
    auto &tickets = std::get<2>( input );

    auto res = validateTickets( limits, tickets );
    auto error_rate = res.first;
    auto &valid_tickets = res.second;
    std::cout << "Error rate of nearby tickets is \033[93;1m" << error_rate
              << "\033[0m" << std::endl;

    auto res2 = fieldOrder( limits, valid_tickets, my_ticket );
    auto multiplication = res2.first;
    auto &field_order = res2.second;

    std::string order{};
    for ( size_t i = 0; i < field_order.size() - 1; i++ ) {
        order += limits[field_order[i]].getName() + ", ";
    }
    order += limits[field_order.back()].getName();

    std::cout << "Your ticket checksum is \033[93;1m" << multiplication
              << "\033[0m (the field order on your ticket is \033[91;1m"
              << order << "\033[0m)" << std::endl;
}
