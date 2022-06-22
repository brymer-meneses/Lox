#ifndef ERROR_H
#define ERROR_H

void report(int line, int column, char* message);
void panic(int line, int column, char* message);

#endif
