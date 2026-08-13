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
                                    //
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
      cmd->flags[cmd->flag_count - 1] = '\0'; // otherwise, the get_char() won't find the char* length

    }

    else if (cmd->cmd_size != 0) // parameter; if already read the command name 
    {
      int parameter_size = 0;
      cmd->parameter_count++;

      if (char** temp = realloc (cmd->parameter, cmd->parameter_count * sizeof(char*)))
      { 
        cmd->parameter = temp;
      }
      else 
      {
        printf ("There was an error allocating memory to cmd->parameter\n");
        return -1;
      }

      parameter_size = 0;

      while (input[i] != ' ' && input[i] != '\n')
      {
        parameter_size++;
        if (char* temp = realloc (*(cmd->parameter + (cmd->parameter_count - 1)), parameter_size * sizeof(char)))
        {
          *(cmd->parameter + (cmd->parameter_count - 1)) = temp;
        }
        else 
        {
          printf ("There was an error allocating memory to *(cmd->parameter)\n");
          return -1;
        }

        printf ("input[i]: %c\nparameter_size -1 = %d\n", input[i], parameter_size -1);

        (cmd->parameter)[cmd->parameter_count - 1][parameter_size - 1] = input[i]; 

        printf ("cmd->parameter[cmd->parameter_count - 1][parameter_size-1] = %c\n", cmd->parameter[cmd->parameter_count - 1][parameter_size-1]);
        i++;
      }
      parameter_size++;
      (cmd->parameter)[cmd->parameter_count - 1][parameter_size - 1] = '\0'; 
        
    }
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
        cmd->command[cmd->cmd_size - 1] = '\0';

    }
  }
  return 0;
}

int exec_cmd ()
{
  pid_t pid = fork();

  if (pid == 0) // child process
  {
    
  }
  else if (pid == -1)
  {
    //error
  } 
  else 
  {
    wait_pid(pid);
  }
}
