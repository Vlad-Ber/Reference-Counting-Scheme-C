#ifndef __UTILS_H__
#define __UTILS_H__
#include <stdbool.h>

typedef union
{
  int int_value;
  char char_value;
  float float_value;
  double double_value;
  char* string_value;
} answer_t;


typedef int(*int_fold_func)(int,int);
typedef bool(*check_func)(char*);
typedef answer_t(*convert_func)(char*);

/// @brief Checks if input str is a number
/// @param str input
/// @return true if str is a number, otherwise false
bool is_number(char *str);

/// @brief Checks that a string is not empty
/// @param str to read
/// @return true if str is not empty, otherwise false
bool not_empty(char* str);

/// @brief reverse a list
/// @param numbers list of numbers
/// @param numbers_size size of list
/// @param f function
/// @return int
int foldl_int_int(int numbers[], int numbers_size, int_fold_func f);

/// @brief adds two integers
/// @param a, int to add
/// @param b, int to add
/// @return sum of a and b
int add(int a, int b);

/// @brief reads string from stdin
/// @param buf to store string in
/// @param buf_siz max size of read input
/// @return read string
int read_string(char *buf, int buf_siz);

/// @brief asks user for an int via stdin
/// @param string to print
/// @return int that user wrote via stdin
int ask_question_int(char *question);

/// @brief fib
/// @param num number to use
/// @return int
int fib(int num);

/// @brief sums up all the integers in list
/// @param list of numbers
/// @param size of list
/// @return sum of list
long sum(int numbers[], int numbers_size);

/// @brief sums up all integers in list after index
/// @param list of numbers
/// @param numbers_size size of list
/// @param index
/// @return sum of list after index
long rec_sum(int numbers[], int numbers_size, int index);

/// @brief asks user for string via stdin
/// @param question, string to print
/// @return string input from user
char* ask_question_string(char *question);

/// @brief trim a string to a single char
/// @str string to trim
/// @return trimmed string, a char
char *trim(char *str);

/// @brief function to ask questions to user
/// @param question to ask
/// @param function check
/// @param function convert
/// @return answer
answer_t ask_question(char* question, check_func check, convert_func convert);

/// @brief prints a char
/// @param char to print
void print(char *str);

/// @brief prints string with new line
/// @str to print
void println(char *str);

#endif
