
#include "SPFIARParser.h"

bool spParserIsInt(const char* str){
	if(str==NULL)
		return false;
	if(!isdigit(*str) && str[0]!='-') //checks the first char - digit or minus
				return false;
	str++;
	for ( ; *str != '\0'; str++ ){ //checks the rest of the chars if they are digits
		if(!isdigit(*str))
			return false;
	}
	return true;
}



SPCommand spParserPraseLine(char* str){
	SPCommand curr_command; //represents the command
	curr_command.validArg = false; //initialize valide.arg to be false
	if(str == NULL){ //if line is null - invalid line
		curr_command.cmd = SP_INVALID_LINE;
		return curr_command;
	}
	char *token;
	char line[SP_MAX_LINE_LENGTH+1]; //represents the line
	strcpy(line,str); //copy the given string
	line[strlen(str)] = '\0';
	token = strtok(line," \t\r\n"); //gets the first token seperated by the given delimiter
	curr_command.cmd = spParserIsCommand(token); //initializes the command
	token=strtok(NULL," \t\r\n"); //gets the second token
    if (curr_command.cmd != SP_ADD_DISC && token!=NULL){ // not add_disc and have more tokens - invalid command
    	curr_command.cmd = SP_INVALID_LINE;
    }
    else if (token == NULL) // no more tokens
    	return curr_command;
    else{
    	if (spParserIsInt(token)){ //add_disc checks if next token is integer
    		int arg = atoi(token);
            token= strtok(NULL," \t\r\n");
            if(token == NULL){ // no more tokens after the column number
            	curr_command.validArg = true;
            	curr_command.arg = arg;
            }
            else //there are more tokens after column - not valid
            	curr_command.cmd = SP_INVALID_LINE;
        }
    	else{ //token is not integer
    		token= strtok(NULL," \t\r\n");
    		if(token != NULL) // more tokens - invalid syntax
    			curr_command.cmd = SP_INVALID_LINE;
    	}
    }
    return curr_command;
}




SP_COMMAND spParserIsCommand(const char* str){
	SP_COMMAND cmd;
	if (!str){
			cmd =SP_INVALID_LINE;
			return cmd;
	}
	else if (!strcmp(str,"undo_move")) //5 if statements that check what the user's command
		cmd = SP_UNDO_MOVE;
	else if (!strcmp(str,"add_disc"))
		cmd = SP_ADD_DISC;
	else if (!strcmp(str,"suggest_move"))
		cmd = SP_SUGGEST_MOVE;
	else if (!strcmp(str,"quit"))
		cmd = SP_QUIT;
	else if (!strcmp(str,"restart_game"))
		cmd = SP_RESTART;
	else
		cmd =SP_INVALID_LINE; //if the command is not valid
	return cmd;
}


