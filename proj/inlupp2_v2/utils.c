#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include "utils.h"

extern char* strdup(const char*);

void swap(int *a, int *b)
{
  int temp = *a;
  *a = *b;
  *b = temp;
}

void clear_input_buffer()
{
  int c;
  do
    {
      c = getchar();
    } while (c != '\n' && c != EOF);
}

int read_string(char* buf, int buf_size)
{
  int counter = 0;
  char tmp = getchar();
  bool bufferFull;

  while(tmp != '\n' && tmp != EOF && !bufferFull)
  {
    buf[counter] = tmp;
    ++counter;
    bufferFull = (counter >= buf_size-1);
    tmp = getchar();
  }

  buf[counter] = '\0'; //Add null terminator
  
  if(bufferFull)
    clear_input_buffer();
  
  return counter;
}

bool is_number(char* str)
{
  bool isNumber = true;
  int strLength = strlen(str);

  for(int i = 1; i < strLength && isNumber; ++i)
    isNumber = isdigit(str[i]);
  
  return isNumber && (str[0] == '-' || isdigit(str[0]));
}

int foldl_int_int(int numbers[], int numbers_size, int_fold_func f)
{
  int result = 0;

  for(int i = 0; i < numbers_size; ++i)
  {
    result = f(result, numbers[i]);
  }
  
  return result;
}

int fib(int num)
{
  switch(num)
  {
  case 0:
    return 0;
  case 1:
    return 1;
  default:
    return fib(num-1) + fib(num-2);
  }
}

long rec_sum(int numbers[], int numbers_size, int index)
{
  if (index < numbers_size)
    return numbers[index] + rec_sum(numbers, numbers_size, index+1);
  else
    return 0;
}

int add(int a, int b)
{
  return a + b;
}

long sum(int numbers[], int numbers_size)
{
  return foldl_int_int(numbers, numbers_size, add);
}

answer_t ask_question(char* question, check_func check, convert_func convert)
{
  int bufferSize = 255;
  int sizeOfInput; //Unused for now
  char buffer[bufferSize];
  answer_t answer;

  do
  {
    printf("%s\n",question);
    sizeOfInput = read_string(buffer, bufferSize);
  } while(!check(buffer) && sizeOfInput > 0);

  //Convert the input
  answer = convert(buffer);
  
  return answer;
}

int ask_question_int(char* question)
{
  answer_t answer = ask_question(question, is_number, (convert_func) atoi);
  return answer.int_value;
}

char* ask_question_string(char* question)
{
  return ask_question(question, not_empty, (convert_func) strdup).string_value;
}

bool not_empty(char* str)
{
  return strlen(str) > 0;
}

void print(char *str)
{
  while(*str != '\0')
  {
    putchar(*str);
    str++;
  }
}

void println(char *str)
{
  while(*str != '\0')
  {
    putchar(*str);
    str++;
  }
  putchar('\n');
}

char *trim(char *str)
{
  char *start = str;
  char *end = start + strlen(str)-1;

  while (isspace(*start)) ++start;
  while (isspace(*end)) --end;

  char *cursor = str;
  for(; start <= end; ++start, ++cursor)
  {
    *cursor = *start;
  }
  *cursor = '\0';
  
  return str;
}

/*
int main()
{
  char myStr[] = {'R','O','F','L','\0'};
  print(myStr);
  println(myStr);
  
  return 0;
}*/
