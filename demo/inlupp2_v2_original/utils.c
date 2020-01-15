#include "utils.h"
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>



answer_t ask_question(char *question, check_func check, convert_func convert)
{

    int str_siz = 255;
    char str[str_siz];
    int conversions = 0;
    do
    {
        printf("%s", question);
        read_string(str, 255);
        conversions = check(str);
    }
    while (!conversions);

    return convert(str);
}

bool not_empty(char *str)
{
    if (strlen(str) >0)
    {
        return true;
    }
    return false;
}


int read_string(char *buf, int buf_siz)
{
    int i = 0;
    char mychar;


    do
    {
        if (i < buf_siz -1)
        {
            mychar = getchar();
            buf[i] = mychar;
            i++;
        }

    }

    while(mychar != '\n');
    buf[i -1] = '\0';


    return i -1;
}



bool is_p_number(char *str)
{

    int length= strlen(str);
    bool number = true;
    if(isdigit(str[0]))
    {
        for(int i=1; i < length; i++)
        {
            int x = isdigit(str[i]);
            if(x==0)
            {
                number = false;
            }
        }
    }
    else
    {
        number = false;
    }
    if(!number)
    {
        printf("Enter a positive number \n");
    }
    return number;
}


int ask_question_int(char *question)
{
    answer_t answer = ask_question(question, is_p_number, (convert_func) atoi);
    return answer.int_value; // svaret som ett heltal
}





/*
char *ask_question_string(char *question, char *buf, int buf_siz){

  int i = 0;
  while(i == 0){
    printf("%s", question);
    i = read_string(buf, buf_siz);


  }
  return buf;
}
*/




bool is_number(char *str)
{

    int length= strlen(str);
    bool number = true;

    if(str[0]=='-' || isdigit(str[0]))
    {

        for(int i=1; i < length; i++)
        {
            int x = isdigit(str[i]);

            if(x==0)
            {
                number = false;

            }
        }
    }
    else
    {
        number = false;


    }
    if(!number)
    {
        printf("inte en int \n");
    }


    return number;
}






bool is_float(char *str)
{

    int length= strlen(str);
    bool floatnum = true;

    if(str[0]=='-' || isdigit(str[0]))
    {

        for(int i=1; i < length; i++)
        {
            int x = isdigit(str[i]);

            if(x==0 && (str[i]))
            {
                floatnum = false;
            }
        }
    }
    else
    {
        floatnum = false;


    }
    if(!floatnum)
    {
        printf("inte en float \n");
    }


    return floatnum;


}



answer_t make_float(char *str)
{
    return (answer_t)
    {
        .float_value = atof(str)
    };
}

double ask_question_float(char *question)
{
    return ask_question(question, is_float, make_float).float_value;
}



char *ask_question_string(char *question)
{


    return ask_question(question, not_empty, (convert_func) strdup).string_value;
}


bool is_shelf(char *str)
{
    bool shelf = true;
    int length = strlen(str);


    if (isupper(str[0] ) && length >= 3)
    {
        for(int i=1; i < length; i++)
        {
            if (!(isdigit(str[i]))) shelf = false;
        }
    }
    else
    {
        shelf = false;
    }

    if (shelf==false)
    {
        printf("Enter a shelf. \n");
    }
    return shelf;
}

/*char *ask_question_shelf(char *question){
  return ask_question(question, is_shelf, (convert_func) strdup).string_value;
}
*/

bool is_menu(char *str)
{
    if (strlen(str)>1)
    {
        printf("inte ett alternativ");
        return false;
    }
    char input = toupper(*str);
    if(input == 'L' || input == 'T' ||input == 'R' ||input == 'G' ||input == 'H' ||input == 'A')
    {
        return true;
    }
    else
    {
        printf("inte ett alternativ");
        return false;
    }
}

/*
int main(void)
{

  int tal;

  tal = ask_question_int("Första talet:");
  printf("Du skrev '%d'\n", tal);

  tal = ask_question_int("Andra talet:");
  printf("Du skrev '%d'\n", tal);


  int buf_siz = 255;
  int read = 0;
  char buf[buf_siz];


  puts("Läs in en sträng:");
  read = read_string(buf, buf_siz);
  printf("'%s' (%d tecken)\n", buf, read);

  puts("Läs in en sträng till:");
  read = read_string(buf, buf_siz);
  printf("'%s' (%d tecken)\n", buf, read);


  strcpy(buf, ask_question_string("skriv nåt \n"));
 printf("%s \n",buf);
  return 0;
}


*/
