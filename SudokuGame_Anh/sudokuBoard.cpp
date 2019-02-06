#include "sudokuBoard.h"


sudokuBoard::sudokuBoard(int** arr, sf::Font & font){

    int size = 32;
    int border = 3; //set border like this to make it right in the middle

    int marginLeft = border * 4;
    int marginTop = border;

    for(int i = 0; i < 9; i++){
        marginLeft = border*4;
        for(int j = 0; j < 9; j++){
            _board[i][j] = new cell{arr[i][j],(arr[i][j] != 0)};
        
            _board[i][j]->text.setFont(font);
            _board[i][j]->text.setPosition(marginLeft,marginTop);
            _board[i][j]->text.setStyle(sf::Text::Bold);
            _board[i][j]->text.setCharacterSize(24);
            _board[i][j]->text.setFillColor(sf::Color::Black);
            
            if(arr[i][j] == 0){
                _board[i][j]->text.setString("");
            }else{ 
                _board[i][j]->text.setString(std::to_string(arr[i][j]));
            }
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
    _iterations = 0;
}

bool duplicate(std::vector<int> vec){                //return true if there is duplicate
    sort(vec.begin(), vec.end());
    for(int i = 0; i < vec.size(); i++){
        if(vec[i] == vec[i+1] && vec[i] != 0)
            return true;
    }

    return false;
}

bool sudokuBoard::checkRow(int row, int col){       //true means bad false means good
    std::vector<int> rowVec;
    for(int i = 0; i < 9; i++){
        rowVec.push_back(_board[row][i]->val);
    }
    return duplicate(rowVec);                       
}

bool sudokuBoard::checkCol(int row, int col){
    std::vector<int> colVec;
    for(int i = 0; i < 9; i++){
        colVec.push_back(_board[i][col]->val);
    }
    return duplicate(colVec);

}

bool sudokuBoard::checkSqr(int row, int col){
    int startRow = row;
    int startCol = col;
    std::vector<int> sqrVec;

    while(startRow % 3 != 0)
        startRow--;
        
    while(startCol % 3 != 0)
        startCol--;

    for(int i = 0; i < 3; i++){
        for(int j = 0; j < 3; j++){
            sqrVec.push_back(_board[i+startRow][j+startCol]->val);
        }
    }

    return duplicate(sqrVec);

}

void sudokuBoard::fillCell(int row, int col, int val){
    _board[row][col]->val = val;
    if(val == 0){
        _board[row][col]->text.setString("");
        _board[row][col]->fixed = false;
    }else{
        _board[row][col]->text.setString(std::to_string(val));
        _board[row][col]->fixed = true;
    }
    
}

bool sudokuBoard::insert(int row, int col, int num){
    if(row > 8 
    || col > 8 
    || num > 9
    || _board[row][col]->fixed){
        // cout << "Uhh Hello? ILLEGAL" << endl;
        return false;
    }

    int temp = _board[row][col]->val;
    _board[row][col]->val = num;

    if(sudokuBoard::checkRow(row,col)){
        // cout <<"row " << row << " already has a " << num << endl;
        _board[row][col]->val = temp;
        return false;
    }

    if(sudokuBoard::checkCol(row,col)){
        // cout <<"column " << col << " already has a " << num << endl;
        _board[row][col]->val = temp;
        return false;
    }

    if(sudokuBoard::checkSqr(row,col)){
        // cout <<"square already has a " << num << endl;
        _board[row][col]->val = temp;
        return false;
    }
    
    _board[row][col]->text.setString(std::to_string(num));
    return true;
}

bool sudokuBoard::fullBoard(){
    for(int i = 0; i < 9; i++){
        for(int j = 0; j < 9; j++){
            if(_board[i][j]->val == 0)
                return false;
        }
    }
    return true;
}

void sudokuBoard::solve(int row, int col, bool backward, 
    sf::RenderWindow &winRef, sf::RectangleShape arr[][9],
    sf::RectangleShape& solveButt, sf::Text& solveText,
    sf::RectangleShape& clearButt, sf::Text& clearText, 
    sf::RectangleShape& highlighted, sf::RectangleShape& selected, sf::Text& insText){       //if true then go backward

    // cout << "now at " << "("<< row << ", " << col << ")" << endl;
    printBoard(winRef, arr, solveButt, solveText, clearButt, clearText, highlighted, selected, insText);
    _iterations++;
    if(fullBoard()){
        return;
    }

    if(_board[row][col]->fixed){        //if at fixed cell then just move on
        if(backward){                   //if going backward then continue back
            if(col <= 0){
                solve(row-1,8,true, winRef, arr, solveButt, solveText, clearButt, clearText, highlighted, selected, insText);
            }else{
                solve(row,col-1,true, winRef, arr, solveButt, solveText, clearButt, clearText, highlighted, selected, insText);
            }
        }else{                          //if going forward then continue forward
            if(col >= 8){
                solve(row+1,0,false, winRef, arr, solveButt, solveText, clearButt, clearText, highlighted, selected, insText);
            }else{
                solve(row,col+1,false, winRef, arr, solveButt, solveText, clearButt, clearText, highlighted, selected, insText);
            }
        }

    }else{                              //if at a changeable cell, change it and go forward        
        int num = _board[row][col]->val + 1;
        // cout << "("<< row << ", " << col << ") is: " << num -1;
        while(!insert(row,col,num) && num < 10){
            num++;
        }

        if(num >= 10){                          //set backward to true because now its wrong
            _board[row][col]->val = 0;          //tha moment u done goof u set it back to 0
            // cout << " backtracking new val is: " << _board[row][col]->val << endl;
            if(col < 1){
                solve(row-1,8,true, winRef, arr, solveButt, solveText, clearButt, clearText, highlighted, selected, insText);
            }else{
                solve(row,col-1,true, winRef, arr, solveButt, solveText, clearButt, clearText, highlighted, selected, insText);
            }
        }else{
            // cout << " new val is: " << _board[row][col]->val << endl;
            if(col >= 8){
                solve(row+1,0,false, winRef, arr, solveButt, solveText, clearButt, clearText, highlighted, selected, insText);
            }else{
                solve(row,col+1,false, winRef, arr, solveButt, solveText, clearButt, clearText, highlighted, selected, insText);
            }
        }
        
    }
    
}


cell* sudokuBoard::fillChoices(){                                    //fill the choices everytime theres a change in input
    cell* output = new cell{};                                      //just a randomcell to output
    int leastCount = 9;                                             //any integer big enough
    for(int i = 0; i < 9; i++){
        for(int j = 0; j < 9; j++){
            if(_board[i][j]->val == 0){
                // cout << "(" << i << ", " << j << ")" << " can be: ";
                _board[i][j]->possibleChoices.clear();              //clear the vector first u numbskull
                int num = 1;
                int count = 0;
                while(num < 10){                                   
                    if(insert(i,j,num)){
                        _board[i][j]->possibleChoices.push_back(num);
                        // cout << num << ", ";
                        ++count;
                    }
                    ++num;
                }
                // cout << endl;
                if(leastCount > count){
                    leastCount = count;
                    // cout << "least count is: " << leastCount;
                    // cout << " and we are at cell "<< "(" << i << ", " << j << ")"  << endl;
                    output = _board[i][j];
                }
                _board[i][j]->val = 0;
            }
        }
    }

    return output;
}

bool sudokuBoard::solve(cell* cellToFill, 
    sf::RenderWindow &winRef, sf::RectangleShape arr[][9],
    sf::RectangleShape& solveButt, sf::Text& solveText,
    sf::RectangleShape& clearButt, sf::Text& clearText, 
    sf::RectangleShape& highlighted, sf::RectangleShape& selected, sf::Text& insText){                 //the better method
    _iterations++;
    // Sleep(500);
    printBoard(winRef, arr, solveButt, solveText, clearButt, clearText, highlighted, selected, insText);
    bool result = true;
    if(fullBoard()){
        return result;
    }

    if(cellToFill->possibleChoices.size() == 0){                         //in the event there is no choice to fill, return false
        return !result;
    }
    
    for(int i = 0; i < cellToFill->possibleChoices.size(); i++){
        if(fullBoard()){
            return true;
        }

        cellToFill->val = cellToFill->possibleChoices[i];
        cellToFill->text.setString(std::to_string(cellToFill->val));
        if(!solve(fillChoices(),winRef,arr, solveButt, solveText, clearButt, clearText, highlighted, selected, insText)){               //if return false then make current val 0
            if(!fullBoard()){
                cellToFill->val = 0;
                cellToFill->text.setString("");
                result = false;
            }
        }
    }
    return result;

}


/*
The board will look something like this:
726 493 815
315 728 946
489 651 237

852 147 693
673 985 124
941 362 758

194 836 572
567 214 389
238 579 461

*/
void sudokuBoard::printBoard(sf::RenderWindow &win, sf::RectangleShape arr[][9],
    sf::RectangleShape& solveButt, sf::Text& solveText,
    sf::RectangleShape& clearButt, sf::Text& clearText, 
    sf::RectangleShape& highlighted, sf::RectangleShape& selected, sf::Text& insText){
    for(int i = 0; i < 9; i++){
        for(int j = 0; j < 9; j++){
            win.draw(arr[i][j]); 
        }
    }
    
    win.draw(highlighted);
    win.draw(selected);
    win.draw(solveButt);
    win.draw(solveText);
    win.draw(clearButt);
    win.draw(clearText);
    win.draw(insText);

    for(int i = 0; i < 9; i++){
        for(int j = 0; j < 9; j++){
            
            win.draw(_board[i][j]->text);  
        }
    }
    
    win.display();
    // std::cout << GHOSTNUMBER << std::endl;
}

int sudokuBoard::getIters(){ return _iterations;}

cell* sudokuBoard::getCell(int i, int j){ return _board[i][j];}

void sudokuBoard::clearBoard(){
    for(int i = 0; i < 9; i++){
        for(int j = 0; j < 9; j++){
            fillCell(i,j,0);
        }
    }
}

bool sudokuBoard::emptyBoard(){
    for(int i = 0; i < 9; i++){
        for(int j = 0; j < 9; j++){
            if(_board[i][j]->val != 0) return false;
        }
    }
    return true;
}
