#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "shell_ctl.h"

int get_char_count (char* buf)
{
  int count = 0;
  while (buf[count] != '\n' && buf[count] != '\0')
  {
    count++;
  }
  return count;
}

bool is_exit_cmd (char* buf)
{
  if (buf[0] == 'q' && buf[1] == '\n') return true;
  if (buf[0] == 'Q' && buf[1] == '\n') return true;

  char* exit = "exit";
  int i;
  for (i = 0; buf[i] != '\n' && i < 4; i++)
  {
    if (buf[i] == exit[i]) continue;
    else return false;
  }
  if (i == 4 && buf[4] == '\n') return true;
  return false;
}

void flush (char** buf)
{
  fflush(stdin);
  memset(*buf, 0, sizeof (buf));
}
