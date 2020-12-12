#include <algorithm>
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>

enum InstructionType {
    NORTH,
    SOUTH,
    WEST,
    EAST,
    LEFT,
    RIGHT,
    FORWARD,
};

class Instruction {
  public:
    Instruction() = delete;
    Instruction( char instruction, long long value ) {
        switch ( instruction ) {
        case 'N':
            _type = NORTH;
            break;
        case 'S':
            _type = SOUTH;
            break;
        case 'W':
            _type = WEST;
            break;
        case 'E':
            _type = EAST;
            break;
        case 'L':
            _type = LEFT;
            break;
        case 'R':
            _type = RIGHT;
            break;
        case 'F':
            _type = FORWARD;
        default:
            break;
        }
        _value = value;
    }

    InstructionType getType() const {
        return _type;
    }

    long long getValue() const {
        return _value;
    }

  private:
    InstructionType _type;
    long long _value;
};

InstructionType getNewDirection( InstructionType original,
                                 InstructionType movement, long long angle ) {
    InstructionType directions[] = { NORTH, EAST, SOUTH, WEST };
    int position = 0;
    for ( int i = 0; i < 4; i++ ) {
        if ( original == directions[i] ) {
            position = i;
            break;
        }
    }
    angle = angle / 90;
    switch ( movement ) {
    case LEFT:
        position -= angle;
        break;
    case RIGHT:
        position += angle;
    default:
        break;
    }
    while ( position < 0 )
        position += 4;
    while ( position > 3 )
        position -= 4;
    return directions[position];
}

void directionMovement( InstructionType direction, long long value,
                        long long &pos_x, long long &pos_y ) {
    switch ( direction ) {
    case NORTH:
        pos_y -= value;
        break;
    case SOUTH:
        pos_y += value;
        break;
    case WEST:
        pos_x -= value;
        break;
    case EAST:
        pos_x += value;
    default:
        break;
    }
}

class Waypoint {
  public:
    Waypoint() {
        _pos_x = 10;
        _pos_y = -1;
    }

    void executeInstruction( const Instruction &instruction ) {
        switch ( instruction.getType() ) {
        case LEFT:
        case RIGHT:
            rotate( instruction.getType(), instruction.getValue() / 90 );
            break;
        default:
            directionMovement( instruction.getType(), instruction.getValue(),
                               _pos_x, _pos_y );
            break;
        }
    }

    const long long &getPosX() const {
        return _pos_x;
    }

    const long long &getPosY() const {
        return _pos_y;
    }

    void reset() {
        _pos_x = 10;
        _pos_y = -1;
    }

  private:
    void rotate( InstructionType direction, long long amount ) {
        long long i = 0;
        long long tmp = 0;
        switch ( direction ) {
        case LEFT:
            for ( ; i < amount; i++ ) {
                tmp = _pos_y;
                _pos_y = ( -1 ) * _pos_x;
                _pos_x = tmp;
            }
            break;
        case RIGHT:
            for ( ; i < amount; i++ ) {
                tmp = _pos_y;
                _pos_y = _pos_x;
                _pos_x = ( -1 ) * tmp;
            }
        default:
            break;
        }
    }
    long long _pos_x;
    long long _pos_y;
};

class Ship {
  public:
    Ship() {
        _direction = EAST;
        _pos_x = 0;
        _pos_y = 0;
    }

    void executeInstruction( const Instruction &instruction ) {
        switch ( instruction.getType() ) {
        case LEFT:
        case RIGHT:
            _direction = getNewDirection( _direction, instruction.getType(),
                                          instruction.getValue() );
            break;
        case FORWARD:
            directionMovement( _direction, instruction.getValue(), _pos_x,
                               _pos_y );
            break;
        default:
            directionMovement( instruction.getType(), instruction.getValue(),
                               _pos_x, _pos_y );
            break;
        }
    }

    void executeInstructionPart2( const Instruction &instruction ) {
        switch ( instruction.getType() ) {
        case FORWARD:
            _pos_x += instruction.getValue() * _waypoint.getPosX();
            _pos_y += instruction.getValue() * _waypoint.getPosY();
            break;
        default:
            _waypoint.executeInstruction( instruction );
            break;
        }
    }

    long long getManhattanDistance() {
        long long ret = 0;
        if ( _pos_x < 0 )
            ret -= _pos_x;
        else
            ret += _pos_x;
        if ( _pos_y < 0 )
            ret -= _pos_y;
        else
            ret += _pos_y;
        return ret;
    }

    void reset() {
        _direction = EAST;
        _pos_x = 0;
        _pos_y = 0;
        _waypoint.reset();
    }

  private:
    InstructionType _direction;
    long long _pos_x;
    long long _pos_y;
    Waypoint _waypoint{};
};

std::vector<Instruction> getInstructions( std::ifstream &file ) {
    std::vector<Instruction> ret{};
    std::string str;
    char instruction;
    long long value;
    while ( std::getline( file, str ) ) {
        std::stringstream ss( str );
        ss >> instruction;
        ss >> value;
        ret.emplace_back( instruction, value );
    }
    return ret;
}

int main() {
    std::ifstream input_file( "input" );

    auto instructions = getInstructions( input_file );
    auto ship = Ship();

    for ( const auto &instruction : instructions ) {
        ship.executeInstruction( instruction );
    }
    std::cout << "The ship's Manhattan distance is \033[93;1m"
              << ship.getManhattanDistance() << "\033[0m" << std::endl;

    ship.reset();
    for ( const auto &instruction : instructions ) {
        ship.executeInstructionPart2( instruction );
    }
    std::cout << "After reading the manual, the ship's Manhattan distance is "
                 "\033[93;1m"
              << ship.getManhattanDistance() << "\033[0m" << std::endl;
}
