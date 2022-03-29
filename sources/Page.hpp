#include <iostream>
#include <string>
#include <vector> 

namespace ariel
{
    class Page 
    {
        public:
            unsigned int page_idx; 
            std::vector<std::string> content; // each field of the vector represnts a line at the page which holds 100 chars 
            bool clear;
            Page() = default;
            Page(unsigned int page_idx);
            ~Page() = default;
            void show();
    };
}