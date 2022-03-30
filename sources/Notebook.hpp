#include <string>
#include "Direction.hpp"
#include "Page.hpp"
#include <unordered_map>
#include <iostream>

namespace ariel{
    
    class Notebook{
        /* clean class just to compile all via instructions of Ex2 part a */
        public:

            std::unordered_map<int, Page> nbook;

            Notebook() = default;
            ~Notebook() = default;
            
            void write(int page, int row, int col, ariel::Direction dir, const std::string & content);
            std::string read(int page, int row, int col, ariel::Direction dir, int len);
            void erase(int page, int row, int col, ariel::Direction dir, int len);
            void show(int page);
    };
};