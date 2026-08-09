#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "cmd_execution.h"
#include "shell_ctl.h"


int parse_input(char* input)
{
  struct Cmd* cmd = malloc (sizeof (struct Cmd));
  cmd->cmd_size = 0; 
  cmd->flag_count = 0;
  cmd->parameter_count = 0;
  
  int cmd_length = get_char_count (input);
  

  for (int i = 0; i < cmd_length; i++)
  {
    if (input [i] == ' ') continue; //skip spaces
    else if (input[i] == '-' && cmd_length != 0)  // flag
    {
      i++;
      while (input [i] != ' ' && i < cmd_length)
      {
        cmd->flag_count++;
        
        if (char* temp = realloc (cmd->flags, cmd->flag_count * sizeof (char)))
        { 
          cmd->flags = temp;
        }
        else 
        {
          printf ("There was an error allocating memory to cmd->flags.\n");
          return -1;
        }

        cmd->flags[cmd->flag_count - 1] = input[i];
        printf ("\n=========\nFlag_count: %d\ncmd->flags[cmd->flag_count - 1]: %c\ninput[i]: %c\n=========\n", cmd->flag_count, cmd->flags[cmd->flag_count - 1], input[i]);
        i++;
      }
      
      cmd->flag_count++;
      if (char* temp = realloc (cmd->flags, cmd->flag_count * sizeof (char)))
      { 
        cmd->flags = temp;
      }
      else 
      {
        printf ("There was an error allocating memory to cmd->flags.\n");
        return -1;
      }
      cmd->flags[cmd->flag_count - 1] = '\n'; // otherwise, the get_char() won't find the char* length

    }

/*    else if (cmd->cmd_size != 0) // parameter; if already read the command name 
    {
      cmd->parameter_count++;

      printf ("we're getting parameters\n");

      if (char** temp = realloc (cmd->parameter, cmd->parameter_count * sizeof(char*)))
      {
        cmd->parameter = temp;
      }
      else 
      {
        printf ("There was an error allocating memory to cmd->parameter\n");
        return -1;
      }

      while(input[i] != ' ' && i < cmd_length)
      {
        parameter_size++;
        if (char* temp = realloc (*(cmd->parameter), cmd->parameter_count * sizeof(char)))
        {
          *(cmd->parameter) = temp;
        }
        *(cmd->parameter)[cmd->parameter_count - 1] = input[i];
        i++;
      }
    }*/
    else  // the command name 
    {
      while(input[i] != ' ' && i < cmd_length)
      {
        cmd->cmd_size++;
        if (char* temp = realloc (cmd->command, cmd->cmd_size * sizeof (char)))
        { 
          cmd->command= temp;
        }
        else
        {
          printf ("There was an error allocating memory to cmd->command\n");
          return -1;
        }
        cmd->command[cmd->cmd_size - 1] = input[i];
        printf ("\n=========\ncmd_count: %d\ncmd->cmd[cmd->cmd_size - 1]: %c\ninput[i]: %c\n=========\n", cmd->cmd_size, cmd->command[cmd->cmd_size - 1], input[i]);
        i++;
      }
        cmd->cmd_size++;
        if (char* temp = realloc (cmd->command, cmd->cmd_size * sizeof (char)))
        { 
          cmd->command= temp;
        }
        else
        {
          printf ("There was an error allocating memory to cmd->command\n");
          return -1;
        }
        cmd->command[cmd->cmd_size - 1] = '\n';

    }
  }
  print_command(cmd);

}
void print_command(struct Cmd* cmd)
{
  printf ("\n=====================\nsafely inside printf_command\n");
  
  printf ("Command: ");
  for (int i = 0; i < get_char_count (cmd->command); i++)
  {
    printf ("%c", cmd->command[i]);
  }
  printf ("\n");

  if (cmd->flag_count > 0)
  {
    printf ("Flags(%d): ", get_char_count(cmd->flags));
    
    for (int i = 0; i < get_char_count (cmd->flags); i++)
    {
      printf ("%c", cmd->flags[i]);
    }
    printf ("\n");
  }
/*  for (int i = 0; i < cmd->parameter_count; i++)
  {
    for (int j = 0; j < get_char_count (cmd->parameter[i]); j++)
      printf ("Parameter %d: %c\n", cmd->parameter[i][j]);
  }*/
  printf ("\nQuiting print_command\n======================\n");
}
