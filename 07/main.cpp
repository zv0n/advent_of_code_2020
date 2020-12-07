#include <fstream>
#include <iostream>
#include <sstream>
#include <unordered_map>
#include <unordered_set>
#include <vector>

enum Contains {
    YES,
    NO,
    DUNNO,
};

class Bag {
  public:
    Bag() = default;
    Bag( const std::string &type ) : _type( type ) {}

    void setType( const std::string &type ) {
        _type = type;
    }

    void addRule( const std::string &type, int count ) {
        _rules[type] = count;
    }

    void printRules() {
        std::cout << "BAG \"" << _type << "\":" << std::endl;
        for ( const auto &entry : _rules ) {
            std::cout << "\t" << entry.first << " - " << entry.second
                      << std::endl;
        }
    }

    const std::string &getType() const {
        return _type;
    }

    const std::unordered_map<std::string, int> &getRules() const {
        return _rules;
    }

  private:
    std::string _type;
    std::unordered_map<std::string, int> _rules;
};

std::unordered_map<std::string, Bag> getBags( std::ifstream &file ) {
    std::unordered_map<std::string, Bag> ret{};
    std::string str;
    std::string type;
    while ( std::getline( file, str ) ) {
        bool empty = str.find( "no other" ) != std::string::npos;
        std::stringstream ss( str );
        ss >> type;
        ss >> str;
        type += " " + str;
        ret[type] = Bag( type );
        if ( empty )
            continue;

        Bag &current_bag = ret[type];
        ss >> str; // bags
        ss >> str; // contain
        int count;
        while ( str != "bags." && str != "bag." ) {
            ss >> count;
            ss >> type;
            ss >> str;
            type += " " + str;
            current_bag.addRule( type, count );
            ss >> str; // bags
        }
    }
    return ret;
}

void findBagsThatContain( const std::string &type,
                          const std::unordered_map<std::string, Bag> &bags,
                          const std::string &investigated,
                          std::unordered_map<std::string, Contains> &cache ) {
    if ( cache[investigated] != DUNNO )
        return;
    const Bag &cur_bag = bags.at( investigated );
    for ( const auto &rule : cur_bag.getRules() ) {
        if ( rule.first == type ) {
            cache[investigated] = YES;
            break;
        }
        findBagsThatContain( type, bags, rule.first, cache );
        if ( cache[rule.first] == YES ) {
            cache[investigated] = YES;
            break;
        }
    }
    if ( cache[investigated] == DUNNO )
        cache[investigated] = NO;
}

std::unordered_set<std::string>
findBagsThatContain( const std::string &type,
                     const std::unordered_map<std::string, Bag> &bags ) {
    // I thought I might print the bags that are valid, that's why I'm using set
    // here instead of a counter
    std::unordered_set<std::string> ret{};
    std::unordered_map<std::string, Contains> cache{};
    for ( auto &entry : bags ) {
        cache[entry.first] = DUNNO;
    }
    for ( auto &entry : bags ) {
        findBagsThatContain( type, bags, entry.first, cache );
        if ( cache[entry.first] == YES )
            ret.insert( entry.first );
    }
    return ret;
}

int bagsRequired( const std::string &type,
                  const std::unordered_map<std::string, Bag> &bags ) {
    size_t required = 0;
    // we'll just assume that the input isn't TOO evil with reusing bag types
    // so we won't be using cache
    for ( auto &rule : bags.at( type ).getRules() ) {
        required +=
            rule.second + rule.second * bagsRequired( rule.first, bags );
    }
    return required;
}

int main() {
    std::ifstream input_file( "input" );
    auto bags = getBags( input_file );
    auto contains = findBagsThatContain( "shiny gold", bags );
    auto res = contains.size();
    std::cout << "There are \033[93;1m" << res
              << "\033[0m bags that eventually contain a shiny gold bag (I'd "
                 "show which, but there's way too many)"
              << std::endl;
    res = bagsRequired( "shiny gold", bags );
    std::cout << "Shiny gold bag requires \033[93;1m" << res
              << "\033[0m additional bags" << std::endl;
}
