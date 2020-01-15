#pragma once
#include <stdio.h>
#include <stdbool.h>




typedef union
{
    int   int_value;
    float float_value;
    char *string_value;
} answer_t;

typedef bool(*check_func)(char *str);

typedef answer_t (*convert_func)(char *str);

bool is_number(char *str);

int ask_question_int(char *question);

extern char *strdup(const char *);

bool not_empty(char *str);

answer_t ask_question(char *question, check_func check, convert_func convert);


int read_string(char *buf, int buf_siz);

char *ask_question_string(char *question);




bool is_shelf(char *str);
//char *ask_question_shelf(char *question);


char ask_question_menu(char *question);
bool is_menu(char *str);
