#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>

class Passport {
  public:
    void setBirthYear( int year ) {
        birth_year = year;
    }
    void setIssueYear( int year ) {
        issue_year = year;
    }
    void setExpirationYear( int year ) {
        expiration_year = year;
    }
    void setHeight( const std::string &input ) {
        height = input;
    }
    void setHairColor( const std::string &input ) {
        hair_color = input;
    }
    void setEyeColor( const std::string &input ) {
        eye_color = input;
    }
    void setPassportID( const std::string &id ) {
        passport_id = id;
    }
    void setCountryID( const std::string &id ) {
        country_id = id;
    }
    int getBirthYear() const {
        return birth_year;
    }
    int getIssueYear() const {
        return issue_year;
    }
    int getExpirationYear() const {
        return expiration_year;
    }
    const std::string &getHeight() const {
        return height;
    }
    const std::string &getHairColor() const {
        return hair_color;
    }
    const std::string &getEyeColor() const {
        return eye_color;
    }
    const std::string &getPassportID() const {
        return passport_id;
    }
    const std::string &getCountryID() const {
        return country_id;
    }

  private:
    int birth_year = -1;
    int issue_year = -1;
    int expiration_year = -1;
    std::string height = "";
    std::string hair_color = "";
    std::string eye_color = "";
    std::string passport_id = "";
    std::string country_id = "";
};

// all validations could've been done here, but oh well
std::vector<Passport> getPassports( std::ifstream &file ) {
    std::vector<Passport> ret{};
    std::string str;
    ret.emplace_back();
    while ( std::getline( file, str ) ) {
        if ( str.empty() || str == "\n" ) {
            ret.emplace_back();
            continue;
        }
        std::stringstream ss( str );
        while ( ss >> str ) {
            auto key = str.substr( 0, 3 );
            auto value = str.substr( 4 );
            if ( key == "byr" )
                ret.back().setBirthYear( std::stoi( value ) );
            else if ( key == "iyr" )
                ret.back().setIssueYear( std::stoi( value ) );
            else if ( key == "eyr" )
                ret.back().setExpirationYear( std::stoi( value ) );
            else if ( key == "hgt" )
                ret.back().setHeight( value );
            else if ( key == "hcl" )
                ret.back().setHairColor( value );
            else if ( key == "ecl" )
                ret.back().setEyeColor( value );
            else if ( key == "pid" )
                ret.back().setPassportID( value );
            else if ( key == "cid" )
                ret.back().setCountryID( value );
        }
    }
    return ret;
}

bool hasAllValues( const Passport &passport ) {
    bool valid = true;
    valid &= passport.getBirthYear() != -1;
    valid &= passport.getIssueYear() != -1;
    valid &= passport.getExpirationYear() != -1;
    valid &= !passport.getHeight().empty();
    valid &= !passport.getHairColor().empty();
    valid &= !passport.getEyeColor().empty();
    valid &= !passport.getPassportID().empty();
    return valid;
}

int allValues( const std::vector<Passport> &passports ) {
    int count = 0;
    for ( auto &passport : passports ) {
        if ( hasAllValues( passport ) )
            count++;
    }
    return count;
}

bool validHeight( const std::string &height ) {
    int height_value = -1;
    std::string height_unit = "";
    std::stringstream ss( height );
    ss >> height_value;
    ss >> height_unit;
    if ( height_value == -1 || ( height_unit != "cm" && height_unit != "in" ) )
        return false;
    if ( height_unit == "cm" )
        return height_value >= 150 && height_value <= 193;
    return height_value >= 59 && height_value <= 76;
}

bool validHex( const std::string &hex ) {
    if ( hex.length() > 7 || hex.length() < 7 )
        return false;
    bool valid = true;
    valid &= hex[0] == '#';
    for ( int i = 1; i < 7; i++ ) {
        valid &= std::isdigit( hex[i] ) || ( hex[i] >= 'a' && hex[i] <= 'f' );
    }
    return valid;
}

bool validEye( const std::string &eye ) {
    return eye == "amb" || eye == "blu" || eye == "brn" || eye == "gry" ||
           eye == "grn" || eye == "hzl" || eye == "oth";
}

bool validPID( const std::string &pid ) {
    if ( pid.length() < 9 || pid.length() > 9 )
        return false;
    bool valid = true;
    for ( int i = 0; i < 9; i++ ) {
        valid &= std::isdigit( pid[i] );
    }
    return valid;
}

int validValues( const std::vector<Passport> &passports ) {
    int count = 0;
    for ( auto &passport : passports ) {
        if ( !hasAllValues( passport ) )
            continue;
        bool valid = true;
        valid &=
            passport.getBirthYear() >= 1920 && passport.getBirthYear() <= 2002;
        valid &=
            passport.getIssueYear() >= 2010 && passport.getIssueYear() <= 2020;
        valid &= passport.getExpirationYear() >= 2020 &&
                 passport.getExpirationYear() <= 2030;
        valid &= validHeight( passport.getHeight() );
        valid &= validHex( passport.getHairColor() );
        valid &= validEye( passport.getEyeColor() );
        valid &= validPID( passport.getPassportID() );
        if ( valid )
            count++;
    }
    return count;
}

int main() {
    std::ifstream input_file( "input" );
    auto passports = getPassports( input_file );
    auto res = allValues( passports );
    std::cout << "There are \033[93;1m" << res
              << "\033[0m passports with all required values" << std::endl;
    res = validValues( passports );
    std::cout << "There are \033[93;1m" << res
              << "\033[0m actually valid passports" << std::endl;
}
