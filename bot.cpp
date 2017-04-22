/* VERSION # 2.1
   NOTE: This is a ground up written version for bot.cpp after the previous version failed due to Timeout
   Author: Iftikhar Khan
   Class: {135,136}
   Instructor: S.Shankar
   Assignment: Project 2
   Description: This is a Level 2 AI. That is, it only plays the game using simple observations; it is only one step up from the provided AI. The only advantages it has over the provided program are:
                1) It marks surrounding areas after a ship is sunk. So, it doesn't need to hit those areas
                2) After getting a hit, it tries to work in the neighbouring areas to get another hit.
                3) It implements a 'Chessboard Strategy'. The chessboard strategy divides all the squares on the board into black and white squares.
                   Ideally, if the bot were to hit all the Black squares, It will hit all the ships at least once.
                   However, if the smallest ship alive is of size 1, the strategy becomes less effective.
   Observations: i) Both the postHit and postHit functions contain repetitive functions. In both cases, they perform almost similar functions in NORTH,SOUTH,EAST,WEST directions.
                ii) The Global char array termed replica has very few uses and provides very little advantages. However, its full potential can only be unlocked using Statistical Analysis to predict the position of the ships.
               iii) In the postSink function, the array operations on REPLICA has been thawed. It appears to create the same problem for postHit. Since, it is possible that the hit or sunk ship could be at the boundaries of the
                    board, the addressing of the array's out of bounds results in a Segmentation Fault and dumped core. So, the code has been commented off.
  Release Notes: a) PostSink now works properly. However, for the future, it may be assisted using a helper function to stop the repetiteveness.
                 b) We now know that the ships are more likely to be around the centre of the board than the borders. This finding may be implemented in Version#3 of the project. The project will be added to on GitHub for future revisions.
*/

#include <cstdlib>
#include <iostream>
//For linking with the rest of the program
#include "bot.h"
#include "screen.h"
using namespace std;

/* -------------------------- */
//Globals:
//TODO:Take these out, if we don't need them. (Update: We actually do)
int ROWS;
int COLS;
//The Global 2D array to store the ship's hits.
//This array will be partially filled, so we nee the Global ROWS and COLS as dimensions
char replica[50][50];
//This boolean is the only way to determine if there was a hit the previous round
bool isHit;
//These are the only ways to keep the hit coordinates in track
int hitRow,hitCol;

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
/*-------------------------------*/
/*This function searches for a char in the replica array. If it finds it, it changes the param to the result, if not, it changes them to -1,-1*/
void search(char (&replica)[50][50], int x, int y, char item, int &foundx, int &foundy){
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
//TODO:Write postHit, which needs to be a function based on a switch statement.
/* This function is called after a ship has been hit. It tries to hit the neighbouring areas in search of the remaining parts */
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
  if(screen.read(hitRow,hitCol-1) == '-'){
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
/*This function is meant to compress the functions of postSink with a modifying enumerator */
void postSinkHelper(int row, int col, Screen &screen, enum proto {'N','S','W','E'};){
  int rowMod, colMod;
  //TODO: Find the proper values for the modifying values that will return a proper value for the directions

  switch (proto) {
    case 'W':
      //provide proper values for modifiers
      break;
    case 'N':
      //Provide proper values for modifiers
      break;
    case 'S':
      //Provide proper values for the modifiers
      break;
    case 'E':
      //Provide proper values for the modifiers;
      break;
  }
  screen.mark(row+rowMod,col-colMod,'x',BLUE);
  do{
    screen.mark(rowMod+colMod, colMod+rowMod, 'x', BLUE);
    screen.mark(rowMod-colMod, colMod-rowMod,'x',BLUE);
    row += rowMod;
    col += colMod;
  }while(screen.read(row,col) == '@');
  screen.mark(row,col,'x',BLUE);
  return;
}

/* This function takes row and col and changes the surrounding areas of the sunken ship and marks them as Miss in both the array and the screen. The reason the screen is marked is to support the logic of the postHit function */
void postSink(int &row, int &col, Screen &screen){
/*NOTE:We must make key observations before proceeding to develop an algorithm for this particular function.
  In regular games, it is possible that the final point of the hitting a ship;that is, the co-ordinate that results in the ship being
  sunk can be in the middle of the ship. This hugely complicates things.
  However, by merely looking at the postHit function and the arrangement of the ships in the board, we can implement the if-else branches
  that would have had to be avoided. So, the algorithm is quite simple.
*/
  //If the hits are on the left,
  int tempCol = col;
  int tempRow = row;
  if(screen.read(row,col-1) == '@'){
    //Engage Westward protocol
    //First, we must mark the immediate east side as MISS
    screen.mark(row,col+1,'x',BLUE);
    do{
      //Since, this is a do-while loop, this will mark the actual 'S' surroundings and continue from there on.
      screen.mark(tempRow+1,tempCol,'x',BLUE);
      screen.mark(tempRow-1,tempCol, 'x', BLUE);
      //It will continue going West
      --tempCol;
    }while(screen.read(tempRow,tempCol) == '@');
    //After the loop ends,the coordinates of tempCol will be the exact coordinates we need to mark as our last Miss
    screen.mark(tempRow,tempCol,'x',BLUE);
    return;
  }
  //If the hits are on the right,
  if(screen.read(row,col+1) == '@'){
    //Engage Eastward protocol
    //First, mark the West
    screen.mark(row, col-1, 'x',BLUE);
    do{
      screen.mark(tempRow+1,tempCol,'x',BLUE);
      screen.mark(tempRow-1,tempCol, 'x', BLUE);
      ++tempCol;
    }while(screen.read(tempRow,tempCol) == '@');
    screen.mark(tempRow,tempCol,'x', BLUE);
    return;
  }
  //If the hits are in the Bottom,
  if(screen.read(row-1,col) == '@'){
    //Engage South
    //Mark immediate North
    screen.mark(row+1, col,'x', BLUE);
    do{
      screen.mark(tempRow, tempCol+1, 'x', BLUE);
      screen.mark(tempRow, tempCol-1, 'x', BLUE);
      --tempRow;
    }while(screen.read(tempRow,tempCol) == '@');
    screen.mark(tempRow,tempCol,'x', BLUE);
    return;
  }
  //If the hits are towards the North, which is most unlikely
  //TODO:Verify if this is actually necessary...i.e. Due to the nature of the rest of the program, if there is a case where the hits are North of the Sink coordinates
  if (screen.read(row+1,col) == '@'){
    //Engage North
    //Mark South as miss
    screen.mark(row-1, col, 'x', BLUE);
    do{
      screen.mark(tempRow, tempCol+1, 'x', BLUE);
      screen.mark(tempRow, tempCol-1, 'x', BLUE);
      ++tempRow;
    }while(screen.read(tempRow,tempCol) == '@');
    screen.mark(tempRow,tempCol,'x',BLUE);
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
  search(replica, ROWS, COLS, '#', indX, indY);
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
    search(replica, ROWS, COLS, 'U', indX, indY);
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
    replica[row][col] = 'M';
  }
  else if(res == HIT){
    screen.mark(row, col, '@', GREEN);
    //update the replica
    replica[row][col] = 'H';
    hitRow = row;
    hitCol = col;
    //update isHit
    isHit = true;
  }
  else if(res == HIT_N_SUNK){
    screen.mark(row, col, 'S', RED);
    //update replica and isHit
    replica[row][col] = 'S';
    isHit = false;
    //Call the postSink function and pass the rows and cols of the sunken ship as arguments
    postSink(row, col,screen);
  }
}
