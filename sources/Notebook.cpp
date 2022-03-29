#include <iostream>
#include <string>
#include <stdexcept>
#include "Notebook.hpp"
#include <vector> 


#define P_COL 100
std::string EMPTY_ROW = "______________________________________________________________________________________________________________";
 
// #define EMPTY_ROW "______________________________________________________________________________________________________________"
namespace ariel
{

void verify_input(int page_idx, int row, int col, int len)
{
    if (page_idx < 0 || row < 0 || col < 0 or len < 0)
    {
        throw std::invalid_argument("input is negative\n");
    }
    if (col >= 100)
    {
        throw std::invalid_argument("write len is out of bound\n");
    }

}

bool validate_asci(std::string content)
{
    for (unsigned int i=0; i < content.length(); i++)
    {
        if (31 > content[i] || 127 < content[i])
        {
            throw std::invalid_argument("wont be able to write this shit..\n");
        }
    }
    return true;
}

bool validate_border(int col, int len)
{ /* validate check for notebook before read/write/erase while HORIZOTAL command*/
    return col + len < P_COL;
}

bool validate_erase(unsigned int page_idx , unsigned int row, unsigned int col, ariel::Direction dir, unsigned int len, Page page)
{ /* check if we gonna write at field which has been erased */
    printf("im here \n");
    if (dir == ariel::Direction::Horizontal)
    {
        printf("hori\n");
        for (unsigned int i=col; i < col+len; i++)
        {
            if (page.content[row].at(i) == '~'){ return false; }
        }
    }
    else { // dir is vertical 
        printf("just verti\n");
        printf("verti %d, %d, %d,         %lu, |||%s||| \n",row, col, len, page.content.size(), page.content[row].c_str());
        for (unsigned int i=row; i < row+len; i++)
        {
            if (page.content[col].at(i) == '~'){ return false; }
        }
    }
    return true; // not gonna write on erase
}

void order_page(Page page, int row)
{ /* will ensure that the page hold all relevant lines before read/write/erase */
    printf("order page start\n");
    for (int i=0; i <= row; i++)
    {
        printf("order_page iterate row %d, i %d\n", row, i);
        if (page.content.size() <= row){page.content.push_back(EMPTY_ROW); printf("??? %lu, %s\n",page.content.size(), page.content[0].c_str()); }
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
    
        void Notebook::write(int page_idx, int row, int col, ariel::Direction dir, std::string content)
        { /* write specific string to page at idx 'page_idx', can write horizontal or vertical 
                this func throws invalid exception when col+content.size > 100 (horizontal), outside border of page
                also throws runtime_erro when user asked to write at 'erased' char place */
            // return;
            verify_input(page_idx, row, col, content.length());

            if (!validate_border(col, content.length()) && dir == ariel::Direction::Horizontal){ throw std::invalid_argument("writing horizontal will exit the border of 100 chars in a line \n"); }
            // border is relevant only for horizontal
            
            validate_asci(content);
            printf("passed first validation\n ");
            if (nbook.count(page_idx) == 0)
            { // doesnt have this page yet, shall create an empty page
                Page temp = Page((unsigned int)page_idx);
                nbook[page_idx] = temp;
            }
            printf("added new empty page\n");
            // if rows is too below in compare to the curr page str, shall add empty rows as needed


            if (dir == ariel::Direction::Vertical){ order_page(nbook[page_idx], (unsigned int)row+content.length()); }
            else { order_page(nbook[page_idx], row); } // horizontal dir
            printf("ordered\n");
            for (unsigned int j=0; j < nbook[page_idx].content.size(); j++){printf("%s\n",nbook[page_idx].content[j].c_str());}
            
            if (!validate_erase((unsigned int)page_idx, (unsigned int)row, (unsigned int)col, dir, content.length(), nbook[page_idx])) { throw std::runtime_error("gonna write on '~' symbol while writing\n"); }
            printf("passed val_erase\n");
            

            for (unsigned int i=0; i < content.length(); i++)
            // write the content at the page char by char
            {
                if (dir == ariel::Direction::Horizontal)
                {
                    nbook[page_idx].content[(unsigned int)row][(unsigned int)col+i] = content.at(i);
                }
                else // vertical
                {
                    nbook[page_idx].content[(unsigned int)row+i][(unsigned int)col] = content.at(i);
                }
            }
            printf("falled at end? :O\n");
        }
        

        std::string Notebook::read(int page_idx, int row, int col, ariel::Direction dir, int len)
        { /* this func read from specific page (page_idx) in specific dir a len bytes of str from the page 
                this func throws invalid exception when col+content.size > 100 (horizontal), outside border of page */
            return "";
            verify_input(page_idx, row, col, len);
            std::string output;
            if (!nbook.count(page_idx))
            { // doesnt have this page yet, shall create an empty page
                Page temp = Page((unsigned int)page_idx);
                nbook[page_idx] = temp;
            }

            if (!validate_border(col, len) && dir == ariel::Direction::Horizontal){ throw std::invalid_argument("reading horizontal will exit the border of 100 chars in a line \n"); }
            // border is relevant only for horizontal

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
            return;
            verify_input(page_idx, row, col, len);

            if (!validate_border(col, len) && dir == ariel::Direction::Horizontal){ throw std::invalid_argument("erasing horizontal will exit the border of 100 chars in a line \n"); }
            // border is relevant only for horizontal

            if (!nbook.count(page_idx))
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

            if (nbook.count(page_idx))
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