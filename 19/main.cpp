#include <cstring>
#include <fstream>
#include <iostream>
#include <sstream>
#include <unordered_set>
#include <vector>

class Rule {
  public:
    Rule() {
        index = -1;
    }
    Rule( int num ) {
        index = num;
    }

    void addRule( std::vector<int> rule ) {
        rules_numbers.push_back( rule );
    }

    void addRule( const std::string &rule ) {
        rules_strings.insert( rule );
    }

    void addRule( char rule ) {
        rules_strings.insert( std::string( 1, rule ) );
    }

    void convertRules( std::vector<Rule> &rule_vector ) {
        if ( !converted ) {
            og_rules_numbers = rules_numbers;
            og_rules_strings = rules_strings;
            for ( auto &rule : rules_numbers ) {
                std::vector<std::string> tmp_strings = { "" };
                for ( auto &item : rule ) {
                    std::vector<std::string> tmp_tmp_strings{};
                    rule_vector[item].convertRules( rule_vector );
                    for ( auto &new_string :
                          rule_vector[item].getStringRules() ) {
                        for ( auto &old_string : tmp_strings ) {
                            tmp_tmp_strings.push_back( old_string +
                                                       new_string );
                        }
                    }
                    tmp_strings = tmp_tmp_strings;
                }
                for ( auto &converted : tmp_strings ) {
                    rules_strings.insert( converted );
                }
            }
            converted = true;
        }
    }

    const std::unordered_set<std::string> &getStringRules() const {
        return rules_strings;
    }

    int getIndex() {
        return index;
    }

  private:
    int index;
    bool converted = false;
    std::vector<std::vector<int>> rules_numbers;
    std::unordered_set<std::string> rules_strings;
    std::vector<std::vector<int>> og_rules_numbers;
    std::unordered_set<std::string> og_rules_strings;
};

std::pair<std::vector<Rule>, std::vector<std::string>>
getInput( std::ifstream &file ) {
    std::vector<Rule> rules{};
    std::vector<std::string> strings{};

    std::string str{};
    int index;
    char tmp_c;
    int tmp_int;
    while ( std::getline( file, str ) ) {
        if ( str == "" )
            break;
        std::stringstream ss( str );
        ss >> index;
        if ( index >= rules.size() ) {
            rules.resize( index + 1 );
        }
        rules[index] = Rule( index );
        ss >> tmp_c;
        ss.get(); // space
        if ( ss.peek() == '"' ) {
            // we assume that rules with letters only have 1 letter
            // and no additional possible rules
            ss >> tmp_c; // "
            ss >> tmp_c;
            rules[index].addRule( tmp_c );
        } else {
            // we assume that no rule with numbers has a char
            std::vector<int> rule;
            while ( ss >> tmp_int ) {
                rule.push_back( tmp_int );
                ss.get();
                if ( ss.peek() == '|' ) {
                    rules[index].addRule( rule );
                    rule.clear();
                    ss.get();
                }
            }
            rules[index].addRule( rule );
        }
    }
    while ( std::getline( file, str ) ) {
        strings.push_back( str );
    }

    return { rules, strings };
}

std::pair<int, std::vector<std::string>>
howManyMatch( const std::vector<std::string> &strings,
              const std::unordered_set<std::string> &rules ) {
    int ret = 0;
    std::vector<std::string> invalid{};
    for ( auto &string : strings ) {
        if ( rules.find( string ) != rules.end() )
            ret++;
        else
            invalid.push_back( string );
    }
    return { ret, invalid };
}

bool matchesPart2(const std::string &input, size_t pos, int count42, int count31, const std::vector<Rule> &rules) {
    if(count31 >= count42 && count31 != 0)
        return false;
    if(count31 == 0) {
        for(auto &rule : rules[42].getStringRules()) {
            if(!strncmp(input.c_str() + pos, rule.c_str(), rule.length())) {
                if(pos + rule.length() == input.length())
                    return false; // we need at least 1 31
                if(matchesPart2(input, pos + rule.length(), count42 + 1, count31, rules))
                    return true;
            }
        }
    }
    for(auto &rule : rules[31].getStringRules()) {
        if(!strncmp(input.c_str() + pos, rule.c_str(), rule.length())) {
            if(pos + rule.length() == input.length()) {
                if((count31 + 1) < count42)
                    return true;
                return false;
            }
            if(matchesPart2(input, pos + rule.length(), count42, count31 + 1, rules))
                return true;
        }
    }
    return false;
}

int howManyMatch2( const std::vector<std::string> &strings,
                   const std::vector<Rule> &rules ) {
    int ret = 0;
    for ( std::string string : strings ) {
        if(matchesPart2(string, 0, 0, 0, rules))
            ret++;
    }
    return ret;
}

int main() {
    std::ifstream input_file( "input" );
    auto input = getInput( input_file );

    auto &rules = input.first;
    auto &strings = input.second;

    rules[0].convertRules( rules );

    auto res = howManyMatch( strings, rules[0].getStringRules() );
    std::cout << "\033[93;1m" << res.first << "\033[0m strings are valid"
              << std::endl;

    res.first += howManyMatch2( res.second, rules );
    std::cout << "\033[93;1m" << res.first
              << "\033[0m strings are ACTUALLY valid" << std::endl;
}
