#include "scanner.h"
#include "stdio.h"
#include "stdbool.h"
#include "string.h"

#include "declarations.h"


void run(char source[]) {
  Scanner scanner = scanner_create(source);
  scanner_scan(&scanner);

  printf("%s", source);

}

void run_file(char filename[]) {

}

void run_prompt() {

  while(true) {
    char line[MAX_INPUT_LIMIT];
    fgets(line, sizeof(line) , stdin);
    run(line);

    if (strcmp(line, "\n") == 0)  break; 
  }

}


int main(int argc, char *argv[]) {

  if (argc == 2) {
    run_file(argv[0]);
  } else if (argc == 1) {
    run_prompt();
  } else {
    printf("Usage clox <filename>");
  }

}
