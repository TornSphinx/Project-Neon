/*
  Author: Iftikhar Khan
  Class: {135,136}
  Instructor: S.Shankar
  Assignment: Project 2
  Goals: This program tries to play as a superior being in the game of Battleships and become victorious...As you
  can see, it terribly fails to do so....
*/

#include <cstdlib>
#include <iostream>
#include "bot.h"
#include "screen.h"

using namespace std;

int ROWS;
int COLS;
char replica[50][50];
bool isHit = false;
//TODO:Find an alternative for so many of these wretched globals...
int ahoyRow,ahoyColumn;
int iter =0;
//Function for initialisation procedure. This breaks the board into a chessboard.
void chessBoard(Screen &screen, int row, int col){
  for(int eachrow = 0; eachrow < row; ++eachrow){
    int eachcolumn;
    for((eachrow%2 == 0)?eachcolumn = 0:eachcolumn = 1; eachcolumn < col; eachcolumn+=2){
      screen.mark(eachrow,eachcolumn,'#',BLUE);
    }
  }
}
//TODO:upgrade this function to a switch in the future.
//Do I really need to explain what this function does? Does it even matter? Life sucks, anyway.
//Why did I put this before 'init', you ask? Well, does it make a difference? No
void postHit(Screen &screen,int row, int column, int hitRow, int hitColumn){
  //As long as a ship is hit but not sunk,
  //TODO: Didn't even need this condition... *Sigh*...
  //if same row but neighbouring column was hit
  if(replica[hitRow][hitColumn-1] == 'H' or replica[hitRow][hitColumn+1] == 'H'){
    //the ship is horizontal
    row = hitRow;
    //hit the previous column
    //PATCH:There is a much better way to arrange the arguments. a while statement, maybe?
    if (replica[hitRow][hitColumn-1] != 'H' and replica[hitRow][hitColumn-1]!= 'M'){
      do{
        column = hitColumn-1;
        hitColumn--;
      }while(screen.read(row,column) != '-' or screen.read(row,column) != '#');
    }
    else if(replica[hitRow][hitColumn+1] != 'H' and replica[hitRow][hitColumn+1] != 'M'){
      do{
        column = hitColumn+1;
        hitColumn++;
      }while(screen.read(row,column) != '-' or screen.read(row,column) != '#');
    }
  }
  //else if different row but same column, it was a vertical
  else if(replica[hitRow-1][hitColumn] == 'H' or replica[hitRow+1][hitColumn] == 'H'){
    column = hitColumn;
    if(replica[hitRow-1][hitColumn] != 'H' and replica[hitRow-1][hitColumn] != 'M'){
      do{
        row = hitRow -1;
        hitRow--;
      }while(screen.read(row,column) != '-' or screen.read(row,column) != '#');
    }
    else if(replica[hitRow+1][hitColumn] != 'H' and replica[hitRow+1][hitColumn] != 'M'){
      do{
        row = hitRow+1;
        hitRow++;
      }while(screen.read(row,column) != '-' or screen.read(row,column) != '#');
    }
  }
  //else, we hit one part of the ship.
  //so, just attack the next part
  else{
    int i =0;
    do{
      if (i ==0){
        row = hitRow+1;
        column = hitColumn;
      }
      else if(i == 1){
        row = hitRow-1;
        column = hitColumn;
      }
      else if(i == 2){
        row = hitRow;
        column = hitColumn+1;
      }
      else if(i == 3){
        row = hitRow;
        column = hitColumn-1;
      }
      ++i;
    }while(((replica[row][column] == 'H' or screen.read(row,column) != 'M') or screen.read(row,column) != 'S') && i <4);
  }
}

        
//TODO:Write another function for postSunk...I'm feeling sleepy... Let me take a nap...

    
/* Initialization procedure, called when the game starts:

   init (rows, cols, num, screen, log) 
 
   Arguments:
    rows, cols = the boards size
    num        = the number of ships 
    screen     = a screen to update your knowledge about the game
    log        = a cout-like output stream
*/
void init(int rows, int cols, int num, Screen &screen, ostream &log) 
{
  ROWS = rows;
  COLS = cols;
  log << "Start." << endl;
  chessBoard(screen, ROWS, COLS);
  log << "Chessboard Arrangement Successful" << endl;
}

bool onScreen(Screen &screen, int rows, int cols, char ch){
  for (int eachrow = 0; eachrow < rows; eachrow++){
    for(int eachcolumn = 0; eachcolumn < cols; eachcolumn++){
      if(screen.read(eachrow,eachcolumn) != ch){
        return true;
      }
    }
  }
  return false;
}

void randomChooser(Screen &screen, int rows, int cols, int dR, int dC){
  if (onScreen(screen, rows, cols, '#')){
    //if there are #'s present, randomly choose one of them
    do{
      dR = rand() % rows;
      dC = rand() % cols;
    }while(screen.read(dR,dC) != '#' or (replica[dR][dC] == 'M' or replica[dR][dC]=='H'));
  }else{
    //choose a sequential one
    do{
      dR = iter/cols;
      dC = iter % cols;
      iter += 2;
    }while(replica[dR][dC] == 'M' or replica[dR][dC] == 'H');
    }
}
/* The procedure handling each turn of the game:
 
   next_turn(sml, lrg, num, gun, screen, log)
 
   Arguments:
    sml, lrg = the sizes of the smallest and the largest ships that are currently alive
    num      = the number of ships that are currently alive
    gun      = a gun.
               Call gun.shoot(row, col) to shoot: 
                  Can be shot only once per turn. 
                  Returns MISS, HIT, HIT_N_SUNK, ALREADY_HIT, or ALREADY_SHOT.
    screen   = a screen to update your knowledge about the game
    log      = a cout-like output stream
*/
void next_turn(int sml, int lrg, int num, Gun &gun, Screen &screen, ostream &log)
{

  int r = rand()%ROWS;
  int c = rand()%COLS;
  //if there was a hit, follow it up
  if (isHit){
    postHit(screen,r,c,ahoyRow,ahoyColumn);
    
  }else{
    //choose a random from the chessboard pattern
    randomChooser(screen,ROWS,COLS,r,c);
  }
  log << "Smallest: " << sml << " Largest: " << lrg << ". ";
  log << "Shoot at " << r << " " << c << endl;

  result res = gun.shoot(r, c);
  // add result on the screen
  if (res == MISS){
    screen.mark(r, c, 'x', BLUE);
    replica[r][c] = 'M'; 
  }
  else if (res == HIT){
    screen.mark(r, c, '@', GREEN);
    replica[r][c] = 'H';
    isHit = true;
    ahoyRow = r;
    ahoyColumn = c;
    log << "Ships Ahoy! We have a hit!" << endl;
  } 
  else if (res == HIT_N_SUNK){
    screen.mark(r, c, 'S', RED);
    isHit = false;
    replica[r][c] = 'S';
    //TODO:Mark surrounding areas as miss in array
    //postSunk();//Mark isHit = false
  }
}
