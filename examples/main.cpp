#include <array>
#include <cassert>
#include <string_view>
#include <iostream>
#include <vector>
#include "../src/bombe/bombe.hpp"


int main( int argc , char * args[]){
    if (argc < 2) return 1;
    Bombe turing ;
    turing.read_toml(args[1]);
    turing.set_machines();
    turing.break_enigma();
    std::cout << turing.result()<< std::endl;
    return 0;
}   