#include <stdio.h>
#include <stdlib.h>
#include "list_linked.h"
#include "iterator.h"
#include "hash_table.h"
#include "utils.h"
#include "common.h"


struct cart
{
    int id;
    ioopm_list_t *order;
};

struct merch
{
    char *name;
    char *description;
    int price;
    ioopm_list_t *shelf;
};

struct shelf
{
    char *shelf_name;
    int quantity;
};

struct database
{
    ioopm_hash_table_t *merch_db;
    ioopm_hash_table_t *shelf_db;
    ioopm_list_t *carts;
    int number_of_carts;
};

static void print_shelfs(ioopm_list_t *shelfs)
{
    ioopm_list_iterator_t *iter = ioopm_list_iterator(shelfs);
    if(ioopm_iterator_has_next(iter))
    {
        while(ioopm_iterator_has_next(iter))
        {
            ioopm_iterator_next(iter);
            elem_t shelf = ioopm_iterator_current(iter);
            printf("-------------\nShelf: %s, Quakntity: %d\n------------- \n", shelf.shelf_value->shelf_name, shelf.shelf_value->quantity);
        }
    }
    else
    {
        puts("Merch not in stock");
    }
    ioopm_iterator_destroy(iter);
}

void ioopm_show_stock(ioopm_db_t *database)
{
    elem_t merch = {.str_value = ask_question_string("Which merch to show stock for:")};
    elem_t saved_merch;
    elem_t *saved_merch_ptr = &saved_merch;
    if (ioopm_hash_table_is_empty(database->merch_db))
    {
        free(merch.str_value);
        puts("Database is empty");
    }
    else if (!ioopm_hash_table_lookup(database->merch_db, merch, saved_merch_ptr))
    {
        free(merch.str_value);
    }
    else
    {
        free(merch.str_value);
        print_shelfs(saved_merch_ptr->merch_value->shelf);
    }
}

static void print_merch(ioopm_list_iterator_t *iter)
{
    elem_t merch_to_print = ioopm_iterator_current(iter);
    char *name = merch_to_print.str_value;
    puts("-------------");
    printf("Name: %s \n", name);
    puts("-------------");
}

void ioopm_list_merch_UI(ioopm_db_t *database) //ABSTRAHERA?
{
    ioopm_list_t *merch_names = ioopm_hash_table_keys(database->merch_db);
    ioopm_list_iterator_t *iter = ioopm_list_iterator(merch_names);
    int counter = 0;
    char option = 'n';
    if(ioopm_iterator_has_next(iter))
    {
        while(ioopm_iterator_has_next(iter))
        {
            if(counter == 20)
            {
                counter = 0;
                while(true)
                {
                    option = ask_question_menu("'C' to continue or 'Q' to quit");
                    if(option == 'Q' || option == 'C')
                    {
                        break;
                    }
                }
            }
            if(option == 'Q')
            {
                break;
            }
            ioopm_iterator_next(iter);
            print_merch(iter);
            counter++;
        }
    }
    else
    {
        puts("Database is empty");
    }
    ioopm_iterator_destroy(iter);
    ioopm_linked_list_destroy(merch_names);
}

void ioopm_list_carts(ioopm_db_t *db)
{
    ioopm_list_iterator_t *iter = ioopm_list_iterator(db->carts);
    while(ioopm_iterator_has_next(iter))
    {
        ioopm_iterator_next(iter);
        int id_to_print = ioopm_iterator_current(iter).cart_value->id;
        printf("Cart %d\n", id_to_print);
    }
    ioopm_iterator_destroy(iter);
}
