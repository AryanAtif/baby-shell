#ifndef CMD_EXECUTION_H
#define CMD_EXECUTION_H

#define extern MAX_CMD_SIZE 

struct Cmd
{
  int cmd_size; 
  char* command;
  
  int flag_count;
  char* flags;
  
  int parameter_count;
  char** parameter; // there can be more than one parameter
};

char** merge_cmd (struct Cmd* cmd);
int parse_input (char* input);
int exec_cmd ();

#endif
