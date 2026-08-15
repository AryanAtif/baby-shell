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
          return NULL; 
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
  
  printf ("cmd = %s\n", cmd[0]);
  if (child_pid == 0) // child process
  {
    if (strcmp(cmd[0], "cd") == 0)
    { 
      char* dir = NULL;
      if (cmd[1][0] == '~')
      {
        dir = concat(dir, cmd[1]);
  /*      concat (&dir, getenv("HOME"));
        char* home = getenv ("HOME");
        int length_home = strlen(home);
        dir = realloc (dir, (length_home) * sizeof(char));
        memcpy(dir, home, length_home);
        print_str (dir, length_home);

        for (int i = 1; i <= strlen (cmd[1]); i++)
        {
          printf ("inside the loop\n i = %d\nstrlen(cmd[1]) = %d\nlength_Home+1 = %d\n", i, strlen(cmd[1]), length_home+i);
          if (char* temp = realloc (dir, (length_home + i) * sizeof(char))) dir = temp;

          dir[length_home + i] = cmd[1][i];
          print_str (dir, length_home + i);
        } printf ("\n"); */
      print_str (dir, strlen(getenv("HOME")) + strlen(cmd[1]));
      } 
      else
      { printf ("inside else (cd)\n");
        dir = cmd[1];
      }

      printf ("cd: %d\n", chdir(dir));
      perror(NULL);
      printf ("Out of the loop\n");
    }
    else 
    {
      printf ("Inside exec\n");
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
  printf ("parameter count: %d\n", cmd->parameter_count);
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
  print_str (dst, length_home);

  for (int i = 1; i <= strlen (src); i++)
  {
    printf ("inside the loop\n i = %d\nstrlen(src) = %d\nlength_Home+1 = %d\n", i, strlen(src), length_home+i);
    if (char* temp = realloc (dst, (length_home + i) * sizeof(char))) dst = temp;
    else perror(NULL);

    dst[length_home + i] = src[i];
    print_str (dst, length_home + i);
  } printf ("\n"); 

  return dst;
}
