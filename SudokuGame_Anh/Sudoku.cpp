#include <string>
#include <fstream>

#include "sudokuBoard.h"

//predefine some colors:
sf::Color black       = sf::Color::Black;
sf::Color white       = sf::Color::White;
sf::Color red         = sf::Color::Red;
sf::Color green       = sf::Color::Green;
sf::Color blue        = sf::Color::Blue;
sf::Color yellow      = sf::Color::Yellow;
sf::Color magenta     = sf::Color::Magenta;
sf::Color cyan        = sf::Color::Cyan;
sf::Color transparent = sf::Color::Transparent;

//IMPORTANT REMINDER:
//ALL SF OBJECTS SHOULD BE PASSED
//BY REFERENCE
 
int** initSudoku();
void initRect(sf::RectangleShape [][9]);
void highlightCell(sf::RectangleShape [][9], sf::Vector2f&, bool [][9]);
void fillBlackOutline(sf::RectangleShape [][9]);
bool highlighted(sf::RectangleShape [][9]);
sf::RectangleShape getHighlighted(sf::RectangleShape [][9]);
bool selected(bool [][9]);
void deselect(bool [][9]);
void initSolveButt(sf::RectangleShape&);
void initSolveText(sf::Text&, sf::Font&);
void initClearButt(sf::RectangleShape&);
void initClearText(sf::Text&, sf::Font&);
void initInsText(sf::Text&, sf::Font&);
bool mouseOnRect(sf::RectangleShape&, sf::Vector2f&);
int getSelectedRow(bool [][9]);
int getSelectedCol(bool [][9]);

// WinMain to hide the window console
int WinMain(HINSTANCE hInstance,
            HINSTANCE hPrevInstance, 
            LPTSTR    lpCmdLine, 
            int       cmdShow)
// int main()
{
    sf::Font font;
    font.loadFromFile("arial.ttf");
    int** sampleSudoku = initSudoku();

    sudokuBoard* sBoard = new sudokuBoard(sampleSudoku, font);

    sf::RenderWindow window(sf::VideoMode(325, 430), "Sudoku Solver by Tran Duy Anh");
    
    bool                selectedCell[9][9] = {false};//Set all Cells to be not selected by default (lame solution to lame problem)
    sf::RectangleShape  rectArr[9][9];
    sf::RectangleShape  solveButt;
    sf::RectangleShape  clearButt;
    sf::RectangleShape  highlightedRect;
    sf::RectangleShape  selectedRect;
    sf::Text            solveText;
    sf::Text            clearText;
    sf::Text            insText;

    initRect(rectArr);
    initSolveButt(solveButt);
    initSolveText(solveText, font);
    initClearButt(clearButt);
    initClearText(clearText, font);
    initInsText(insText, font);
    highlightedRect = rectArr[0][0];
    selectedRect = rectArr[0][0];

    while (window.isOpen())
    {
        sf::Event event;
        // get the current mouse position in the window
        sf::Vector2i pixelPos = sf::Mouse::getPosition(window);
        // convert it to world coordinates
        sf::Vector2f worldPos = window.mapPixelToCoords(pixelPos);

        while (window.pollEvent(event))
        {
            switch (event.type){
                case sf::Event::Closed:
                    window.close();
                    break;

                case sf::Event::TextEntered:
                    if(selected(selectedCell)){
                        // Handle ASCII characters only
                        int num;
                        if (event.text.unicode <= 57 && event.text.unicode >= 48){
                            num = event.text.unicode - 48; //XDDDDD
                            sBoard->fillCell(getSelectedRow(selectedCell), getSelectedCol(selectedCell), num);
                        }  
                        deselect(selectedCell);  
                        break;                                  
                    }
            }
        }

        window.clear();
        highlightedRect = getHighlighted(rectArr);
        selectedRect = rectArr[getSelectedRow(selectedCell)][getSelectedCol(selectedCell)];

        if (sf::Mouse::isButtonPressed(sf::Mouse::Left)){
            if(mouseOnRect(solveButt, worldPos) && solveText.getString() == "Solve!"){
                if(!sBoard->emptyBoard()){
                    solveText.setString("Solving...");
                    solveText.setPosition(25, 352);
                    insText.setFillColor(black);

                    //Note that there are two methods for solving. The currently commented method is not optimal
                    sBoard->solve(sBoard->fillChoices(), window, rectArr, solveButt, solveText, clearButt, clearText, highlightedRect, selectedRect, insText);  //where you draw EVERYTHING
                    // sBoard->solve(0,0,false, window, rectArr, solveButt, solveText, clearButt, clearText, highlightedRect, insText);
                    
                    solveText.setString("Done!");
                    solveText.setPosition(28, 352);
                    insText.setFillColor(white);
                }
                
            }
            if(mouseOnRect(clearButt, worldPos)){
                sBoard->clearBoard();
                solveText.setString("Solve!");
            }
            for(int i = 0; i < 9; i++){
                for(int j = 0; j < 9; j++){
                    if(mouseOnRect(rectArr[i][j], worldPos) && solveText.getString() == "Solve!"){
                        selectedCell[i][j] = true;
                    }else{
                        selectedCell[i][j] = false;
                    }
                }
            }
        }
        
        if(!mouseOnRect(solveButt, worldPos) && !mouseOnRect(clearButt, worldPos)){
            insText.setString("");        
        }

        if(mouseOnRect(solveButt, worldPos) && solveText.getString() == "Solve!"){
            
            if(sBoard->emptyBoard()){
                insText.setString("Board is empty!");
            }else{
                insText.setString("Click to Solve");
                solveButt.setOutlineColor(red);
            }
        }else{
            solveButt.setOutlineColor(blue);
        }

        if(mouseOnRect(clearButt, worldPos)){
            clearButt.setOutlineColor(red);
            insText.setString("Click to Reset the Board");
        }else{
            clearButt.setOutlineColor(blue);
        }

        if(solveText.getString() == "Solve!"){
            if(highlighted(rectArr)){ 
                insText.setString("Select this cell");
            }
            highlightCell(rectArr, worldPos, selectedCell);
        }else{
            fillBlackOutline(rectArr);
        }

        if(selected(selectedCell)){
            insText.setString("Key in the value");
        }

        
        sBoard->printBoard(window, rectArr, solveButt, solveText, clearButt, clearText, highlightedRect, selectedRect, insText);

    }

    return 0;
}


sf::RectangleShape getHighlighted(sf::RectangleShape haha[][9]){
    for(int i = 0; i < 9; i++){
        for(int j = 0; j < 9; j++){
            if(haha[i][j].getOutlineColor() == green) return haha[i][j];
        }
    }
    return haha[0][0];
}

int getSelectedCol(bool haha[][9]){
    for(int i = 0; i < 9; i++){
        for(int j = 0; j < 9; j++){
            if(haha[i][j]) return j;
        }
    }
    return 0;
}

int getSelectedRow(bool haha[][9]){
    for(int i = 0; i < 9; i++){
        for(int j = 0; j < 9; j++){
            if(haha[i][j]) return i;
        }
    }
    return 0;
}

void deselect(bool haha[][9]){
    for(int i = 0; i < 9; i++){
        for(int j = 0; j < 9; j++){
            if(haha[i][j]) haha[i][j] = false;
        }
    }
}

bool selected(bool haha[][9]){
    for(int i = 0; i < 9; i++){
        for(int j = 0; j < 9; j++){
            if(haha[i][j]) return true;
        }
    }
    return false;
}

bool highlighted(sf::RectangleShape haha[][9]){
    for(int i = 0; i < 9; i++){
        for(int j = 0; j < 9; j++){
            if(haha[i][j].getOutlineColor() == green) return true;
        }
    }
    return false;
}

//if mouse is placed on a cell, return true
bool mouseOnRect(sf::RectangleShape& solveButt, sf::Vector2f& worldPos){
    if( worldPos.x >= solveButt.getPosition().x && worldPos.x <= solveButt.getPosition().x + solveButt.getSize().x
        && worldPos.y >= solveButt.getPosition().y && worldPos.y <= solveButt.getPosition().y + solveButt.getSize().y){
            return true;
    }
    return false;
}

void initInsText(sf::Text& text, sf::Font& font){
    text.setString("Select a cell");
    text.setFont(font);
    text.setCharacterSize(24);
    text.setStyle(sf::Text::Bold);
    // text.setFillColor(white);
    text.setPosition(20, 390);
}

//initialize the Solve Text 
void initSolveText(sf::Text& text, sf::Font& font){
    text.setString("Solve!");
    text.setFont(font);
    text.setCharacterSize(24);
    text.setStyle(sf::Text::Bold);
    text.setFillColor(black);
    text.setPosition(28, 352);
}

//initialize the Solve Text 
void initSolveButt(sf::RectangleShape &butt){
    butt.setSize(sf::Vector2f(130, 30));
    butt.setOutlineColor(blue);
    butt.setOutlineThickness(2);
    // butt.setFillColor(sf::Color::White);
    butt.setPosition(20,350);
}

//initialize the Clear Text 
void initClearText(sf::Text& text, sf::Font& font){
    text.setString("Clear all");
    text.setFont(font);
    text.setCharacterSize(24);
    text.setStyle(sf::Text::Bold);
    text.setFillColor(black);
    text.setPosition(178, 352);
}

//initialize the Clear Button
void initClearButt(sf::RectangleShape &butt){
    butt.setSize(sf::Vector2f(130, 30));
    butt.setOutlineColor(blue);
    butt.setOutlineThickness(2);
    // butt.setFillColor(sf::Color::White);
    butt.setPosition(170,350);
}

//initialize all the individual cells of the sudoku board
void initRect(sf::RectangleShape arr[][9]){
    int size = 32;
    int border = 3;
    int marginLeft = border;
    int marginTop = border;
    for(int i = 0; i < 9; i++){
        marginLeft = border;
        for(int j = 0; j < 9; j++){
            
            arr[i][j].setSize(sf::Vector2f(size, size));
            arr[i][j].setOutlineColor(black);
            arr[i][j].setOutlineThickness(border);
            arr[i][j].setPosition(marginLeft,marginTop);
            
            if(j % 3 == 2){
                marginLeft += size + border*2;
            }else{
                marginLeft += size + border;
            }
        }

        if(i % 3 == 2){
            marginTop += size + border*2;
        }else{
            marginTop += size + border;
        }
    }

}

void fillBlackOutline(sf::RectangleShape arr[][9]){
    for(int i = 0; i < 9; i++){
        for(int j = 0; j < 9; j++){
            arr[i][j].setOutlineColor(black);
        }
    }
}

void highlightCell(sf::RectangleShape arr[][9],sf::Vector2f& mousePos, bool boolArr[][9]){
    
    for(int i = 0; i < 9; i++){
        for(int j = 0; j < 9; j++){
            if(mouseOnRect(arr[i][j], mousePos)){
                arr[i][j].setOutlineColor(green);
            }else{
                if(boolArr[i][j]){
                    arr[i][j].setOutlineColor(red);
                }else{
                    arr[i][j].setOutlineColor(black);
                }
            }
        }
    }
}

//initialize a 9*9 empty sudoku board
int** initSudoku(){
    int** sud;
    sud = new int*[9];
    for(int i = 0; i < 9; i++){
        sud[i] = new int[9];
        for(int j = 0; j < 9; j++){
            sud[i][j] = 0;
        }
    }
    return sud;
}