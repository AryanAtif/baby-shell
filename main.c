#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

#include "shell_ctl.h"

#define MAX_CMD_SIZE 100

int main(int argc, char** argv)
{
  char* input_cmd = malloc (MAX_CMD_SIZE * sizeof(char));
  system("clear");

  while (!is_exit_cmd(input_cmd))
  {
    write (STDOUT_FILENO, "> ", 2);
    flush (&input_cmd);
    read (STDIN_FILENO, input_cmd, MAX_CMD_SIZE);
    write (STDOUT_FILENO, input_cmd, get_char_count (input_cmd));
  }

  return 0;
}

