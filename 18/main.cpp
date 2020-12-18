#include <algorithm>
#include <fstream>
#include <iostream>
#include <sstream>
#include <unordered_map>
#include <vector>

std::vector<std::string> getProblems( std::ifstream &file ) {
    std::vector<std::string> ret{};
    std::string str{};

    while ( std::getline( file, str ) ) {
        ret.push_back( "(" + str + ")" );
    }

    return ret;
}

enum MathAction {
    ADD,
    MULTIPLY,
};

std::pair<size_t, size_t> solveParenthesis( const std::string &problem,
                                            size_t pos, int part ) {
    std::pair<size_t, size_t> parenthesis_result = {};
    std::vector<size_t> nums;
    std::vector<MathAction> actions;
    while ( problem[pos] != ')' ) {
        switch ( problem[pos] ) {
        case '(':
            parenthesis_result = solveParenthesis( problem, pos + 1, part );
            nums.push_back( parenthesis_result.second );
            pos = parenthesis_result.first;
            break;
        case '+':
            actions.push_back( ADD );
            break;
        case '*':
            actions.push_back( MULTIPLY );
            break;
        default:
            if ( problem[pos] >= '0' && problem[pos] <= '9' ) {
                nums.push_back( problem[pos] - '0' );
            }
            break;
        }
        pos++;
    }
    size_t result = 0;
    if ( part == 1 ) {
        result = nums[0];
        for ( size_t i = 1; i < nums.size(); i++ ) {
            switch ( actions[i - 1] ) {
            case ADD:
                result += nums[i];
                break;
            case MULTIPLY:
                result *= nums[i];
                break;
            }
        }
    } else {
        for ( size_t i = 0; i < actions.size(); i++ ) {
            if ( actions[i] == ADD ) {
                nums[i + 1] = nums[i] + nums[i + 1];
                nums[i] = 1;
            }
        }
        result = 1;
        for ( size_t i = 0; i < nums.size(); i++ ) {
            result *= nums[i];
        }
    }
    return { pos, result };
}

std::vector<size_t> solveProblems( const std::vector<std::string> &problems,
                                   int part = 1 ) {
    std::vector<size_t> ret{};
    for ( auto &problem : problems ) {
        ret.push_back( solveParenthesis( problem, 1, part ).second );
    }
    return ret;
}

int main() {
    std::ifstream input_file( "input" );
    auto problems = getProblems( input_file );

    auto results = solveProblems( problems );
    size_t sum = 0;
    for ( auto &result : results ) {
        sum += result;
    }

    std::cout << "Sum of all results is \033[93;1m" << sum << "\033[0m"
              << std::endl;

    results = solveProblems( problems, 2 );
    sum = 0;
    for ( auto &result : results ) {
        sum += result;
    }

    std::cout << "Sum of all results in advanced math is \033[93;1m" << sum
              << "\033[0m" << std::endl;
}
