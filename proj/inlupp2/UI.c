#include "buisness_logic.h"
#include "utils.h"
#include "hash_table.h"
#include "iterator.h"
#include "list_linked.h"
#include "frontend.h"

void ioopm_add_merch_UI(ioopm_db_t *database)
{
    char *merch_name = ask_question_string("Name for the new merchandise?");
    char *merch_description = ask_question_string("Description for the new merchandise?");
    int merch_price = ask_question_int("Price for the new merchandise");
    if(merch_price > 0)
    {
        ioopm_add_merch(database, merch_name, merch_description, merch_price);
    }
    else
    {
        puts("Can't give merch a negative price");
        free(merch_name);
        free(merch_description);
    }
}

void ioopm_remove_merch_UI(ioopm_db_t *database)
{
    char *merch_to_remove = ask_question_string("What is the name of the merch to remove?");
    ioopm_remove_merch(database, merch_to_remove);
    free(merch_to_remove);
}

void ioopm_edit_merch_UI(ioopm_db_t *database)
{
    char *merch_name = ask_question_string("Merch to edit: ");
    char *new_name = ask_question_string("New merch name: ");
    char *new_desc = ask_question_string("New merch description: ");
    int new_price = ask_question_int("New price: ");
    if(new_price < 0)
    {
        puts("Can't give merch a negative price");
        free(merch_name);
        free(new_name);
        free(new_desc);
    }
    else if(!ioopm_edit_merch(database, merch_name, new_name, new_desc, new_price))
    {
        free(merch_name);
        free(new_name);
        free(new_desc);
    }
    else
    {
        free(merch_name);
    }

}

void ioopm_replenish_stock_UI(ioopm_db_t *database)
{
    char *merch_name = ask_question_string("What merch do you want to replenish?");
    int no_items = ask_question_int("Replenish stock by how much?:");
    char *shelf_name = ask_question_shelf("On what shelf do you want to replenish?");
    bool shelf_used = ioopm_replenish_stock(database, merch_name, shelf_name, no_items);
    if(!shelf_used)
    {
        free(shelf_name);
        free(merch_name);
    }
}

void ioopm_remove_cart_UI(ioopm_db_t *db)
{
    int cart_id = ask_question_int("Cart to remove?");
    ioopm_remove_cart(db, cart_id);
}

void ioopm_add_to_cart_UI(ioopm_db_t *db)
{
    int cart_id = ask_question_int("Cart to add to?");
    char *merch_name = ask_question_string("Merch to add?");
    int quantity = ask_question_int("How much to add?");
    bool merch_name_used = ioopm_add_to_cart(db, merch_name, quantity, cart_id);
    if (!merch_name_used)
    {
        free(merch_name);
    }
}

void ioopm_remove_from_cart_UI(ioopm_db_t *db)
{
    int cart_id = ask_question_int("Cart to remove from?");
    char *merch_name = ask_question_string("Merch to remove: ");
    int quantity = ask_question_int("Quantity to remove");
    if (quantity < 0)
    {
        puts("You cant remove a negative quantity");
    }
    else
    {
        ioopm_remove_from_cart(db, cart_id, merch_name, quantity);
    }
    free(merch_name);
}

void ioopm_cost_UI(ioopm_db_t *db)
{
    int cart_id = ask_question_int("Cart to calculate cost for: ");
    int cost = ioopm_calculate_cost(db, cart_id);
    printf("%d \n", cost);
}

void ioopm_checkout_UI(ioopm_db_t *db)
{
    int cart_id = ask_question_int("Cart to checkout: ");
    if (!ioopm_checkout_possible(db, cart_id))
    {
        puts("Invalid checkout");
    }
    else
    {
        int cost = ioopm_calculate_cost(db, cart_id);
        printf("Total cost for cart: %d \n", cost);
        ioopm_checkout(db, cart_id);
    }
}

void event_loop(ioopm_db_t *db)
{
    char menu_choice = ask_question_menu("A - add\nL - list\nQ - quit\nT - Remove merch\nE - edit\nS - Show stock\nR - Replenish\nC - Create cart\nM - Remove cart\nX - List carts\nD - add to cart\nO - Remove from cart\nK - Checkout cart");
    while(menu_choice != 'Q')
    {
        if(menu_choice == 'A')
        {
            ioopm_add_merch_UI(db);
        }
        else if(menu_choice == 'L')
        {
            ioopm_list_merch_UI(db);
        }
        else if(menu_choice == 'T')
        {
            ioopm_remove_merch_UI(db);
        }
        else if(menu_choice == 'E')
        {
            ioopm_edit_merch_UI(db);
        }
        else if(menu_choice == 'S')
        {
            ioopm_show_stock(db);
        }
        else if(menu_choice == 'R')
        {
            ioopm_replenish_stock_UI(db);
        }
        else if(menu_choice == 'C')
        {
            puts("---------------------");
            puts("New cart created");
            puts("---------------------");
            ioopm_create_cart(db);
        }
        else if(menu_choice == 'M')
        {
            ioopm_remove_cart_UI(db);
        }
        else if(menu_choice == 'D')
        {
            ioopm_add_to_cart_UI(db);
        }
        else if(menu_choice == 'O')
        {
            ioopm_remove_from_cart_UI(db);
        }
        else if(menu_choice == 'K')
        {
            ioopm_checkout_UI(db);
        }
        else if(menu_choice == 'X')
        {
            puts("List of carts");
            puts("---------------------");
            ioopm_list_carts(db);
            puts("---------------------");
        }
        menu_choice = ask_question_menu("A - add\nL - list\nQ - quit\nT - Remove merch\nE - edit\nS - Show stock\nR - Replenish\nC - Create cart\nM - Remove cart\nX - List carts\nD - add to cart\nO - Remove from cart\nK - Checkout cart");
    }
}

int main()
{
    ioopm_db_t *database = ioopm_database_create();
    event_loop(database);
    ioopm_destroy_database(database);
    return 0;
}

