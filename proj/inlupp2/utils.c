#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "utils.h"
#include <ctype.h>



extern char *strdup(const char *);

/*
typedef union {
  int   int_value;
  float float_value;
  char *string_value;
  char char_value;
  } answer_t;


typedef bool(*check_func)(char*);
typedef answer_t(*convert_func)(char*);
*/

bool is_number(char *str)
{
    // printf("%d\n", str[0]);
    if (str[0] == 45 || isdigit(str[0]))
    {
        for (int i = 1; i < (strlen(str) - 1); ++i )
        {
            //printf("%d\n", str[i]);
            if (!(isdigit(str[i])))
            {
                return false;
            }
        }
    }
    else
        return false;
    return true;
}


bool is_float(char *str)
{
    int point_num = 1;
    if (str[0] == 45 || isdigit(str[0]))
    {
        for (int i = 1;  i < strlen(str); ++i )
        {
            if (str[i] == 46)
            {
                point_num = point_num - 1;
            }
            if (!(isdigit(str[i])) || point_num < 0)
            {
                return false;
            }
        }
    }
    else
        return false;
    return true;
}


bool is_menu(char* str)
{
    if (strlen(str) != 1)
    {
        return false;
    }
    char* str1 = "LlTtRrGgHhAa";
    for (int i = 0; i <= 12; ++i)
    {
        if(*str == str1[i])
        {
            return true;
        }
    }
    return false;
}

bool is_menu_command(char *c)
{
    if(isalpha(c[0]) && strlen(c) == 1)
    {
        char *command_list = "LlTtRrGgHhAaCcQqEeSsMmDdOoKkXx";
        int i = 0;
        for(; i < strlen(command_list); i++)
        {
            if(c[0] == command_list[i])
            {
                return true;
            }
        }
        return false;
    }
    else
    {
        return false;
    }
}



/// Hjälpfunktion till ask_question_string
bool not_empty(char *str)
{
    return strlen(str) > 0;
}

answer_t make_float(char *str)
{
    answer_t a;                // skapa ett oinitierat answer_t-värde
    a.float_value = atof(str); // gör det till en float, via atof
    return a;                  // returnera värdet
}


int string_length(char* str)
{
    if (strcmp(str,""))
    {
        return 0;
    }
    else
    {
        int result = 0;
        while (str[result] != 0)
        {
            ++result;
        }
        return result;
    }
}

int print(char* str)
{
    for(int i = 0; i < string_length(str); ++i)
    {
        putchar(str[i]);
    }
    return 0;
}

int println(char* str)
{
    char *str1 = malloc(string_length(str) + 2);
    strcpy(str1, str);
    strcat(str1, "\n");
    print(str1);
    return 0;
}

int read_string(char *buf, int buf_siz)
{
    int result = 0;
    do
    {
        // printf("%d\n", result);
        buf[result] = getchar();
        ++result;
    }
    while ((result)  < (buf_siz) && buf[result - 1] != '\n');

    if (buf[result -1] != '\n')
    {
        int c = 0;
        do
        {
            c = getchar();
        }
        while (c != '\n'&& c != EOF);
    }
    buf[result -1] = '\0';
    return (result - 1);
}

answer_t ask_question(char *question, check_func check, convert_func convert)
{
    char buff[50];
    do
    {
        printf("%s\n", question);
        read_string( buff, 50);
        // printf("%s\n", buff);
    }
    while (!(check(buff)));
    return convert(buff);
}

int ask_question_int(char *question)
{
    answer_t answer = ask_question(question, is_number, (convert_func) atoi);
    return answer.int_value; // svaret som ett heltal
}



double ask_question_float(char *question)
{
    return ask_question(question, is_float, make_float).float_value;
}


char *ask_question_string(char *question)
{
    return ask_question(question, not_empty, (convert_func) strdup).string_value;
}

answer_t toupper_wrap(char *c)
{
    answer_t answer = {.char_value = toupper(c[0])};
    return answer;
}

bool is_shelf(char *str)
{
    if(strlen(str) != 3)
    {
        return false;
    }
    else if(isalpha(str[0]))
    {
        int i = 1;
        while(i < strlen(str))
        {
            if(!isdigit(str[i]))
            {
                return false;
            }
            else
            {
                i++;
            }
        }
        return true;
    }
    else
    {
        return false;
    }
}

char *ask_question_shelf(char *question)
{
    return ask_question(question, is_shelf, (convert_func) strdup).string_value;
}

char ask_question_menu(char *question)
{
    return ask_question(question, is_menu_command, (convert_func) toupper_wrap).char_value;
}
