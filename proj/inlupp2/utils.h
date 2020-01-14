#ifndef __UTILS_H__
#define __UTILS_H__
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>

/**
 * @file utils.h
 * @author Adam Axelsson, Andreas Harju Schnee
 * @date 1 Nov 2019
 * @brief Simple hash table that maps integer keys to string values.
 *
 * Here typically goes a more extensive explanation of what the header
 * defines. Doxygens tags are words preceeded by either a backslash @\
 * or by an at symbol @@.
 *
 * @see http://wrigstad.com/ioopm19/assignments/assignment1.html
 */

typedef union
{
    int   int_value;
    float float_value;
    char *string_value;
    char char_value;
} answer_t;

typedef bool(*check_func)(char *);
typedef answer_t(*convert_func)(char *);

/// @brief Reads input from the terminal.
/// @param buf, the location where the input is stored.
/// @param buf_size, the size of the input.
/// @return the size of the input string.
int read_string(char *buf, int buf_siz);

/// @brief Asks the user for a int.
/// @param question, the question to ask.
/// @return The int from the user input.
int ask_question_int(char *question);

/// @brief Asks the user for a float.
/// @param question, the question to ask.
/// @return The float from the user input.
double ask_question_float(char *question);

/// @brief Asks the user for a string.
/// @param question, the question to ask.
/// @return The string from the user input.
char *ask_question_string(char *question);

/// @brief Asks the user for a shelf name.
/// @param question, the question to ask.
/// @return the name of the shelf from the user.
char *ask_question_shelf(char *question);

/// @brief Asks the user for a menu-command.
/// @param question, the question to ask.
/// @return The menu-command from the user.
char ask_question_menu(char *question);

/// @brief Check if str is a string of integers.
/// @param str, the string to check.
/// @return true if str contains only integers, else false.
bool is_number(char *str);

/// @brief checks if str is an empty string.
/// @param str, the string to check.
/// @return true if str is not empty, else false.
bool not_empty(char *str);

/// @brief Asks a question and read input from the terminal.
/// @param question, the question to ask.
/// @param check, the function to check for correct input.
/// @param convert, the function to convert the input to the correct output-type.
/// @return The user input as an answer_t.
answer_t ask_question(char *question, check_func check, convert_func convert);


#endif
