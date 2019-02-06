#include <SFML/Graphics.hpp>
#include <vector>           //for the better method
#include <iostream>
#include <algorithm>
#include <windows.h>

struct cell{
    int val;                                //val
    bool fixed;                             //fixed or nah
    std::vector<int> possibleChoices;       //for the better method
    sf::Text text;
};
class sudokuBoard{
    private:
        cell* _board[9][9];
        int _iterations;
    public:
        sudokuBoard(int**, sf::Font &);
        bool insert(int,int,int);
        bool checkRow(int,int);
        bool checkCol(int,int);
        bool checkSqr(int,int);
        bool fullBoard();
        bool emptyBoard();
        void solve(int,int,bool, sf::RenderWindow &, sf::RectangleShape[][9], 
        sf::RectangleShape&, sf::Text&, 
        sf::RectangleShape&, sf::Text&, 
        sf::RectangleShape&, sf::RectangleShape&, sf::Text&);           //brute-force + backtracking method
        cell* fillChoices();                 //(for method 2) and return the pointer of cell
        bool solve(cell*, sf::RenderWindow &, sf::RectangleShape[][9], 
        sf::RectangleShape&, sf::Text&, 
        sf::RectangleShape&, sf::Text&, 
        sf::RectangleShape&, sf::RectangleShape&, sf::Text&);         //for method 2
        int getIters();
        void printBoard(sf::RenderWindow &, sf::RectangleShape[][9], 
        sf::RectangleShape&, sf::Text&, 
        sf::RectangleShape&, sf::Text&, 
        sf::RectangleShape&, sf::RectangleShape&, sf::Text&);  
        cell* getCell(int, int);
        void fillCell(int, int, int);           //similar to insert (i just dont want to change it)
        void clearBoard();
};
 
