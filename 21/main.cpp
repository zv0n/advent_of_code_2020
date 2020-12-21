#include <fstream>
#include <iostream>
#include <sstream>
#include <unordered_map>
#include <unordered_set>
#include <vector>

std::pair<std::unordered_map<std::string, std::unordered_set<std::string>>,
          std::vector<std::vector<std::string>>>
getMeals( std::ifstream &file ) {
    std::unordered_map<std::string, std::unordered_set<std::string>> alergens;
    std::vector<std::vector<std::string>> meals;

    std::string str{};
    while ( std::getline( file, str ) ) {
        std::vector<std::string> meal;
        std::stringstream ss( str );
        while ( ss >> str ) {
            if ( str[0] == '(' ) {
                break;
            }
            meal.push_back( str );
        }
        meals.push_back( meal );
        while ( ss >> str ) {
            if ( str.back() == ')' || str.back() == ',' )
                str = str.substr( 0, str.length() - 1 );

            if ( alergens[str].empty() ) {
                for ( auto &ingredient : meal )
                    alergens[str].insert( ingredient );
            } else {
                std::vector<std::string> remove{};
                for ( auto &ingredient : alergens[str] ) {
                    if ( std::find( meal.begin(), meal.end(), ingredient ) ==
                         meal.end() )
                        remove.push_back( ingredient );
                }
                for ( auto &removal : remove )
                    alergens[str].erase( removal );
            }
        }
    }

    return { alergens, meals };
}

size_t howMany( const std::unordered_set<std::string> &safe,
                const std::vector<std::vector<std::string>> &meals ) {
    size_t ret = 0;
    for ( auto &meal : meals ) {
        for ( auto &ingredient : meal ) {
            if ( safe.find( ingredient ) != safe.end() )
                ret++;
        }
    }
    return ret;
}

std::unordered_set<std::string> getSafeIngredients(
    const std::unordered_map<std::string, std::unordered_set<std::string>>
        &alergens,
    const std::vector<std::vector<std::string>> &meals ) {
    std::unordered_set<std::string> ret{};
    for ( auto &meal : meals ) {
        for ( auto &ingredient : meal ) {
            bool found = false;
            for ( auto &alergen : alergens ) {
                if ( alergen.second.find( ingredient ) !=
                     alergen.second.end() ) {
                    found = true;
                    break;
                }
            }
            if ( !found )
                ret.insert( ingredient );
        }
    }
    return ret;
}

std::vector<std::pair<std::string, std::string>> oneDangerPerIngredient(
    std::unordered_map<std::string, std::unordered_set<std::string>>
        alergens ) {
    std::vector<std::pair<std::string, std::string>> ret{};
    while ( !alergens.empty() ) {
        std::string removal_ingredient{};
        std::string removal_alergen{};
        for ( auto &alergen : alergens ) {
            if ( alergen.second.size() == 1 ) {
                removal_ingredient = *alergen.second.begin();
                removal_alergen = alergen.first;
                ret.emplace_back( removal_alergen, removal_ingredient );
                break;
            }
        }
        alergens.erase( removal_alergen );
        for ( auto &alergen : alergens ) {
            alergen.second.erase( removal_ingredient );
        }
    }
    return ret;
}

bool compareAlergens( const std::pair<std::string, std::string> &a,
                      const std::pair<std::string, std::string> &b ) {
    return strcmp( a.first.c_str(), b.first.c_str() ) < 0;
}

int main() {
    std::ifstream input_file( "input" );
    auto input = getMeals( input_file );
    auto &alergens = input.first;
    auto &meals = input.second;
    auto safe = getSafeIngredients( alergens, meals );

    auto res = howMany( safe, meals );

    std::cout << "Safe ingredients appear \033[93;1m" << res << "\033[0m times"
              << std::endl;

    auto correct = oneDangerPerIngredient( alergens );
    std::sort( correct.begin(), correct.end(), compareAlergens );

    std::string canon{};
    for ( size_t i = 0; i < correct.size() - 1; i++ ) {
        canon += correct[i].second + ",";
    }
    canon += correct.back().second;
    std::cout << "Canonical list of alergens is \033[93;1m" << canon
              << "\033[0m" << std::endl;
}
