#include <stdio.h>

#include "cmd_execution.h"

typedef struct 
{
  char* command;
  char* flags;
  char** options; // there can be more than one options
} Cmd;

typedef struct 
{
  
} read_cmd;


char** parse_input(char* input)
{
  int cmd_size = 0; 
  int cmd_idx = 0;
  
  int flag_count = 0;
  
  Cmd cmd;

  for (int i = 0; i < get_char_count (input); i++)
  {
    if (input [i] == ' ') continue; //skip spaces
                                    //
    else if (input[i] == '-' && cmd_size != 0)  // flag
    {
      i++;
      while (input [i] != ' ' && input [i] != '\n')
      {
        flag_count++;
        cmd.flags = malloc (flag_count * sizeof (char));
        cmd.flags[flag_count - 1] == input[i];
        i++;
      }
    }

    else if (

  }
}
