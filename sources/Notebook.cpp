#include <iostream>
#include <string>
#include <stdexcept>
#include "Notebook.hpp"
#include <vector> 


constexpr int P_COL = 100;
std::string EMPTY_ROW = "______________________________________________________________________________________________________________";
constexpr int MIN_ASCII = 32;
constexpr int MAX_ASCII = 125 ;
// #define EMPTY_ROW "______________________________________________________________________________________________________________"
namespace ariel
{

int verify_input(int page_idx, int row, int col, int len)
{

    if (page_idx < 0 || row < 0 || col < 0 || len < 0){ return 0; }
    
    if (col >= P_COL)
    {
        return 0;
    }
    return 1;
}

bool validate_asci(std::string content)
{
    for (unsigned int i=0; i < content.length(); i++)
    {
        if (MIN_ASCII > content[i] || MAX_ASCII < content[i])
        {
            throw std::invalid_argument("wont be able to write this shit..\n");
        }
    }
    return true;
}

bool validate_border(int col, int len)
{ /* validate check for notebook before read/write/erase while HORIZOTAL command*/
    return col + len - 1 < P_COL;
}

bool validate_erase(unsigned int page_idx , unsigned int row, unsigned int col, ariel::Direction dir, unsigned int len, Page & page)
{ /* check if we gonna write at field which has been erased */
    if (dir == ariel::Direction::Horizontal)
    {
        for (unsigned int i=col; i < col+len; i++)
        {
            if (page.content[row].at(i) == '~'){ return false; }
        }
    }
    else { // dir is vertical 
        for (unsigned int i=row; i < row+len; i++)
        {
            if (page.content[i].at(col) == '~'){ return false; }
        }
    }
    return true; // not gonna write on erase
}

bool validate_content(unsigned int page_idx , unsigned int row, unsigned int col, ariel::Direction dir, unsigned int len, Page & page)
{ /* check if we gonna write at field which has been writen */
    if (dir == ariel::Direction::Horizontal)
    {
        for (unsigned int i=col; i < col+len; i++)
        {
            if (page.content[row].at(i) != '_'){ return false; }
        }
    }
    else { // dir is vertical 
        for (unsigned int i=row; i < row+len; i++)
        {
            if (page.content[i].at(col) != '_'){ return false; }
        }
    }
    return true; // not gonna write on writen
}

void order_page(Page & page, int row)
{ /* will ensure that the page hold all relevant lines before read/write/erase */
    for (int i=0; i <= row; i++)
    {
        if (page.content.size() <= row){page.content.push_back(EMPTY_ROW); }
    }
}

// class Notebook{
    /* this class represents a notebook using funcs of write,read,erase and show. using the Page class as objects of the notebook */

    // public:

        // Notebook(){/* have nothing really to construct since declaring on nbook itself is enough to add items */}
        // ~Notebook()
        // { /* shall delete all pages from notebook (release memory) then delete the notebook obj itself */
        //     std::unordered_map<int, Page>::iterator iter;
        //     for(iter = nbook.begin(); iter != nbook.end(); iter++)
        //     {
        //         iter->second.~Page();
        //     }
        //     nbook.clear();
        // }
    
        void Notebook::write(int page_idx, int row, int col, ariel::Direction dir, const std::string & content)
        { /* write specific string to page at idx 'page_idx', can write horizontal or vertical 
                this func throws invalid exception when col+content.size > 100 (horizontal), outside border of page
                also throws runtime_erro when user asked to write at 'erased' char place */

            if(verify_input(page_idx, row, col, content.length()) == 0){ throw std::invalid_argument("fk it\n");}

            if (!validate_border(col, content.length()) && dir == ariel::Direction::Horizontal){ throw std::invalid_argument("writing horizontal will exit the border of 100 chars in a line \n"); }
            // border is relevant only for horizontal
            
            validate_asci(content);

            if (nbook.count(page_idx) == 0)
            { // doesnt have this page yet, shall create an empty page
                Page temp = Page((unsigned int)page_idx);
                nbook[page_idx] = temp;
            }
            // if rows is too below in compare to the curr page str, shall add empty rows as needed


            if (dir == ariel::Direction::Vertical){ order_page(nbook[page_idx], row+(int)content.length()); }
            else { order_page(nbook[page_idx], row); } // horizontal dir

            if (!validate_content((unsigned int)page_idx, (unsigned int)row, (unsigned int)col, dir, content.length(), nbook[page_idx])) { throw std::runtime_error("gonna write on writen symbol while writing\n"); }

            if (!validate_erase((unsigned int)page_idx, (unsigned int)row, (unsigned int)col, dir, content.length(), nbook[page_idx])) { throw std::runtime_error("gonna write on '~' symbol while writing\n"); }
            

            for (unsigned int i=0; i < content.length(); i++)
            // write the content at the page char by char
            {
                if (dir == ariel::Direction::Horizontal)
                {
                    
                    // .replace((unsigned int)col+i, 1, std::string(1, content.at(i)))
                    nbook[page_idx].content[(unsigned int)row][(unsigned int)col+i] = content.at(i);;
                }
                else // vertical
                {
                    // .replace((unsigned int)col, 1, std::string(1, content.at(i)))
                    nbook[page_idx].content[(unsigned int)row+i][(unsigned int)col] = content.at(i);;
                }
            }
            // nbook[page_idx].show();
        }
        

        std::string Notebook::read(int page_idx, int row, int col, ariel::Direction dir, int len)
        { /* this func read from specific page (page_idx) in specific dir a len bytes of str from the page 
                this func throws invalid exception when col+content.size > 100 (horizontal), outside border of page */
            if(verify_input(page_idx, row, col, len) == 0){throw std::invalid_argument("fk it\n");}

            if (!validate_border(col, len) && dir == ariel::Direction::Horizontal){ throw std::invalid_argument("reading horizontal will exit the border of 100 chars in a line \n"); }
            // border is relevant only for horizontal

            std::string output;
            if (nbook.count(page_idx) == 0)
            { // doesnt have this page yet, shall create an empty page
                Page temp = Page((unsigned int)page_idx);
                nbook[page_idx] = temp;
            }

            // if rows is too below in compare to the curr page str, shall add empty rows as needed
            if (dir == ariel::Direction::Vertical){ order_page(nbook[page_idx], row+len); }
            else { order_page(nbook[page_idx], row); } // horizontal dir


            for (unsigned int i=0; i < len; i++)
            // read the content at the page char by char
            {
                if (dir == ariel::Direction::Horizontal)
                {
                    output += nbook[page_idx].content[(unsigned int)row][(unsigned int)col+i];
                }
                else // vertical
                {
                    output += nbook[page_idx].content[(unsigned int)row+i][(unsigned int)col];
                }
            }

            return output; 
        }

        void Notebook::erase(int page_idx, int row, int col, ariel::Direction dir, int len)
        { /* change from location in the page chars in dir and len inputed to '~' which symbolize "erasing" 
                this func throws invalid exception when col+content.size > 100 (horizontal), outside border of page */
            if(verify_input(page_idx, row, col, len) == 0){throw std::invalid_argument("fk it\n");}

            if (!validate_border(col, len) && dir == ariel::Direction::Horizontal){ throw std::invalid_argument("erasing horizontal will exit the border of 100 chars in a line \n"); }
            // border is relevant only for horizontal

            if (nbook.count(page_idx) == 0)
            { // doesnt have this page yet, shall create an empty page
                Page temp = Page((unsigned int)page_idx);
                nbook[page_idx] = temp;
            }

            // if rows is too below in compare to the curr page str, shall add empty rows as needed
            if (dir == ariel::Direction::Vertical){ order_page(nbook[page_idx], row+len); }
            else { order_page(nbook[page_idx], row); } // horizontal dir

            for (unsigned int i=0; i < len; i++)
            // erase to '~' the content at the page char by char to the len
            {
                if (dir == ariel::Direction::Horizontal)
                {
                    nbook[page_idx].content[(unsigned int)row][(unsigned int)col+i] = '~';
                }
                else // vertical
                {
                    nbook[page_idx].content[(unsigned int)row+i][(unsigned int)col] = '~';
                }
            }
            // nbook[page_idx].show();
        }
        
        void Notebook::show(int page_idx)
        { /* prints the asken page via calling the show func of the specific page if exists, else: print empty page */
            /* 
            format of printing:
                'page:' <page_idx>
                1: content (string)
                2: content (string) etc.. 
                <last_page_row+1>: EMPTY_ROW 
            */
            if (page_idx < 0 ){ throw std::invalid_argument("negatve page num.. \n"); }

            if (nbook.count(page_idx) == 0)
            { // print the content of a specific page
                nbook[page_idx].show();
            }
            else
            { // print empty page
                std::cout << "page: " << page_idx << std::endl;
                std::cout << "0: " << EMPTY_ROW << std::endl;
            }
        }
// };
}