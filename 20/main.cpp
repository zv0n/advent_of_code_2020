#include <fstream>
#include <iostream>
#include <regex>
#include <sstream>
#include <unordered_set>
#include <vector>

enum Pos {
    TOP,
    TOP_R,
    BOTTOM,
    BOTTOM_R,
    LEFT,
    LEFT_R,
    RIGHT,
    RIGHT_R,
    EMPTY,
};

class Tile {
  public:
    Tile() {
        index = -1;
    }

    Tile( size_t num ) {
        index = num;
    }

    size_t getIndex() const {
        return index;
    }

    void addLine( const std::string &input ) {
        std::vector<bool> tmp{};
        for ( auto c : input ) {
            switch ( c ) {
            case '.':
                tmp.push_back( false );
                break;
            case '#':
                tmp.push_back( true );
            default:
                break;
            }
        }
        lines.push_back( tmp );
    }

    void checkTile( size_t index, const std::vector<Tile> &tiles ) {
        // check if `index` is current tile's neighbour
        auto top = getTop();
        auto res = checkLine( top, index, tiles );
        if ( res.first )
            top_neighbour = { index, res.second };

        auto bottom = getBottom();
        res = checkLine( bottom, index, tiles );
        if ( res.first )
            bottom_neighbour = { index, res.second };

        auto left = getLeft();
        res = checkLine( left, index, tiles );
        if ( res.first )
            left_neighbour = { index, res.second };

        auto right = getRight();
        res = checkLine( right, index, tiles );
        if ( res.first )
            right_neighbour = { index, res.second };
    }

    std::vector<bool> getTop() const {
        return lines[0];
    }
    std::vector<bool> getTopReversed() const {
        auto ret = getTop();
        std::reverse( ret.begin(), ret.end() );
        return ret;
    }

    std::vector<bool> getBottom() const {
        return lines.back();
    }
    std::vector<bool> getBottomReversed() const {
        auto ret = getBottom();
        std::reverse( ret.begin(), ret.end() );
        return ret;
    }

    std::vector<bool> getLeft() const {
        std::vector<bool> ret{};
        for ( auto &line : lines ) {
            ret.push_back( line[0] );
        }
        return ret;
    }
    std::vector<bool> getLeftReversed() const {
        auto ret = getLeft();
        std::reverse( ret.begin(), ret.end() );
        return ret;
    }

    std::vector<bool> getRight() const {
        std::vector<bool> ret{};
        for ( auto &line : lines ) {
            ret.push_back( line.back() );
        }
        return ret;
    }
    std::vector<bool> getRightReversed() const {
        auto ret = getRight();
        std::reverse( ret.begin(), ret.end() );
        return ret;
    }

    bool isCorner() const {
        // check if tile is a corner (needs to not have neighbour on 2
        // neighbouring edgeds)
        bool lr = left_neighbour.first == (size_t)-1 ||
                  right_neighbour.first == (size_t)-1;
        bool tb = top_neighbour.first == (size_t)-1 ||
                  bottom_neighbour.first == (size_t)-1;
        return lr && tb;
    }

    size_t getRightNeighbour() {
        return right_neighbour.first;
    }
    size_t getBottomNeighbour() {
        return bottom_neighbour.first;
    }

    Pos getRightPos() {
        return right_neighbour.second;
    }
    Pos getBottomPos() {
        return bottom_neighbour.second;
    }
    Pos getLeftPos() {
        return left_neighbour.second;
    }
    Pos getTopPos() {
        return top_neighbour.second;
    }

    void fixSelf( Pos left, Pos top ) {
        if ( top == EMPTY ) {
            switch ( left ) {
            case TOP:
                rotateLeft();
                flipHorizontal();
                break;
            case TOP_R:
                rotateLeft();
                break;
            case BOTTOM:
                rotateRight();
                break;
            case BOTTOM_R:
                rotateRight();
                flipHorizontal();
                break;
            case LEFT:
                break;
            case LEFT_R:
                flipHorizontal();
                break;
            case RIGHT:
                flipVertical();
                break;
            case RIGHT_R:
                flipVertical();
                flipHorizontal();
            default:
                break;
            }
        } else {
            switch ( top ) {
            case TOP:
                break;
            case TOP_R:
                flipVertical();
                break;
            case BOTTOM:
                flipHorizontal();
                break;
            case BOTTOM_R:
                flipHorizontal();
                flipVertical();
                break;
            case LEFT:
                rotateRight();
                flipVertical();
                break;
            case LEFT_R:
                rotateRight();
                break;
            case RIGHT:
                rotateLeft();
                break;
            case RIGHT_R:
                rotateLeft();
                flipVertical();
            default:
                break;
            }
        }
    }

    std::vector<std::string> getPicture() {
        std::vector<std::string> ret{};
        for ( size_t i = 1; i < lines.size() - 1; i++ ) {
            std::string tmp{};
            for ( size_t j = 1; j < lines[i].size() - 1; j++ ) {
                if ( lines[i][j] ) {
                    tmp += '#';
                } else {
                    tmp += '.';
                }
            }
            ret.push_back( tmp );
        }
        return ret;
    }

    void rotateLeft() {
        std::vector<std::vector<bool>> ret{};
        for ( size_t i = lines[0].size() - 1; i > 0; i-- ) {
            std::vector<bool> tmp{};
            for ( auto &line : lines ) {
                tmp.push_back( line[i] );
            }
            ret.push_back( tmp );
        }
        std::vector<bool> tmp{};
        for ( auto &line : lines ) {
            tmp.push_back( line[0] );
        }
        ret.push_back( tmp );
        lines = ret;
        auto tmp_n = left_neighbour;
        left_neighbour = top_neighbour;
        top_neighbour = right_neighbour;
        right_neighbour = bottom_neighbour;
        bottom_neighbour = tmp_n;

        right_neighbour.second = reverse( right_neighbour.second );
        left_neighbour.second = reverse( left_neighbour.second );
    }
    void rotateRight() {
        std::vector<std::vector<bool>> ret{};
        for ( size_t i = 0; i < lines[0].size(); i++ ) {
            std::vector<bool> tmp{};
            for ( size_t j = lines[0].size() - 1; j > 0; j-- ) {
                tmp.push_back( lines[j][i] );
            }
            tmp.push_back( lines[0][i] );
            ret.push_back( tmp );
        }
        lines = ret;
        auto tmp_n = left_neighbour;
        left_neighbour = bottom_neighbour;
        bottom_neighbour = right_neighbour;
        right_neighbour = top_neighbour;
        top_neighbour = tmp_n;

        top_neighbour.second = reverse( top_neighbour.second );
        bottom_neighbour.second = reverse( bottom_neighbour.second );
    }
    void flipVertical() {
        std::vector<std::vector<bool>> ret{};
        for ( size_t i = 0; i < lines.size(); i++ ) {
            std::vector<bool> tmp{};
            for ( size_t j = lines[i].size() - 1; j > 0; j-- ) {
                tmp.push_back( lines[i][j] );
            }
            tmp.push_back( lines[i][0] );
            ret.push_back( tmp );
        }
        lines = ret;
        auto tmp_n = left_neighbour;
        left_neighbour = right_neighbour;
        right_neighbour = tmp_n;

        top_neighbour.second = reverse( top_neighbour.second );
        bottom_neighbour.second = reverse( bottom_neighbour.second );
    }
    void flipHorizontal() {
        std::vector<std::vector<bool>> ret{};
        for ( size_t i = lines.size() - 1; i > 0; i-- ) {
            ret.push_back( lines[i] );
        }
        ret.push_back( lines[0] );
        lines = ret;
        auto tmp_n = top_neighbour;
        top_neighbour = bottom_neighbour;
        bottom_neighbour = tmp_n;

        right_neighbour.second = reverse( right_neighbour.second );
        left_neighbour.second = reverse( left_neighbour.second );
    }

  private:
    std::pair<bool, Pos> checkLine( const std::vector<bool> &input,
                                    size_t index,
                                    const std::vector<Tile> &tiles ) {
        // check if `input` matches any edges of tile with index `index`
        auto bottom = tiles[index].getBottom();
        auto bottom_r = tiles[index].getBottomReversed();
        auto top = tiles[index].getTop();
        auto top_r = tiles[index].getTopReversed();
        if ( input == bottom )
            return { true, BOTTOM };
        if ( input == bottom_r )
            return { true, BOTTOM_R };
        if ( input == top )
            return { true, TOP };
        if ( input == top_r )
            return { true, TOP_R };
        auto left = tiles[index].getLeft();
        auto left_r = tiles[index].getLeftReversed();
        auto right = tiles[index].getRight();
        auto right_r = tiles[index].getRightReversed();
        if ( input == left )
            return { true, LEFT };
        if ( input == left_r )
            return { true, LEFT_R };
        if ( input == right )
            return { true, RIGHT };
        if ( input == right_r )
            return { true, RIGHT_R };
        return { false, EMPTY };
    }

    Pos reverse( Pos input ) {
        switch ( input ) {
        case TOP:
            return TOP_R;
        case TOP_R:
            return TOP;
        case BOTTOM:
            return BOTTOM_R;
        case BOTTOM_R:
            return BOTTOM;
        case LEFT:
            return LEFT_R;
        case LEFT_R:
            return LEFT;
        case RIGHT:
            return RIGHT_R;
        case RIGHT_R:
            return RIGHT;
        case EMPTY:
            return EMPTY;
        }
    }

    size_t index;
    std::vector<std::vector<bool>> lines;
    std::pair<size_t, Pos> left_neighbour = { -1, EMPTY };
    std::pair<size_t, Pos> right_neighbour = { -1, EMPTY };
    std::pair<size_t, Pos> top_neighbour = { -1, EMPTY };
    std::pair<size_t, Pos> bottom_neighbour = { -1, EMPTY };
};

std::vector<Tile> getTiles( std::ifstream &file ) {
    std::vector<Tile> ret{};

    std::string str{};
    int index;
    Tile tmp{};
    bool next_is_index = true;
    while ( std::getline( file, str ) ) {
        if ( str == "" ) {
            ret.push_back( tmp );
            next_is_index = true;
            continue;
        }
        if ( next_is_index ) {
            next_is_index = false;
            std::stringstream ss( str );
            ss >> str;
            ss >> index;
            tmp = Tile( index );
        } else {
            tmp.addLine( str );
        }
    }
    if ( ret.back().getIndex() != tmp.getIndex() )
        ret.push_back( tmp );

    return ret;
}

void findNeighbours( std::vector<Tile> &tiles ) {
    for ( size_t i = 0; i < tiles.size(); i++ ) {
        for ( size_t j = 0; j < tiles.size(); j++ ) {
            if ( i != j ) {
                tiles[i].checkTile( j, tiles );
            }
        }
    }
}

std::vector<size_t> findCorners( const std::vector<Tile> &tiles ) {
    std::vector<size_t> ret{};
    for ( size_t i = 0; i < tiles.size(); i++ ) {
        if ( tiles[i].isCorner() ) {
            ret.push_back( i );
        }
    }
    return ret;
}

void correctTiles( std::vector<Tile> &tiles, size_t anchor_index ) {
    // rotate tiles so their edges are correctly oriented
    Tile *start = &tiles[anchor_index];
    Tile *prev_start = start;
    while ( true ) {
        Tile *cur = start;
        Tile *prev_cur = cur;
        while ( true ) {
            if ( prev_cur == cur ) {
                if ( prev_start == start ) {
                    cur->fixSelf( EMPTY, EMPTY );
                } else {
                    cur->fixSelf( EMPTY, prev_start->getBottomPos() );
                }
            } else {
                cur->fixSelf( prev_cur->getRightPos(), EMPTY );
            }
            auto next = cur->getRightNeighbour();
            if ( next == static_cast<size_t>( -1 ) ) {
                break;
            }
            prev_cur = cur;
            cur = &tiles[next];
        }
        auto next = start->getBottomNeighbour();
        if ( next == static_cast<size_t>( -1 ) ) {
            break;
        }
        prev_start = start;
        start = &tiles[next];
    }
}

std::vector<std::string> reconstructPicture( std::vector<Tile> &tiles,
                                             size_t topleft ) {
    std::vector<std::string> ret{};
    Tile *start = &tiles[topleft];
    while ( start != nullptr ) {
        Tile *cur = start;
        auto initial = cur->getPicture();
        do {
            cur = &tiles[cur->getRightNeighbour()];
            auto pic = cur->getPicture();
            for ( size_t i = 0; i < initial.size(); i++ ) {
                initial[i] += pic[i];
            }
        } while ( cur->getRightNeighbour() != static_cast<size_t>( -1 ) );
        for ( auto &line : initial ) {
            ret.push_back( std::move( line ) );
        }
        auto index = start->getBottomNeighbour();
        if ( index == static_cast<size_t>( -1 ) )
            start = nullptr;
        else
            start = &tiles[index];
    }
    return ret;
}

void makeTopLeftV1( size_t index, std::vector<Tile> &tiles ) {
    Tile &worker = tiles[index];
    while ( worker.getTopPos() != EMPTY || worker.getLeftPos() != EMPTY ) {
        worker.rotateLeft();
    }
}

void makeTopLeftV2( size_t index, std::vector<Tile> &tiles ) {
    Tile &worker = tiles[index];
    worker.flipHorizontal();
    while ( worker.getTopPos() != EMPTY || worker.getLeftPos() != EMPTY ) {
        worker.rotateLeft();
    }
}

size_t findMin( std::vector<size_t> &input ) {
    size_t min = 0;
    for ( size_t i = 1; i < input.size(); i++ ) {
        if ( input[i] < input[min] )
            min = i;
    }
    return min;
}

bool checkMonster( size_t line, size_t head_pos,
                   const std::vector<std::string> &picture ) {
    if ( head_pos < 18 )
        return false;
    if ( picture[line][head_pos] != '#' )
        return false;
    line++;
    if ( picture[line][head_pos - 18] != '#' )
        return false;
    if ( picture[line][head_pos - 13] != '#' )
        return false;
    if ( picture[line][head_pos - 12] != '#' )
        return false;
    if ( picture[line][head_pos - 7] != '#' )
        return false;
    if ( picture[line][head_pos - 6] != '#' )
        return false;
    if ( picture[line][head_pos - 1] != '#' )
        return false;
    if ( picture[line][head_pos] != '#' )
        return false;
    if ( picture[line][head_pos + 1] != '#' )
        return false;
    line++;
    if ( picture[line][head_pos - 17] != '#' )
        return false;
    if ( picture[line][head_pos - 14] != '#' )
        return false;
    if ( picture[line][head_pos - 11] != '#' )
        return false;
    if ( picture[line][head_pos - 8] != '#' )
        return false;
    if ( picture[line][head_pos - 5] != '#' )
        return false;
    if ( picture[line][head_pos - 2] != '#' )
        return false;
    return true;
}

void deleteMonster( size_t line, size_t head_pos,
                    std::vector<std::string> &picture ) {
    picture[line][head_pos] = 'O';
    line++;
    picture[line][head_pos - 18] = 'O';
    picture[line][head_pos - 13] = 'O';
    picture[line][head_pos - 12] = 'O';
    picture[line][head_pos - 7] = 'O';
    picture[line][head_pos - 6] = 'O';
    picture[line][head_pos - 1] = 'O';
    picture[line][head_pos] = 'O';
    picture[line][head_pos + 1] = 'O';
    line++;
    picture[line][head_pos - 17] = 'O';
    picture[line][head_pos - 14] = 'O';
    picture[line][head_pos - 11] = 'O';
    picture[line][head_pos - 8] = 'O';
    picture[line][head_pos - 5] = 'O';
    picture[line][head_pos - 2] = 'O';
}

std::string prettify( const std::string &input ) {
    std::string ret = "";
    size_t pos = 0;
    size_t prev_pos = 0;
    while ( ( pos = input.find( 'O', pos ) ) != std::string::npos ) {
        ret += input.substr( prev_pos, pos - prev_pos );
        pos++;
        ret += "\033[93;1mO\033[0m";
        prev_pos = pos;
    }
    ret += input.substr( prev_pos );
    return ret;
}

int main( int argc, char **argv ) {
    std::ifstream input_file( "input" );
    auto tiles = getTiles( input_file );

    findNeighbours( tiles );
    auto corners = findCorners( tiles );
    std::string corners_str{};
    size_t res = 1;
    for ( size_t i = 0; i < corners.size() - 1; i++ ) {
        corners_str += std::to_string( tiles[corners[i]].getIndex() ) + ", ";
        res *= tiles[corners[i]].getIndex();
    }
    corners_str += std::to_string( tiles[corners.back()].getIndex() );
    res *= tiles[corners.back()].getIndex();

    std::cout << "The multiplication is: \033[93;1m" << res
              << "\033[0m (\033[91;1m" << corners_str << "\033[0m)"
              << std::endl;

    std::vector<std::vector<std::string>> pictures{};
    for ( auto corner : corners ) {
        std::vector<Tile> workTiles = tiles;
        makeTopLeftV1( corner, workTiles );
        correctTiles( workTiles, corner );
        pictures.push_back( reconstructPicture( workTiles, corner ) );

        workTiles = tiles;
        makeTopLeftV2( corner, workTiles );
        correctTiles( workTiles, corner );
        pictures.push_back( reconstructPicture( workTiles, corner ) );
    }

    std::vector<size_t> picture_hashes;
    for ( auto &picture : pictures ) {
        std::smatch sm;
        for ( size_t i = 0; i < picture.size() - 1; i++ ) {
            for ( size_t j = 18; j < picture[i].length(); j++ ) {
                if ( checkMonster( i, j, picture ) )
                    deleteMonster( i, j, picture );
            }
        }
        size_t hashes = 0;
        for ( auto &line : picture ) {
            for ( auto &c : line ) {
                if ( c == '#' )
                    hashes++;
            }
        }
        picture_hashes.push_back( hashes );
    }
    auto result_index = findMin( picture_hashes );
    std::cout << "There are \033[93;1m" << picture_hashes[result_index]
              << "\033[0m hashes" << std::endl;

    if ( argc == 2 && !strcmp( argv[1], "--monsters" ) ) {
        std::cout << "MONSTER LAKE: " << std::endl;
        for ( auto &line : pictures[result_index] ) {
            std::cout << prettify( line ) << std::endl;
        }
    }
}
