#include "./managers/game.hpp"

int main(){
    try{
        game();
    }
    catch (const std::exception& e){
        std::cerr << "eyyy" << e.what() << '\n';
    }
}