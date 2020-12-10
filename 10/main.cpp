#include <algorithm>
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>

std::vector<size_t> getNums( std::ifstream &file ) {
    std::vector<size_t> ret{};
    std::string str;
    size_t num;
    while ( std::getline( file, str ) ) {
        std::stringstream ss( str );
        ss >> num;
        ret.push_back( num );
    }
    return ret;
}

std::tuple<int, int, int>
getJoltDifferences( const std::vector<size_t> &nums ) {
    std::vector<int> buf = { 0, 0, 0 };
    size_t prev = 0;
    for ( size_t i = 0; i < nums.size(); i++ ) {
        auto diff = nums[i] - prev;
        buf[diff - 1]++;
        prev = nums[i];
    }
    return { buf[0], buf[1], buf[2] };
}

size_t possibilities( const std::vector<size_t> &nums ) {
    std::vector<unsigned long long> cache{};
    cache.resize( nums.size() );
    cache.back() = 1;
    for ( size_t i = nums.size() - 2; i != (size_t)-1; i-- ) {
        for ( size_t j = i + 1; j < nums.size(); j++ ) {
            if ( nums[j] - nums[i] < 4 ) {
                cache[i] += cache[j];
            }
        }
    }
    unsigned long long ret = 0;
    for ( size_t i = 0; i < nums.size(); i++ ) {
        if ( nums[i] > 3 )
            break;
        ret += cache[i];
    }
    return ret;
}

int main() {
    std::ifstream input_file( "input" );
    auto nums = getNums( input_file );
    std::sort( nums.begin(), nums.end() );
    nums.push_back( nums[nums.size() - 1] + 3 );
    auto differences = getJoltDifferences( nums );
    auto res = std::get<0>( differences ) * std::get<2>( differences );
    std::cout << "Multiplied difference: \033[93;1m" << res
              << "\033[0m (\033[91;1m" << std::get<0>( differences ) << ", "
              << std::get<1>( differences ) << ", "
              << std::get<2>( differences ) << "\033[0m)" << std::endl;
    auto res2 = possibilities( nums );
    std::cout << "There are: \033[93;1m" << res2
              << "\033[0m possible ways of arranging the adapters" << std::endl;
}
