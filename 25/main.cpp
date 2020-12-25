#include <fstream>
#include <iostream>

std::pair<size_t, size_t> getPublicKeys( std::ifstream &file ) {
    size_t a;
    size_t b;

    std::string str{};
    std::getline( file, str );
    a = std::stoull(str);
    std::getline( file, str );
    b = std::stoull(str);

    return {a, b};
}

size_t loop(size_t subject, size_t value, size_t modulo, size_t loops) {
    for(size_t i = 0; i < loops; i++) {
        value *= subject;
        value %= modulo;
    }
    return value;
}

std::pair<size_t, bool> getLoopNumber(size_t subject, size_t modulo, size_t public_a, size_t public_b) {
    size_t loop_num = 0;
    size_t value = 1;
    while(value != public_a && value != public_b) {
        loop_num++;
        value = loop(subject, value, modulo, 1);
    }
    if(value == public_a)
        return {loop_num, false};
    return {loop_num, true};
}

int main() {
    std::ifstream input_file( "input" );
    auto public_keys = getPublicKeys( input_file );
    size_t subject_value = 7;
    size_t modulo = 20201227;

    auto loop_number = getLoopNumber(subject_value, modulo, public_keys.first, public_keys.second);
    size_t encryption = 0;
    if(!loop_number.second) {
        encryption = loop(public_keys.second, 1, modulo, loop_number.first);
    } else {
        encryption = loop(public_keys.first, 1, modulo, loop_number.first);
    }

    std::cout << "The encryption key is \033[93;1m" << encryption
              << "\033[0m" << std::endl;
}
