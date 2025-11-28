#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

typedef struct {
  char* buffer;
  size_t buffer_lenght;
  ssize_t input_lenght;
} InputBuffer;

typedef enum { STATEMENT_INSERT, STATEMENT_SELECT } StatementType;

typedef struct {
  StatementType type;
} Statement;

typedef enum { META_COMMAND_SUCCESS, META_COMMAND_UNRECOGNIZED_COMMAND } MetaCommandResult;

typedef enum { PREPARE_SUCCESS, PREPARE_UNRECOGNIZED_STATEMENT } PrepareResult;

void print_prompt();
void read_prompt(InputBuffer* input_buffer);
void close_input_buffer(InputBuffer* input_buffer);
void execute_statement(Statement* statement);

InputBuffer* new_input_buffer();
PrepareResult prepare_statement(InputBuffer* input_buffer, Statement* statement);
MetaCommandResult do_meta_command(InputBuffer* input_buffer);

int main(int argc, char* argv[])
{
  InputBuffer* input_buffer = new_input_buffer();
  while(true) {
    print_prompt();
    read_prompt(input_buffer);
    if(input_buffer->buffer[0] == '.') {
      switch(do_meta_command(input_buffer)) {
        case(META_COMMAND_SUCCESS):
          continue;
        case(META_COMMAND_UNRECOGNIZED_COMMAND):
          printf("Unrecognized command: '%s'\n", input_buffer->buffer);
          continue;
      }
    }

    Statement statement;
    switch(prepare_statement(input_buffer, &statement)) {
      case(PREPARE_SUCCESS):
        break;
      case(PREPARE_UNRECOGNIZED_STATEMENT):
        printf("Unrecognized statement at: '%s'\n", input_buffer->buffer);
        continue;
    }

    execute_statement(&statement);
    printf("Executed.\n");
  }
}

InputBuffer* new_input_buffer()
{
    InputBuffer* input_buffer = (InputBuffer*)malloc(sizeof(InputBuffer));
    input_buffer->buffer_lenght = 0;
    input_buffer->input_lenght = 0;

    return input_buffer;
}

PrepareResult prepare_statement(InputBuffer* input_buffer, Statement* statement)
{
  if(strncmp(input_buffer->buffer, "insert", 6) == 0) {
    return PREPARE_SUCCESS;
  }

  if(strcmp(input_buffer->buffer, "select") == 0) {
    return PREPARE_SUCCESS;
  }

  return PREPARE_UNRECOGNIZED_STATEMENT;
}

void execute_statement(Statement* statement)
{
  switch(statement->type) {
    case(STATEMENT_INSERT):
      printf("insert statement\n");
      break;
    case(STATEMENT_SELECT):
      printf("select statement\n");
      break;
  }
}

MetaCommandResult do_meta_command(InputBuffer* input_buffer)
{
  if(strcmp(input_buffer->buffer, ".exit") == 0) {
    printf("Bye Bye :)\n");
    exit(EXIT_SUCCESS);
  } else {
    return META_COMMAND_UNRECOGNIZED_COMMAND;
  }
}

void print_prompt()
{
    printf("dbc >");
}

void read_prompt(InputBuffer* input_buffer)
{
    ssize_t bytes_read = getline(&(input_buffer->buffer), &(input_buffer->buffer_lenght), stdin);
    
    if(bytes_read <= 0) {
        close_input_buffer(input_buffer);
        exit(EXIT_FAILURE);
    }

    input_buffer->input_lenght = bytes_read - 1;
    input_buffer->buffer[bytes_read - 1] = 0;
}

void close_input_buffer(InputBuffer* input_buffer)
{
    free(input_buffer->buffer);
    free(input_buffer);
}
