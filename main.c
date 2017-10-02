#include "SPMainAux.h"

int main(){
	int maxDepth = SPFiarInit(); //get level from the user
	SPFiarGame* game = spFiarGameCreate(HISTORY_SIZE); //create new game
	if(!game) // allocation error
		Fail(game,false); //print error, no need to destroy, exit
	bool check = false , toprint = true; // check stands for gameover check, toprint flag tells to print board or not
	while(true){ //game started
		SPCommand command = NewCommand(game,toprint); //get command from user
		if(command.cmd == SP_QUIT) //if the user quits the game
			Quit(game);
		else if(command.cmd == SP_RESTART){ //user wants to restart
			check = false; toprint = true;
			maxDepth = Restart(game); //restarting
			game = spFiarGameCreate(HISTORY_SIZE); //create new game
			if(!game) // allocation error
				Fail(game,false);//print error, no need to destroy, exit
			continue;
		}
		else if(command.cmd == SP_UNDO_MOVE){ //user wants to undo his last move
			toprint = UndoMove(game); //if succeeded toprint = true - need to print board
			if(toprint) check=false; // undo succeeded- the game is not over
			continue; //still user's turn
		}
		else{
			if(!check){ //game is not over
				toprint = MakeMove(game,maxDepth,command); //making move
				if(command.cmd == SP_SUGGEST_MOVE)	toprint = false; //if command = suggest_move - no need to print board
				if(!toprint)
					continue; //still user's turn
			}
			else{
				printf("Error: the game is over\n");  continue; //game over
			}
		}
		check = WinnerCheckPrint(game); //checks winner
		if(check){ // if there is a winner no need to print board
			 toprint = false;
			 continue; // user's turn
		}
		else // if user did not win, now it is computer move
			ComputerMove(game,maxDepth);
		check = WinnerCheckPrint(game); // check if computer won
		if(check) toprint = false; // if there is a winner no need to print board
	}
}
