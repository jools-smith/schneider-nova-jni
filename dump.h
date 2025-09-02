
#ifndef DUMP_H
#define DUMP_H

#ifdef DUMP

#define DEBUG_PRINT(format, ...) { printf(format, ##__VA_ARGS__); printf("\n"); }
#define DEBUG_PRINTLN(a) { printf("%s...\n", a); }

#else

#define DEBUG_PRINT(format, ...) //
#define DEBUG_PRINTLN(a) //
#endif


#endif
