#include <stdio.h>
#include <termios.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

void flush (char** buf);
int get_char_count (char* buf);
bool is_exit_cmd (char* buf);

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

int get_char_count (char* buf)
{
  int count = 0;
  while (buf[count] != '\n')
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
