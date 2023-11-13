#include "bombe.hpp"
#include <algorithm>
#include <array>
#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <exception>
#include <iostream>
#include <memory>
#include <ostream>
#include <random>
#include <string>
#include <string_view>
#include <thread>
#include <toml/get.hpp>
#include <toml/parser.hpp>
#include <tuple>
#include <utility>
#include <vector>

Bombe::Bombe(){
    this->cypher_values =  std::map<std::string,std::tuple< std::array<std::string,4> , std::string , Enigma , std::string > >();
    this->smachines = std::vector<std::tuple< std::array<std::string,4> , std::string , Enigma , std::string >>();
    this->plugboard =  std::vector<std::array<int,2>>() ;
    this->rotors = std::vector<std::string>();
    this->reflector = std::vector<std::string>();
    this->ringstellum = std::vector<char>();
    this->permutations = 0;
    this->message = "";
    this->target="";
}

Bombe::~Bombe(){
    
}

void Bombe::read_toml( const std::string & path ) {

    try {
        
        const auto file = toml::parse(path);

        const auto & ringstellum = toml::find(file,"ringstellum-config");
        const auto result = toml::find<std::vector<std::string>>(ringstellum,"ringstellum");

        for (const auto & item : result ) {
            this->ringstellum.push_back( static_cast<char>(item[0]) );
        }

        const auto & plugboard =  toml::find(file,"plugboard-config");
        this->plugboard = toml::find<std::vector<std::array<int,2>>>(plugboard,"plugboard");

        const auto & reflectors = toml::find(file,"reflector-config");
        this->reflector = toml::find<std::vector<std::string>>(reflectors,"reflectors");  
                
        const auto & rotors =  toml::find(file,"rotor-config");
        this->rotors = toml::find<std::vector<std::string>>(rotors,"rotors");
        
        const auto & permutations = toml::find(file,"permutations-config");
        this->permutations = toml::find<int>(permutations,"permutations");

        const auto & text = toml::find(file,"text");
        this->message = toml::find<std::string>(text,"message");
        this->target = toml::find<std::string>(text,"target");

    } catch ( std::exception &e ) {
        std::exit(1);
    }
}


void Bombe::break_enigma() {
    std::vector<std::thread> threads;
    std::vector<std::string> msgs;
    size_t index = 0;
    for( size_t i = 0 ; i < this->smachines.size() ; i++  ) msgs.push_back(this->message);    
    for( auto & machine : this->smachines ){
        if( index >= this->smachines.size() ) break;
        
        threads.push_back(
            std::thread(
                    [&machine , &code = msgs.at(index) ](){
                    std::get<2>(machine).encode_string(code);
                    std::get<3>(machine) = code;                
                }
            )
        );
        index++;
    }
    
    for ( auto & thread : threads ) {
        thread.join();
    }
    
    for (const auto & machine : this->smachines ) {
        this->cypher_values[std::get<3>(machine)] = machine;
    }


}

void Bombe::set_machines() {
    
    int permutations_counter = 0;

    while (permutations_counter < this->permutations) {
        std::vector<std::string> queue;
        queue.push_back(  this->reflector.at(0) ); 
        queue.push_back(  this->reflector.at(1) ); 
        queue.push_back(  this->reflector.at(2) ); 
        queue.push_back(  this->reflector.at(3) ); 

        std::shuffle(this->rotors.begin(), this->rotors.end(), randomizer());

        std::vector<std::string> vrotors;
        vrotors.push_back( Rotor::load_rotor_path( this->rotors.at(0) )); 
        vrotors.push_back( Rotor::load_rotor_path( this->rotors.at(1) )); 
        vrotors.push_back( Rotor::load_rotor_path( this->rotors.at(2) )); 
        vrotors.push_back( Rotor::load_rotor_path( this->rotors.at(3) )); 

        while (!queue.empty() ) {
            std::string current = queue.back();
            std::string creflector = Reflector::load_reflector_path( current );
            queue.pop_back();
            Enigma machine = Enigma();
            machine.manage_reflector_load( creflector );
            machine.manage_rotor_load( vrotors.at(0), vrotors.at(1), vrotors.at(2), vrotors.at(3));
            machine.ringstellum( this->ringstellum.at(0) ,this->ringstellum.at(1) ,this->ringstellum.at(2) ,this->ringstellum.at(3) );
            for( auto connector = this->plugboard.begin(); connector != this->plugboard.end(); connector++ ){
                machine.manage_plugboard_connections( (*connector).at(0) , (*connector).at(1) ) ;
            }
            this->smachines.push_back( std::tuple< std::array<std::string,4> , std::string , Enigma , std::string >( { this->rotors.at(0), this->rotors.at(1), this->rotors.at(2), this->rotors.at(3) } , current , machine , "" ) );
        }
        permutations_counter++;
    
    }        

}

std::string Bombe::result() const {
    if( this->cypher_values.count(this->target) == 1 ){
        return "[" + std::get<0>( this->cypher_values.at(this->target)).at(0) + "]" + 
               "[" + std::get<0>( this->cypher_values.at(this->target)).at(1) + "]" +
               "[" + std::get<0>( this->cypher_values.at(this->target)).at(2) + "]" +
               "[" + std::get<0>( this->cypher_values.at(this->target)).at(3) + "]" +
               " [" + std::get<1>( this->cypher_values.at(this->target)) + "] " +
               std::get<3>( this->cypher_values.at(this->target)) ;
    }
    return "Not aviable configuration";
}