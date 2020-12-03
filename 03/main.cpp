#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>

std::vector<std::vector<bool>> getTrees( std::ifstream &file ) {
    std::vector<std::vector<bool>> ret{};
    std::string str;
    while ( std::getline( file, str ) ) {
        std::vector<bool> treeLine{};
        for(auto &c : str) {
            switch(c) {
                case '.':
                    treeLine.push_back(false);
                    break;
                case '#':
                    treeLine.push_back(true);
                default:
                    break;
            }
        }
        ret.push_back(treeLine);
    }
    return ret;
}

size_t findTrees(const std::vector<std::vector<bool>> &trees, int x, int y) {
    std::pair<size_t, size_t> pos = {0,0};
    size_t modulus = trees[0].size();
    size_t encounters = 0;
    while(pos.second < trees.size()) {
        if(trees[pos.second][pos.first])
            encounters++;
        pos.first = (pos.first + x) % modulus;
        pos.second += y;
    }
    return encounters;
}

int main() {
    std::ifstream input_file( "input" );
    auto trees = getTrees( input_file );
    auto res31 = findTrees(trees, 3, 1);
    std::cout << "You've encountered \033[93;1m" << res31 << "\033[0m trees wit slope (3,1)"
              << std::endl;

    auto res11 = findTrees(trees, 1, 1);
    auto res51 = findTrees(trees, 5, 1);
    auto res71 = findTrees(trees, 7, 1);
    auto res12 = findTrees(trees, 1, 2);
    std::cout << "You've encountered this amount of trees for given slopes: \033[91;1m"
              << res11 << " (1,1), " << res31 << " (3,1), " << res51 << " (5,1), "
              << res71 << " (7,1), " << res12 << " (2,1)\033[0m. When multiplied, they produce "
              << "\033[93;1m" << res11 * res31 * res51 * res71 * res12 << "\033[0m."
              << std::endl;
}
