#ifndef COLOR_H
#define COLOR_H

// couleurs en code ANSI
#define COLOR_RED  "\033[31m"
#define COLOR_BLUE  "\033[34m"
#define COLOR_RESET  "\033[0m"

#define PRINT_RED(text)    COLOR_RED text COLOR_RESET
#define PRINT_BLUE(text)  COLOR_BLUE text COLOR_RESET

#endif
