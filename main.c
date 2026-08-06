#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

#include "shell_ctl.h"

int main(int argc, char** argv)
{
  char* input_cmd = malloc (100 * sizeof(char));
  system("clear");

  while (!is_exit_cmd(input_cmd))
  {
    write (STDOUT_FILENO, "> ", 2);
    flush (&input_cmd);
    read (STDIN_FILENO, input_cmd, 100);
    write (STDOUT_FILENO, input_cmd, get_char_count (input_cmd));
    printf ("\ninput_cmd = %s", input_cmd);
  }

  return 0;
}

