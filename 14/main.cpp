#include <algorithm>
#include <fstream>
#include <iostream>
#include <sstream>
#include <unordered_map>
#include <vector>

#define pow2_36 0x0000000800000000
#define max36 0x0000000FFFFFFFFF

enum InstructionType {
    BITMASK,
    MEMORY,
};

struct Instruction {
    InstructionType type;
    size_t address;
    size_t value;
    std::string mask;
};

std::vector<Instruction> getInstructions( std::ifstream &file ) {
    std::vector<Instruction> ret{};
    std::string str;
    std::string instruction;

    while ( std::getline( file, str ) ) {
        Instruction inst = {
            .type = MEMORY,
            .address = 0,
            .value = 0,
            .mask = "",
        };
        instruction = str.substr( 0, 4 );
        if ( instruction == "mask" ) {
            inst.type = BITMASK;
            inst.mask = str.substr( 7 );
        } else {
            char c;
            std::stringstream ss( str.substr( 4 ) );
            ss >> inst.address;
            ss >> c;
            ss >> c;
            ss >> inst.value;
        }
        ret.push_back( inst );
    }
    return ret;
}

class DockingComputer {
  public:
    DockingComputer() {}

    void addToMem( size_t address, size_t value ) {
        value &= _and_mask;
        value |= _or_mask;
        _memory[address] = value;
    }

    void addToMemV2( size_t address, size_t value ) {
        address |= _or_mask;
        addToMemV2Internal( address, value, 0 );
    }

    void updateMask( const std::string &mask ) {
        _mask = mask;
        _and_mask = max36;
        _or_mask = 0;
        size_t cur = pow2_36;
        for ( auto c : mask ) {
            switch ( c ) {
            case '0':
                _and_mask &= ~cur;
                break;
            case '1':
                _or_mask |= cur;
            default:
                break;
            }
            cur = cur >> 1;
        }
    }

    size_t sumOfMemory() {
        size_t ret = 0;
        for ( const auto &entry : _memory ) {
            ret += entry.second;
        }
        return ret;
    }

    void reset() {
        _memory.clear();
        _and_mask = max36;
        _or_mask = 0;
        _mask = "";
    }

  private:
    void addToMemV2Internal( size_t address, size_t value, size_t mask_pos ) {
        if ( mask_pos >= _mask.size() )
            return;
        while ( _mask[mask_pos] != 'X' ) {
            mask_pos++;
            if ( mask_pos >= _mask.size() )
                return;
        }

        size_t cur = ( pow2_36 >> mask_pos );
        address |= cur;
        _memory[address] = value;
        addToMemV2Internal( address, value, mask_pos + 1 );
        address &= ~cur;
        _memory[address] = value;
        addToMemV2Internal( address, value, mask_pos + 1 );
    }

    std::unordered_map<size_t, size_t> _memory;
    size_t _and_mask = max36;
    size_t _or_mask = 0;
    std::string _mask = "";
};

int main() {
    std::ifstream input_file( "input" );
    auto instructions = getInstructions( input_file );

    DockingComputer docker{};
    for ( const auto &instruction : instructions ) {
        switch ( instruction.type ) {
        case MEMORY:
            docker.addToMem( instruction.address, instruction.value );
            break;
        case BITMASK:
            docker.updateMask( instruction.mask );
            break;
        }
    }
    std::cout << "Sum of all values in the computer is \033[93;1m"
              << docker.sumOfMemory() << "\033[0m" << std::endl;

    docker.reset();
    for ( const auto &instruction : instructions ) {
        switch ( instruction.type ) {
        case MEMORY:
            docker.addToMemV2( instruction.address, instruction.value );
            break;
        case BITMASK:
            docker.updateMask( instruction.mask );
            break;
        }
    }
    std::cout << "Sum of all values in the computer (V2) is \033[93;1m"
              << docker.sumOfMemory() << "\033[0m" << std::endl;
}
