#include <cstdint>
#include <iostream>
#include <memory>
#include <tuple>
#include <vector>
#include <array>
#include <algorithm>
#include <string>
#include <string_view>
#include <thread>
#include "../enigma/enigma.hpp"
#include "../enigma/generator.hpp"
#include <toml.hpp>
#include <map>

class Bombe {

    public:
        Bombe();
        Bombe(Bombe & );
        ~Bombe();
        void read_toml( const std::string & );
        void set_machines();
        void break_enigma();
        std::string result() const ;

    private:
        std::vector<std::tuple< std::array<std::string,4> , std::string , Enigma , std::string >> smachines;
        std::vector<char> ringstellum;
        std::vector< std::array<int, 2> > plugboard;
        std::vector<std::string> reflector;
        std::vector<std::string> rotors;
        std::map<std::string,std::tuple< std::array<std::string,4> , std::string , Enigma , std::string > > cypher_values;
        int permutations;
        std::string message;
        std::string target;
};