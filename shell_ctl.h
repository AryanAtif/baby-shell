#ifndef SHELL_CTL_H
#define SHELL_CTL_H

void flush (char** buf);
int get_char_count (char* buf);
bool is_exit_cmd (char* buf);

#endif
