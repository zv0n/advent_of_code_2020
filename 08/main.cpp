#include <fstream>
#include <iostream>
#include <sstream>
#include <unordered_map>
#include <unordered_set>
#include <vector>

enum InstructionType {
    ACC,
    JMP,
    NOP,
};

class Instruction {
  public:
    Instruction() = default;
    Instruction( const std::string &type ) {
        setType( type );
    }
    Instruction( const std::string &type, int parameter ) {
        setType( type );
        setParameter( parameter );
    }

    void setType( const std::string &type ) {
        if ( type == "acc" )
            _type = ACC;
        else if ( type == "jmp" )
            _type = JMP;
        else
            _type = NOP;
    }

    void setParameter( int parameter ) {
        _parameter = parameter;
    }

    void executeInstruction( long long &glob_acc, size_t &program_counter ) {
        _called++;
        switch ( _type ) {
        case ACC:
            glob_acc += _parameter;
            program_counter++;
            break;
        case JMP:
            program_counter += _parameter;
            break;
        case NOP:
            program_counter++;
        default:
            break;
        }
    }

    bool hasBeenCalled() const {
        return _called > 0;
    }

    void resetCalled() {
        _called = 0;
    }

    bool isJmpOrNop() {
        return _type == JMP || _type == NOP;
    }

    void switchJmpNop() {
        switch ( _type ) {
        case JMP:
            _type = NOP;
            break;
        case NOP:
            _type = JMP;
        default:
            break;
        }
    }

  private:
    InstructionType _type;
    int _parameter;
    int _called = 0;
};

std::vector<Instruction> getInstructions( std::ifstream &file ) {
    std::vector<Instruction> ret{};
    std::string str;
    int parameter;
    while ( std::getline( file, str ) ) {
        std::stringstream ss( str );
        ss >> str;
        ss >> parameter;
        ret.emplace_back( str, parameter );
    }
    return ret;
}

int accBeforeSecondCall( std::vector<Instruction> &instructions ) {
    size_t program_counter = 0;
    long long acc = 0;
    while ( !instructions[program_counter].hasBeenCalled() ) {
        instructions[program_counter].executeInstruction( acc,
                                                          program_counter );
    }
    return acc;
}

size_t changeNextJmpOrNop( size_t last_change,
                           std::vector<Instruction> &instructions ) {
    size_t counter = last_change + 1;
    if ( instructions[counter].isJmpOrNop() && counter < instructions.size() ) {
        instructions[counter].switchJmpNop();
    }
    return counter;
}

int fixBootLoop( std::vector<Instruction> &instructions ) {
    size_t program_counter = 0;
    size_t last_change = -1;
    long long acc = 0;
    // brute force FTW
    while ( program_counter < instructions.size() ) {
        for ( auto &instruction : instructions ) {
            instruction.resetCalled();
        }
        last_change = changeNextJmpOrNop( last_change, instructions );
        program_counter = 0;
        acc = 0;
        while ( program_counter < instructions.size() &&
                !instructions[program_counter].hasBeenCalled() ) {
            instructions[program_counter].executeInstruction( acc,
                                                              program_counter );
        }
        // reset the instruction back to normal
        instructions[last_change].switchJmpNop();
    }
    return acc;
}

int main() {
    std::ifstream input_file( "input" );
    auto instructions = getInstructions( input_file );
    auto res = accBeforeSecondCall( instructions );
    std::cout << "ACC right before an instruction is called for the second "
                 "time is \033[93;1m"
              << res << "\033[0m" << std::endl;
    res = fixBootLoop( instructions );
    std::cout << "After fixing the boot loop, ACC has value of \033[93;1m"
              << res << "\033[0m" << std::endl;
}
