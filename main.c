#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

typedef struct {
  char* buffer;
  size_t buffer_lenght;
  ssize_t input_lenght;
} InputBuffer;

InputBuffer* new_input_buffer();
void print_prompt();
void read_prompt(InputBuffer* input_buffer);
void close_input_buffer(InputBuffer* input_buffer);

int main(int argc, char* argv[])
{
  InputBuffer* input_buffer = new_input_buffer();
  while(true) {
    print_prompt();
    read_prompt(input_buffer);
    if(strcmp(input_buffer->buffer, "exit") == 0) {
        printf("Bye bye :)\n");
        close_input_buffer(input_buffer);
        exit(EXIT_SUCCESS);
    } else {
        printf("The following command '%s' wasn't recognized.\n", input_buffer->buffer);
    }
  }
}

InputBuffer* new_input_buffer()
{
    InputBuffer* input_buffer = (InputBuffer*)malloc(sizeof(InputBuffer));
    input_buffer->buffer_lenght = 0;
    input_buffer->input_lenght = 0;

    return input_buffer;
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
