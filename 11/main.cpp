#include <algorithm>
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>

enum Seat {
    OCCUPIED,
    EMPTY,
    FLOOR,
};

std::vector<std::vector<Seat>> getLayout( std::ifstream &file ) {
    std::vector<std::vector<Seat>> ret{};
    std::string str;
    while ( std::getline( file, str ) ) {
        // we'll pad left/right with floor for simpler computations
        std::vector<Seat> tmp = { FLOOR };
        for ( auto &c : str ) {
            switch ( c ) {
            case '.':
                tmp.push_back( FLOOR );
                break;
            case 'L':
                tmp.push_back( EMPTY );
                break;
            case '#':
                tmp.push_back( OCCUPIED );
            default:
                break;
            }
        }
        // again, padding left/right column
        tmp.push_back( FLOOR );
        ret.push_back( std::move( tmp ) );
    }
    // we'll pad top and bottom with floor for simpler computations
    std::vector<Seat> tmp( ret[0].size(), FLOOR );
    ret.push_back( tmp );
    ret.insert( ret.begin(), tmp );
    return ret;
}

bool shouldChangePart1( size_t row, size_t col,
                        const std::vector<std::vector<Seat>> &layout ) {
    if ( layout[row][col] == FLOOR )
        return false;
    int occupied = 0;
    for ( size_t i = row - 1; i <= row + 1; i++ ) {
        for ( size_t j = col - 1; j <= col + 1; j++ ) {
            if ( ( i != row || j != col ) && layout[i][j] == OCCUPIED )
                occupied++;
        }
    }
    switch ( layout[row][col] ) {
    case OCCUPIED:
        return occupied >= 4;
    case EMPTY:
        return occupied == 0;
    default:
        break;
    }
    return false;
}

bool foundOccupied( size_t row_tmp, size_t col_tmp, int row_add, int col_add,
                    const std::vector<std::vector<Seat>> &layout ) {
    row_tmp += row_add;
    col_tmp += col_add;
    size_t row_limit = 0;
    size_t col_limit = 0;
    if ( row_add > 0 )
        row_limit = layout.size() - 1;
    if ( col_add > 0 )
        col_limit = layout[0].size() - 1;
    while ( row_tmp != row_limit && col_tmp != col_limit &&
            layout[row_tmp][col_tmp] == FLOOR ) {
        row_tmp += row_add;
        col_tmp += col_add;
    }
    return layout[row_tmp][col_tmp] == OCCUPIED;
}

bool shouldChangePart2( size_t row, size_t col,
                        const std::vector<std::vector<Seat>> &layout ) {
    if ( layout[row][col] == FLOOR )
        return false;
    int occupied = 0;
    // up
    if ( foundOccupied( row, col, -1, 0, layout ) )
        occupied++;
    // down
    if ( foundOccupied( row, col, 1, 0, layout ) )
        occupied++;
    // left
    if ( foundOccupied( row, col, 0, -1, layout ) )
        occupied++;
    // right
    if ( foundOccupied( row, col, 0, 1, layout ) )
        occupied++;
    // northwest
    if ( foundOccupied( row, col, -1, -1, layout ) )
        occupied++;
    // northeast
    if ( foundOccupied( row, col, -1, 1, layout ) )
        occupied++;
    // southwest
    if ( foundOccupied( row, col, 1, -1, layout ) )
        occupied++;
    // southeast
    if ( foundOccupied( row, col, 1, 1, layout ) )
        occupied++;

    switch ( layout[row][col] ) {
    case OCCUPIED:
        return occupied >= 5;
    case EMPTY:
        return occupied == 0;
    default:
        break;
    }
    return false;
}

size_t findEquilibrium(
    std::vector<std::vector<Seat>> input,
    bool ( *shouldChange )( size_t, size_t,
                            const std::vector<std::vector<Seat>> & ) ) {
    std::vector<std::vector<Seat>> prev{};
    int changes = 0;
    do {
        prev = input;
        changes = 0;
        for ( size_t i = 1; i < input.size() - 1; i++ ) {
            for ( size_t j = 1; j < input[i].size() - 1; j++ ) {
                if ( shouldChange( i, j, prev ) ) {
                    changes++;
                    switch ( input[i][j] ) {
                    case OCCUPIED:
                        input[i][j] = EMPTY;
                        break;
                    case EMPTY:
                        input[i][j] = OCCUPIED;
                    default:
                        break;
                    }
                }
            }
        }
    } while ( changes != 0 );
    size_t occupied = 0;
    for ( size_t i = 1; i < input.size() - 1; i++ ) {
        for ( size_t j = 1; j < input[i].size() - 1; j++ ) {
            if ( input[i][j] == OCCUPIED )
                occupied++;
        }
    }
    return occupied;
}

int main() {
    std::ifstream input_file( "input" );
    auto layout = getLayout( input_file );
    auto res = findEquilibrium( layout, shouldChangePart1 );
    std::cout << "After finding an equilibrium \033[93;1m" << res
              << "\033[0m seats are occupied" << std::endl;
    auto res2 = findEquilibrium( layout, shouldChangePart2 );
    std::cout << "After finding an equilibrium (with real people this time) "
                 "\033[93;1m"
              << res2 << "\033[0m seats are occupied" << std::endl;
}
