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

size_t findFirstInvalid( const std::vector<size_t> &nums, int preamble ) {
    for ( size_t pos = preamble; pos < nums.size(); pos++ ) {
        bool valid = false;
        for ( size_t i = pos - preamble; i < pos; i++ ) {
            for ( size_t j = i + 1; j < pos; j++ ) {
                if ( nums[i] + nums[j] == nums[pos] ) {
                    valid = true;
                    break;
                }
            }
            if ( valid )
                break;
        }
        if ( !valid )
            return nums[pos];
    }
    return -1;
}

std::pair<size_t, size_t> findMinMax( const std::vector<size_t> &nums,
                                      size_t start, size_t end ) {
    size_t min = -1;
    size_t max = 0;
    for ( size_t i = start; i <= end; i++ ) {
        if ( nums[i] < min )
            min = nums[i];
        if ( nums[i] > max )
            max = nums[i];
    }
    return { min, max };
}

std::pair<size_t, size_t> exploitWeakness( const std::vector<size_t> &nums,
                                           size_t invalid ) {
    size_t start = 0;
    size_t end = 0;
    for ( size_t i = 0; i < nums.size(); i++ ) {
        start = i;
        size_t sum = 0;
        for ( size_t j = i; j < nums.size(); j++ ) {
            sum += nums[j];
            if ( sum == invalid ) {
                end = j;
                break;
            }
            if ( sum > invalid )
                break;
        }
        if ( end != 0 )
            break;
    }
    return findMinMax( nums, start, end );
}

int main() {
    std::ifstream input_file( "input" );
    auto nums = getNums( input_file );
    auto invalid = findFirstInvalid( nums, 25 );
    std::cout << "First invalid number is \033[93;1m" << invalid << "\033[0m"
              << std::endl;
    auto exploit = exploitWeakness( nums, invalid );
    std::cout << "Encryption weakness is \033[93;1m"
              << exploit.first + exploit.second << "\033[0m (\033[91;1m"
              << exploit.first << ", " << exploit.second << "\033[0m)"
              << std::endl;
}
