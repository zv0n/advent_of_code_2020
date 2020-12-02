#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>

class PasswordValidator {
  public:
    PasswordValidator( int min, int max, char c, std::string password ) {
        _min = min;
        _max = max;
        _c = c;
        _password = password;
    }

    bool valid() const {
        int count = 0;
        for ( auto &x : _password ) {
            if ( x == _c )
                count++;
        }
        return count >= _min && count <= _max;
    }

    bool valid2() const {
        int count = 0;
        if ( _password[_min - 1] == _c )
            count++;
        if ( _password[_max - 1] == _c )
            count++;
        return count == 1;
    }

  private:
    int _min;
    int _max;
    char _c;
    std::string _password;
};

std::vector<PasswordValidator> getPasswords( std::ifstream &file ) {
    std::vector<PasswordValidator> ret{};
    int min = 0;
    int max = 0;
    char c = 0;
    std::string str;
    while ( std::getline( file, str ) ) {
        std::stringstream ss( str );
        ss >> min;
        ss >> max;
        max *= -1;
        ss >> c;
        ss >> str; // :
        ss >> str; // password
        ret.emplace_back( min, max, c, str );
    }
    return ret;
}

int validPasswords( const std::vector<PasswordValidator> &passwords,
                    int part = 1 ) {
    int count_part1 = 0;
    int count_part2 = 0;
    for ( auto &password : passwords ) {
        if ( password.valid() )
            count_part1++;
        if ( password.valid2() )
            count_part2++;
    }
    if ( part == 1 )
        return count_part1;
    return count_part2;
}

int main() {
    std::ifstream input_file( "input" );
    auto passwords = getPasswords( input_file );
    auto res = validPasswords( passwords );
    std::cout << "There are \033[93;1m" << res << "\033[0m valid passwords"
              << std::endl;
    res = validPasswords( passwords, 2 );
    std::cout << "There are \033[93;1m" << res
              << "\033[0m ACTUALLY valid passwords" << std::endl;
}
