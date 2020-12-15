#include <iostream>
#include <unordered_map>
#include <vector>

int playMemoryGame( const std::vector<int> &input, int rounds ) {
    std::unordered_map<int, int> memory{};
    int round = 0;
    for ( int i = 0; i < input.size() - 1; i++ ) {
        round++;
        memory[input[i]] = round;
        rounds--;
    }
    rounds--;
    round++;
    int prev = input.back();
    for ( int i = 0; i < rounds; i++ ) {
        round++;
        int last = memory[prev];
        memory[prev] = round - 1;
        if ( last == 0 ) {
            prev = 0;
        } else {
            prev = round - 1 - last;
        }
    }
    return prev;
}

int main() {
    std::vector<int> input = { 9, 6, 0, 10, 18, 2, 1 };
    auto res = playMemoryGame( input, 2020 );
    std::cout << "The 2020th number is \033[93;1m" << res << "\033[0m"
              << std::endl;

    res = playMemoryGame( input, 30000000 );
    std::cout << "The 30000000th number is \033[93;1m" << res << "\033[0m"
              << std::endl;
}
