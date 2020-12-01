#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>

std::vector<int> getNums( std::ifstream &file ) {
    std::vector<int> ret{};
    int tmp = 0;
    std::string str;
    while ( std::getline( file, str ) ) {
        std::stringstream ss( str );
        ss >> tmp;
        ret.push_back( tmp );
    }
    return ret;
}

std::vector<int> findSum( const std::vector<int> &nums, int sum, int depth,
                          int start = 0 ) {
    if ( depth == 1 ) {
        // we're at the bottom, no more recursion
        for ( size_t i = start; i < nums.size(); i++ ) {
            if ( nums[i] == sum )
                return { nums[i] };
        }
    } else {
        for ( size_t i = start; i < nums.size(); i++ ) {
            int nextsum = sum - nums[i];
            if(nextsum <= 0)
                continue;
            auto res = findSum( nums, nextsum, depth - 1, i + 1 );
            if ( !res.empty() ) {
                res.push_back( nums[i] );
                return res;
            }
        }
    }
    return {};
}

int main() {
    std::ifstream input_file( "input" );
    auto nums = getNums( input_file );
    auto res = findSum( nums, 2020, 2 );
    std::cout << "The 2 numbers that add up to 2020 are \033[91;1m" << res[0]
              << ", " << res[1] << "\033[0m and their product is \033[93;1m"
              << res[0] * res[1] << "\033[0m" << std::endl;
    auto res3 = findSum( nums, 2020, 3 );
    std::cout << "The 3 numbers that add up to 2020 are \033[91;1m" << res3[0]
              << ", " << res3[1] << ", " << res3[2]
              << "\033[0m and their product is \033[93;1m"
              << res3[0] * res3[1] * res3[2] << "\033[0m" << std::endl;
}
