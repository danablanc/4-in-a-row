#ifndef SPMAINAUX_H_
#define SPMAINAUX_H_

#include "SPFIARGame.h"
#include "SPMiniMax.h"
#include "SPFIARParser.h"
#include <assert.h>

#define HISTORY_SIZE 20 //size of the array list


/**** The use of assertions checks in sub function in order to check null pointers is approved by Moab ****/


/**
 * Initializes new game. Get's the level of the game from the user
 * Prints message according to the data the user entered.
 *
 * @return
 * -1  - if the user quits the game
 * maxDepth - number between 1 and 7 - otherwise
 */
int SPFiarInit();

/**
 * The function prints the game board and gets the command from the user
 * The function asks from the user for a command and gives a message if the command is not valid.
 *
 * @param game - the src game
 * @param check - boolean that is set to true if the game is not over
 *
 * @return
 * command - the command of the user
 * assertion error if game == null
 */
SPCommand NewCommand(SPFiarGame *game, bool check);

/**
 * The function gets a command and follows it - the function takes care of the command add disc and suggest move
 * The function calls for the relevant functions and prints messages as needed
 *
 * @param game - the src game
 * @param maxDepth - the game's level
 * @param command - the user command's
 *
 * @return
 * true - if the command was successfully done
 * false - otherwise (in case of some error)
 * assertion error if game == null
 */
bool MakeMove(SPFiarGame *game, int maxDepth, SPCommand command);
/**
 * The function makes a move on the game board according to the user's command
 * The function prints suitable message if an error occurred.
 *
 * @param game - the src game
 * @param col - the column where the user's command
 *
 * @return
 * true - if the command was successfully done
 * false - otherwise (in case of some error)
 * assertion error if game == null
 */
bool AddDisc(SPFiarGame *game,int col);

/**
 * The function undo the previous two moves
 * The function prints suitable message if an error occurred
 * Also, if the undo was successful  - prints which disc was moved
 *
 * @param game - the src game
 *
 * @return
 * true - if the command was successfully done
 * false - otherwise (in case of some error)
 * assertion error if game == null
 */
bool UndoMove(SPFiarGame *game);

/**
 * The function checks if someone won the game or if there is a tie and the game is over
 * The function prints a message according to the game situation
 *
 * @param game - the src game
 *
 * @return
 * true - if someone won or the game is over and there is a tie
 * false - otherwise (in case of some error)
 * assertion error if game == null
 */
bool WinnerCheckPrint(SPFiarGame *game);

/**
 * The function makes the computers move according to the minimax algorithm
 * The function prints error message if the algorithm didn't work,
 * otherwise - it prints the move that the computer made.
 *
 * @param game - the src game
 * @param maxDepth- the level of the game that the user chose
 *
 * @return
 * true - if the computer made a move
 * false - otherwise (in case of some error)
 * assertion error if game == null
 */
bool ComputerMove(SPFiarGame *game, int maxDepth);

/**
 * The function destroys the game and quits the window - prints a suitable message
 *
 * @param game - the src game
 * @return nothing - the function is void
 * assertion error if game == null
 */
void Quit(SPFiarGame *game);

/**
 * The function destroys the game and starts a new one - prints a suitable message
 *
 * @param game - the src game
 * @return the level of the game
 * assertion error if game == null
 */
int Restart(SPFiarGame *game);

/**
 * The function destroys the game and quits the window - prints a malloc faild error
 *
 * @param game - the src game , boolean arg destroy
 * @return nothing - the function is void
 * assertion error if game == null
 */
void Fail(SPFiarGame *game,bool destroy);

#endif
