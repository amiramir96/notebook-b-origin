#include "Page.hpp"
#include <iostream>
#include <string>
#include <vector> 

constexpr int PP_COL = 100;
std::string EMPT = "______________________________________________________________________________________________________________";
// #define EMPTY_ROW "______________________________________________________________________________________________________________"

// class Page
// { /* this class represent a specific page in a notebook*/ 
// public:
namespace ariel
{
        // unsigned int page_idx; 
        // std::vector<std::string> content; // each field of the vector represnts a line at the page which holds 100 chars 
        // bool clear;
        // Page(){}
        Page::Page(unsigned int idx)
        { /* constructor */
            // init vars
            page_idx = idx;
            content = {}; 
            clear = true;
        }
        // ~Page(){ /* dest */content.clear(); }
        void Page::show()
        { 
            /* two cases of show: 
                    1. page is clear -> shall print only first empty row
                    2. page isnt clear -> shall print whole rows to make a page + one more empty line in the end of the page
            */
            std::cout << "page: " << page_idx << std::endl; 
            // if (clear)
            // {
            //     std::cout << "0: " << EMPT << std::endl;
            // }
            // else 
            // {
                unsigned int i=0;
                for (; i < content.size() ; i++)
                {
                    std::cout << i << " " << content[i] << std::endl;
                }
                std::cout << i << " " << EMPT << std::endl; 
            // }
        }
// };
}