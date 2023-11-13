# Breaking-Enigma
My own version of the Alan Turing "Bombe" machine for breaking [Enigma](https://github.com/MoskitoSantana/Inside-Enigma)

# What is the Bombe Machine ? 💣
As the German military grew, so did the number of messages sent using the Enigma. It began to overwhelm the small staff of cryptanalysts in Poland. They realized that the time-consuming hand-worked method of analysis would not be sufficient. Marian Rejewski developed plans for a machine that could, through brute force, work through the more than 17,000 possible positions.3 The machine was called a Bomba.

Alan Turing realized that the solution did not lie in creating a machine that replicated sixty Enigmas. The Polish Bomba searched for matches in indicators. Once already the Germans had changed how indicators were used, throwing the Poles back into the darkness until new Zygalski sheets could be cut. The Germans could easily change the indicators again. Turing began thinking about a machine that worked, not with the indicators, but with assumed text. By using text that cryptanalysts

If you wanna know more about this machine 
+ [Wikipedia]()
+ [USDD](https://media.defense.gov/2022/Sep/29/2003087366/-1/-1/0/SOLVING%20THE%20ENIGMA%20-%20HISTORY%20OF%20THE%20CRYPTANALYTIC%20BOMBE.PDF)
+ Watch the movie ( The Imitation Game )
+ Read the book ( Codebreakers )

# Usage 

### Configuration File 🧰
Must be a toml file with the following structure :
```toml
[ringstellum-config]
ringstellum=[ '@' , '{' , '/', 'K' ]

[plugboard-config]
plugboard=[ [ 4 , 54 ] , [ 90 , 24 ] , [ 5 , 75 ] , [84 , 31] ]

[reflector-config]
reflectors=["A","E","F","D"]

[rotor-config]
rotors=[ "I" ,  "V" ,  "VII"  , "IV" ]

[permutations-config]
permutations = 368

[text]
message="AAA"
target="n(Q"
```

### C++
The easy and the only way 🌚 :
```cc
    Bombe turing ;
    turing.read_toml("config.toml");
    turing.set_machines();
    turing.break_enigma();
    std::cout << turing.result()<< std::endl;
```
