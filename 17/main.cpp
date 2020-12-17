#include <algorithm>
#include <fstream>
#include <iostream>
#include <sstream>
#include <unordered_map>
#include <vector>

class PocketDimension {
  public:
    PocketDimension() = delete;
    PocketDimension( const std::vector<std::vector<bool>> &initial_state ) {
        _initial_state = initial_state;
        reset();
    }

    void reset() {
        min_w = 0;
        max_w = 0;
        min_z = 0;
        max_z = 0;
        min_x = 0;
        max_x = _initial_state[0].size() - 1;
        min_y = 0;
        max_y = _initial_state.size() - 1;
        cubes.clear();
        for ( size_t i = 0; i < _initial_state.size(); i++ ) {
            for ( size_t j = 0; j < _initial_state[i].size(); j++ ) {
                cubes[0][0][i][j] = _initial_state[i][j];
            }
        }
    }

    void step3D() {
        auto cubes_next = cubes;
        for ( int z = min_z - 1; z <= max_z + 1; z++ ) {
            for ( int y = min_y - 1; y <= max_y + 1; y++ ) {
                for ( int x = min_x - 1; x <= max_x + 1; x++ ) {
                    auto neigbours = numberOfNeighbours( 0, z, y, x );
                    if ( cubes[0][z][y][x] &&
                         ( neigbours < 2 || neigbours > 3 ) ) {
                        cubes_next[0][z][y][x] = false;
                    } else if ( !cubes[0][z][y][x] && neigbours == 3 ) {
                        cubes_next[0][z][y][x] = true;
                        checkMinMax( 0, z, y, x );
                    }
                }
            }
        }
        cubes = cubes_next;
    }

    void step3D( size_t count ) {
        for ( size_t i = 0; i < count; i++ ) {
            step3D();
        }
    }

    void step4D() {
        auto cubes_next = cubes;
        for ( int w = min_w - 1; w <= max_w + 1; w++ ) {
            for ( int z = min_z - 1; z <= max_z + 1; z++ ) {
                for ( int y = min_y - 1; y <= max_y + 1; y++ ) {
                    for ( int x = min_x - 1; x <= max_x + 1; x++ ) {
                        auto neigbours = numberOfNeighbours( w, z, y, x );
                        if ( cubes[w][z][y][x] &&
                             ( neigbours < 2 || neigbours > 3 ) ) {
                            cubes_next[w][z][y][x] = false;
                        } else if ( !cubes[w][z][y][x] && neigbours == 3 ) {
                            cubes_next[w][z][y][x] = true;
                            checkMinMax( w, z, y, x );
                        }
                    }
                }
            }
        }
        cubes = cubes_next;
    }

    void step4D( size_t count ) {
        for ( size_t i = 0; i < count; i++ ) {
            step4D();
        }
    }

    size_t activeCubes() {
        size_t ret = 0;
        for ( int w = min_w; w <= max_w; w++ ) {
            for ( int z = min_z; z <= max_z; z++ ) {
                for ( int y = min_y; y <= max_y; y++ ) {
                    for ( int x = min_x; x <= max_x; x++ ) {
                        if ( cubes[w][z][y][x] )
                            ret++;
                    }
                }
            }
        }
        return ret;
    }

  private:
    int numberOfNeighbours( int w, int z, int y, int x ) {
        int ret = 0;
        for ( int w_n = w - 1; w_n <= w + 1; w_n++ ) {
            for ( int z_n = z - 1; z_n <= z + 1; z_n++ ) {
                for ( int y_n = y - 1; y_n <= y + 1; y_n++ ) {
                    for ( int x_n = x - 1; x_n <= x + 1; x_n++ ) {
                        if ( w_n == w && z_n == z && y_n == y && x_n == x )
                            continue;
                        if ( cubes[w_n][z_n][y_n][x_n] )
                            ret++;
                    }
                }
            }
        }
        return ret;
    }

    void checkMinMax( int w, int z, int y, int x ) {
        if ( w < min_w )
            min_w = w;
        else if ( w > max_w )
            max_w = w;
        if ( z < min_z )
            min_z = z;
        else if ( z > max_z )
            max_z = z;
        if ( y < min_y )
            min_y = y;
        else if ( y > max_y )
            max_y = y;
        if ( x < min_x )
            min_x = x;
        else if ( x > max_x )
            max_x = x;
    }

    int min_x, max_x;
    int min_y, max_y;
    int min_z, max_z;
    int min_w, max_w;
    std::unordered_map<
        int, std::unordered_map<
                 int, std::unordered_map<int, std::unordered_map<int, bool>>>>
        cubes;
    std::vector<std::vector<bool>> _initial_state;
};

std::vector<std::vector<bool>> getInitialState( std::ifstream &file ) {
    std::vector<std::vector<bool>> ret{};
    std::string str{};

    while ( std::getline( file, str ) ) {
        std::vector<bool> tmp{};
        for ( auto c : str ) {
            switch ( c ) {
            case '#':
                tmp.push_back( true );
                break;
            case '.':
                tmp.push_back( false );
            default:
                break;
            }
        }
        ret.push_back( tmp );
    }

    return ret;
}

int main() {
    std::ifstream input_file( "input" );
    auto initial = getInitialState( input_file );

    PocketDimension pocket( initial );
    pocket.step3D( 6 );

    std::cout << "After \033[91;1m6\033[0m iterations, there are \033[93;1m"
              << pocket.activeCubes() << "\033[0m active cubes" << std::endl;
    pocket.reset();
    pocket.step4D( 6 );
    std::cout << "After \033[91;1m6\033[0m iterations (in \033[91;1m4D\033[0m "
                 "this time), there are \033[93;1m"
              << pocket.activeCubes() << "\033[0m active cubes" << std::endl;
}
