/* VERSION # 0.1.2
   NOTE: This is a ground up written version for bot.cpp after the previous version failed due to Timeout
   Author: Iftikhar Khan
   Assignment: Project 2
   Description: This is a Level 2 AI. That is, it only plays the game using simple observations; it is only one step up from the provided AI. The only advantages it has over the provided program are:
                1) It marks surrounding areas after a ship is sunk. So, it doesn't need to hit those areas
                2) After getting a hit, it tries to work in the neighbouring areas to get another hit.
                3) It implements a 'Chessboard Strategy'. The chessboard strategy divides all the squares on the board into black and white squares.
                   Ideally, if the bot were to hit all the Black squares, It will hit all the ships at least once.
                   However, if the smallest ship alive is of size 1, the strategy becomes less effective.
   Observations: i) Both the postHit and postHit functions contain repetitive functions. In both cases, they perform almost similar functions in NORTH,SOUTH,EAST,WEST directions.
                ii) The Global char array termed replica has very few uses and provides very little advantages. However, its full potential can only be unlocked using Statistical Analysis to predict the position of the ship.
               iii) Since, marking the coordinates sometimes yields out of bound errors, I avoided using it during postSink and postHit functions. Although, useful this brings another nasty bug; the search() function returns values
                    that it has already shot. UPDATE: fixed
   Release Notes: a) PostSink now works properly. However, for the future, it may be assisted using a helper function to overcome the repetiteveness
                  b) We now know that the ships are more likely to be around the centre of the board than the borders. This finding may be implemented in Version#1
                  c) Version#0.2 should include a fix for the dreaded UNWITTING bug.TODO:Needs fixing.
*/

#include <cstdlib>
#include <iostream>
//For linking with the rest of the program
#include "bot.h"
#include "screen.h"
using namespace std;

/* -------------------------- */
//Globals:
int ROWS;
int COLS;
//The Global 2D array to store the ship's hits.
//This array will be partially filled, so we nee the Global ROWS and COLS as dimensions
char replica[40][40];
//This boolean is the only way to determine if there was a hit the previous round
bool isHit;
//These are the only ways to keep the hit coordinates in track
int hitRow,hitCol;
int iter;
/* -------------------------*/
/*Implements a chessboard pattern in order to hunt ships efficiently.
  Should be used when board is initialised.
  Works best when sml > 1;
  Input:
  screen = to visualise the pattern
  row, col = dimensions
*/
void chessBoard(int row, int col){
  //in each row,
  for(int eachRow = 0; eachRow < row; ++eachRow){
    //renew the column
    int eachColumn;
    //init: the logic decides which column to mark with # first; then, it iterates alternatingly
    for((eachRow %2 == 0)?eachColumn = 0:eachColumn =1; eachColumn < col; eachColumn +=2){
      //UPDATE:the chessboard strategy is only implemented in the array and is not visualised on screen.
      replica[eachRow][eachColumn] = '#';
    }
  }
}
/*-------------------------------*/
/*This function fills the array with the character U. This helps with searches for unexplored spots later on */
void fillArray(){
  for(int i = 0; i < ROWS; ++i){
    for(int j = 0; j < COLS; ++j){
      replica[i][j] = 'U';
    }
  }
}

/* -----------------------------*/
/* This function returns true if the co-ordinates are withing the scope of the array */
bool inScope(int &row, int &col){
  if (row <= ROWS && row >= 0 && col <= COLS && col >=0){
    return true;
  }
  return false;
}

/* This function sets the provided value at the provided co-ordinates */
void setArray(int row, int col, char value){
  if(inScope(row,col)){
    replica[row][col] = value;
  }
}

//Read the value from the given coordinates of the array
char readArray(int row, int col){
  if(inScope(row,col)){
    return replica[row][col];
  }
  return ' ';
}
/*-------------------------------*/

/*This function searches for a char in the replica array. If it finds it, it changes the param to the result, if not, it changes them to -1,-1*/
void search(int x, int y, char item, int &foundx, int &foundy, Screen &screen){
  bool found = false;
  for(int i = 0; i < x; ++i){
    for(int j = 0; j < y; ++j){
      if (replica[i][j] == item){
        foundx = i;
        foundy = j;
        found = true;
        break;
      }
    }
  }
  if (!found){
    foundx = -1;
    foundy = -1;
  }
}

/*-------------------------*/

//TODO:Upon further analysis, there is a severe flaw within postHit.
/*BUG: When executed, even if the ship's been hit several times, postHit returns coordinates, the ships possibly cannot be in. That is,
       postHit is not an intelligent hunter after hunting mode has been activated (labelled the 'UNWITTING' Bug). The function has to be written from the ground up in order to solve the issue.
  This function is called after a ship has been hit. It tries to hit the neighbouring areas in search of the remaining parts */
void postHit(int &row, int &col, Screen &screen){
  //First, check North
  //To check the sides, we use screen.read since, the index maybe out of bounds.
  if(screen.read(hitRow, hitCol+1) == '-'){
    //change the rows and cols to the north
    row = hitRow;
    col = hitCol+1;
    return;
  }
  //else if it was previously hit, then there must have been an attempt there, and the last hit could be the middle of the ship
  //so, change the north, till it's unexplored water
  else if(screen.read(hitRow,hitCol+1) == '@'){
    //while the coordinates are not unexplored,
    while(screen.read(hitRow,hitCol) != '-'){
      ++hitCol;
    }
    //after getting the coordinates, set them to the rows and cols
    row = hitRow;
    col = hitCol;
    return;
  }
  //Repeat the above strategy for South, East and West
  //Check South and apply strategy
  if(screen.read(hitRow,hitCol-1) == '-' ){
    row = hitRow;
    col = hitCol -1;
    return;
  }
  else if (screen.read(hitRow, hitCol-1) == '@'){
    while(screen.read(hitRow, hitCol)!= '-'){
      --hitCol;
    }
    row = hitRow;
    col = hitCol;
    return;
  }
  //Check West
  if(screen.read(hitRow-1,hitCol) == '-'){
    row = hitRow-1;
    col = hitCol;
    return;
  }
  else if(screen.read(hitRow-1,hitCol) == '@'){
    while(screen.read(hitRow,hitCol)!= '-'){
      --hitRow;
    }
    row = hitRow;
    col = hitCol;
    return;
  }
  //Check East
  if (screen.read(hitRow+1,hitCol) == '-'){
    row = hitRow+1;
    col = hitCol;
    return;
  }
  else if (screen.read(hitRow+1,hitCol) == '@'){
    while(screen.read(hitRow,hitCol)!= '-'){
      ++hitRow;
    }
    row = hitRow;
    col = hitCol;
    return;
  }
}

/*-------------------------------*/
/* This function takes row and col and changes the surrounding areas of the sunken ship and marks them as Miss in both the array and the screen. The reason the screen is marked is to support the logic of the postHit function */
/*NOTE:We must make key observations before proceeding to develop an algorithm for this particular function.
  In regular games, it is possible that the final point of the hitting a ship;that is, the co-ordinate that results in the ship being
  sunk can be in the middle of the ship. This hugely complicates things.
  However, by merely looking at the postHit function and the arrangement of the ships in the board, we can implement the if-else branches
  that would have had to be avoided. So, the algorithm is quite simple.
*/
void postSink(int &row, int &col, Screen &screen){
  //If the hits are on the left,
  int tempCol = col;
  int tempRow = row;
  if(screen.read(row,col-1) == '@'){
    //Engage Westward protocol
    //First, we must mark the immediate east side as MISS
    screen.mark(row,col+1,'x',BLUE);
    setArray(row,col+1,'M');
    do{
      //Since, this is a do-while loop, this will mark the actual 'S' surroundings and continue from there on.
      screen.mark(tempRow+1,tempCol,'x',BLUE);
      screen.mark(tempRow-1,tempCol, 'x', BLUE);
      setArray(tempRow+1,tempCol,'M');
      setArray(tempRow-1,tempCol,'M');
      //It will continue going West
      --tempCol;
    }while(screen.read(tempRow,tempCol) == '@');
    //After the loop ends,the coordinates of tempCol will be the exact coordinates we need to mark as our last Miss
    screen.mark(tempRow,tempCol,'x',BLUE);
    setArray(tempRow,tempCol,'M');
    return;
  }
  //If the hits are on the right,
  if(screen.read(row,col+1) == '@'){
    //Engage Eastward protocol
    //First, mark the West
    screen.mark(row, col-1, 'x',BLUE);
    setArray(tempRow,tempCol-1,'M');
    do{
      screen.mark(tempRow+1,tempCol,'x',BLUE);
      screen.mark(tempRow-1,tempCol, 'x', BLUE);
      setArray(tempRow+1,tempCol,'M');
      setArray(tempRow-1,tempCol,'M');
      ++tempCol;
    }while(screen.read(tempRow,tempCol) == '@');
    screen.mark(tempRow,tempCol,'x', BLUE);
    setArray(tempRow,tempCol,'M');
    return;
  }
  //If the hits are in the Bottom,
  if(screen.read(row-1,col) == '@'){
    //Engage South
    //Mark immediate North
    screen.mark(row+1, col,'x', BLUE);
    setArray(row+1,col,'M');
    do{
      screen.mark(tempRow, tempCol+1, 'x', BLUE);
      screen.mark(tempRow, tempCol-1, 'x', BLUE);
      setArray(tempRow,tempCol+1,'M');
      setArray(tempRow,tempCol-1,'M');
      --tempRow;
    }while(screen.read(tempRow,tempCol) == '@');
    screen.mark(tempRow,tempCol,'x', BLUE);
    setArray(tempRow,tempCol,'M');
    return;
  }
  //If the hits are towards the North,
  if (screen.read(row+1,col) == '@'){
    //Engage North
    //Mark South as miss
    screen.mark(row-1, col, 'x', BLUE);
    setArray(row-1,col,'M');
    do{
      screen.mark(tempRow, tempCol+1, 'x', BLUE);
      screen.mark(tempRow, tempCol-1, 'x', BLUE);
      setArray(tempRow,tempCol+1,'M');
      setArray(tempRow,tempCol-1,'M');
      ++tempRow;
    }while(screen.read(tempRow,tempCol) == '@');
    screen.mark(tempRow,tempCol,'x',BLUE);
    setArray(tempRow,tempCol,'M');
    return;
  }
}

/*----------------------------*/
/* This function chooses the coordinates of where to shoot */

void gunner(int &row, int &col, ostream &log, Screen &screen){
  //First, check if there was a hit the previous turn
  if (isHit == true){
    //call postHit
    log << "There was a hit last round"<< endl;
    log << "Engaging HIT protocol" << endl;
    postHit(row, col,screen);
    //Immediately stop the execution of this function; because the latter statements may still be true;
    return;
  }
  //else, ask to choose a random(UPDATE: Not necessarily) mark from the chessboard pattern
  //we can only do so, if there is a chessboard strategy left. i.e. there is still a # left.
  //if there is a # left, then check for it.
  int indX, indY;
  search(ROWS, COLS, '#', indX, indY, screen);
  //TODO:if ind is less than 0, find a sequential row and col to shoot
  //To simplify, indX will never be
  if (!(indX < 0)){
    row = indX;
    col = indY;
  }
  //If all the chessboard has been used and the ships haven't been sunk, (this only happens when there is still a ship of size 1 left on the board), do a sequential hit on the next available spot;
  else{
    //TODO:search through the array for an unexplored spot.
    //To accomplish this, we have to write a function to fill the array with an arbitrary char. Otherwise, the search() function will have trouble.
    search(replica, ROWS, COLS, 'U', indX, indY, screen);
    row = indX;
    col = indY;
  }
}



/*--------------------------------*/
/* Called once at the beginning
   input:
   rows, cols = size of the board
   num = number of ships on the board
   screen = the screen that allows me to see the blasted output
   log = cout, but better
*/
void init(int rows, int cols, int num, Screen &screen, ostream &log){
  //initialise the rows and cols
  ROWS = rows;
  COLS = cols;
  log << "Start." << endl;
  fillArray();
  log << "Array Updated with Values" << endl;
  //Call the chessboard implementer
  chessBoard(rows, cols);
  log << "Chessboard Strategy Implemented" << endl;
  isHit = false;
  log << "isHit is set to default value" << endl;
}
/* ------------------------*/

/* This function handles each turn of the game
   Input:
   sml, lrg = the sizes of the smallest and the largest ships alive
   num = the number of ships alive
   gun = a gun
*/
void next_turn (int sml, int lrg, int num, Gun &gun, Screen &screen, ostream &log){
  //TODO:find a row and column to shoot
  int row;
  int col;
  //Call the gunner
  gunner(row,col,log,screen);
  log << "Smallest: " << sml << " Largest: " << lrg << ". ";
  log << "Shoot at " << row << " "<< col<< endl;
  result res = gun.shoot(row,col);
  //Display the results on the screen
  if(res == MISS){
    screen.mark(row,col, 'x', BLUE);
    //update the replica
    setArray(row,col,'M');
  }
  else if(res == HIT){
    screen.mark(row, col, '@', GREEN);
    //update the replica
    replica[row][col] = 'H';
    setArray(row,col,'H');
    hitRow = row;
    hitCol = col;
    //update isHit
    isHit = true;
    iter = 0;
  }
  else if(res == HIT_N_SUNK){
    screen.mark(row, col, 'S', RED);
    //update replica and isHit
    setArray(row,col,'S');
    isHit = false;
    //Call the postSink function and pass the rows and cols of the sunken ship as arguments
    postSink(row, col,screen);
  }
}
