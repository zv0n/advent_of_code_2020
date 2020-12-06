#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>

class Group {
  public:
    Group() {
        answers.resize( 26, 0 );
    }

    void addAnswers( const std::string &input ) {
        for ( auto &c : input ) {
            answers[c - 97]++;
        }
        members++;
    }

    int answersCount() const {
        return countAnswers( 1 );
    }

    int universalAnswerCount() const {
        return countAnswers( members );
    }

  private:
    int countAnswers( int min ) const {
        int ret = 0;
        for ( const auto &answer : answers ) {
            if ( answer >= min )
                ret++;
        }
        return ret;
    }

    std::vector<int> answers;
    int members = 0;
};

std::vector<Group> getGroups( std::ifstream &file ) {
    std::vector<Group> ret{};
    std::string str;
    ret.emplace_back();
    while ( std::getline( file, str ) ) {
        if ( str.empty() )
            ret.emplace_back();
        else
            ret.back().addAnswers( str );
    }
    return ret;
}

int groupAnswersSum( const std::vector<Group> &groups, int part ) {
    int sum = 0;
    for ( auto &group : groups ) {
        if ( part == 1 )
            sum += group.answersCount();
        else
            sum += group.universalAnswerCount();
    }
    return sum;
}

int groupUniqueAnswersSum( const std::vector<Group> &groups ) {
    return groupAnswersSum( groups, 1 );
}

int groupUniversalAnswersSum( const std::vector<Group> &groups ) {
    return groupAnswersSum( groups, 2 );
}

int main() {
    std::ifstream input_file( "input" );
    auto groups = getGroups( input_file );
    auto res = groupUniqueAnswersSum( groups );
    std::cout << "Sum of unique answers from each group is \033[93;1m" << res
              << "\033[0m" << std::endl;
    res = groupUniversalAnswersSum( groups );
    std::cout << "Sum of universal answers from each group is \033[93;1m" << res
              << "\033[0m" << std::endl;
}
