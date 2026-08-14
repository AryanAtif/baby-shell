#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

#include "shell_ctl.h"
#include "cmd_execution.h"

#define MAX_CMD_SIZE 100

int main(int argc, char** argv)
{
  char* input_cmd = malloc (MAX_CMD_SIZE * sizeof(char));
  system("clear");

  while (!is_exit_cmd(input_cmd))
  {
    flush (&input_cmd);
    write (STDOUT_FILENO, "> ", 2);
    //flush (&input_cmd);
    read (STDIN_FILENO, input_cmd, MAX_CMD_SIZE);
    //write (STDOUT_FILENO, input_cmd, get_char_count (input_cmd));

    char** command = parse_input (input_cmd);
    if (command == NULL) return -1;

    exec_cmd(command);
  }

  return 0;
}

