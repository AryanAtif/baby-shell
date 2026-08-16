#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

#include "cmd_execution.h"
#include "shell_ctl.h"


char** parse_input(char* input)
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
          return NULL;
        }

        cmd->flags[cmd->flag_count - 1] = input[i];
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
        return NULL;
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
        return NULL;
      }

      parameter_size = 0;

      while (input[i] != ' ' && input[i] != '\n')
      {
        parameter_size++;
        if (char* temp = realloc (*(cmd->parameter + (cmd->parameter_count - 1)), parameter_size * sizeof(char)))
        {
          *(cmd->parameter + (cmd->parameter_count - 1)) = temp;
        } else 
        {
          printf ("There was an error allocating memory to *(cmd->parameter)\n");
          return NULL;
        }

        (cmd->parameter)[cmd->parameter_count - 1][parameter_size - 1] = input[i]; 
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
          return NULL; 
        }
        cmd->command[cmd->cmd_size - 1] = input[i];
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
          return NULL;
        }
        cmd->command[cmd->cmd_size - 1] = '\0';
    }

  }
  return merge_cmd (cmd);
}

int exec_cmd (char** cmd)
{
  pid_t w_pid;
  pid_t child_pid = fork();
  int status;
  
  if (child_pid == 0) // child process
  {
    if (strcmp(cmd[0], "cd") == 0)
    { 
      char* dir = NULL;
      if (cmd[1][0] == '~')
      {
        dir = concat(dir, cmd[1]);
      } 
      else
      {
        dir = cmd[1];
      }
      printf ("cd: %d\n", chdir(dir));
      perror(NULL);
    }
    else if (strcmp(cmd[0], "exit") == 0 || strcmp(cmd[0], "q") == 0 || strcmp(cmd[0], "Q") == 0)
    {
      return 0;
    }
    else 
    {
      execvp (cmd[0], cmd);
    }
  }
  else if (child_pid == -1)
  {
    //error
  } 
  else 
  {
    do
    {
      w_pid = waitpid(child_pid, &status,  WUNTRACED);
    }
    while (!(WIFEXITED(status) || WIFSIGNALED(status)));
  }
}

char** merge_cmd (struct Cmd* cmd)
{
  int argc = 0;
  char** new_arr = malloc(sizeof(char*));
  *new_arr = malloc(sizeof(char) * cmd->cmd_size);

  if (cmd->cmd_size > 0)
  {
    for (int i = 0; i < cmd->cmd_size; i++)
    {
      new_arr[argc][i] = cmd->command[i];
    }
  }
  else return NULL;

  // copy the flags into new_arr
  if (cmd->flag_count > 0)
  {
    argc++;

    new_arr = realloc (new_arr, (argc + 1) * sizeof (char*));
    new_arr[argc] = realloc (new_arr[argc], sizeof (char));
    new_arr[argc][0] = '-';
  }
  for (int i = 0; i < cmd->flag_count; i++)
  {
    new_arr[argc] = realloc (new_arr[argc], (i + 1) * sizeof (char));
    new_arr[argc][i+1] = cmd->flags[i];
  }

  // copy the parameters into new_arr
  for (int i = 0; i < cmd->parameter_count; i++)
  {
    argc++;
    new_arr = realloc (new_arr, (argc + 1) * sizeof (char*));

    for (int j = 0; cmd->parameter[i][j] != '\0'; j++)
    {
      new_arr[argc] = realloc (new_arr[argc], (j + 1) * sizeof (char));
      new_arr[argc][j] = cmd->parameter[i][j];
    }
  }

  argc++;
  new_arr = realloc (new_arr, (argc + 1) * sizeof (char*));
  new_arr[argc] = realloc (new_arr[argc], sizeof (char));
  
  new_arr[argc] = (char*) 0; 

 return new_arr;
}
void print_str (char* str, int length)
{
  for (int i =0; i < length; i++)
    printf ("%c", str[i]);
  printf ("\n");
}

char* concat (char* dst, char* src)
{
  char* home = getenv ("HOME");
  int length_home = strlen(home);
  dst = realloc (dst, (length_home) * sizeof(char));
  memcpy(dst, home, length_home);

  for (int i = 1; i <= strlen (src); i++)
  {
    if (char* temp = realloc (dst, (length_home + i) * sizeof(char))) dst = temp;
    else perror(NULL);

    dst[length_home + i - 1] = src[i];
  }  

  return dst;
}
