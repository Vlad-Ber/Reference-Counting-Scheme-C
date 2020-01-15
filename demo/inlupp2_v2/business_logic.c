#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include "business_logic.h"
#include "hash_table.h"
#include "linked_list.h"
#include "utils.h"
#include "user_interface.h"
#include "../../src/refmem.h"

// Data types

struct merch
{
    char *name; //Names are keys when storing in ht as they are unique
    char *description; //Description of element
    size_t price;  //Price of element
    ioopm_list_t *locations;
};

struct stock
{
    char *location; //Shelfnumber on the form of |CHAR|INT|INT|
    int amount;
};

struct cart
{
    ioopm_list_t *cart_merchandise;
    int total_cost;
    int index;
};

struct cart_item
{
    merch_t *merch;
    int amount;
};

///***** Static functions *****///

static size_t hash_string(elem_t name_to_hash)
{
    size_t result = 0;
    char *key = name_to_hash.str_value;

    do
    {
        result = result * 31 + *key;

    }
    while(*++key != '\0');

    return result;
}

//Compares the string componenet of 2 elements
static bool str_compare(elem_t elem1, elem_t elem2)
{
    return strcmp(elem1.str_value, elem2.str_value) == 0;
}

static elem_t create_stock(char *shelf, int amount)
{
    stock_t *stock_information = allocate(sizeof(stock_t), doNothing);
    stock_information->location = shelf;
    stock_information->amount = amount;
    elem_t stock_p;
    stock_p.p = stock_information;

    return stock_p;
}

static void remove_locations(ioopm_hash_table_t *db_shelf, merch_t *merch_to_remove)
{

    for(size_t i = 0; i < ioopm_linked_list_size(merch_to_remove->locations); ++i)
    {
        stock_t *stock_to_remove = ioopm_linked_list_get(merch_to_remove->locations, 0).element.p;

        elem_t remove_shelf_key;
        remove_shelf_key.str_value = stock_to_remove->location;

        ioopm_linked_list_remove(merch_to_remove->locations, i);

        ioopm_hash_table_remove(db_shelf, remove_shelf_key);


        char *value_in_shelf = ioopm_hash_table_lookup(db_shelf, remove_shelf_key).element.str_value;
        free(value_in_shelf);

        //entry in struct is allocated during ask_question, free it

        free(stock_to_remove->location);

        //free struct
        deallocate(stock_to_remove);

        i = -1;
    }
    ioopm_linked_list_destroy(merch_to_remove->locations);
}

static int get_next_cart_index()
{
    static int index = 0;

    index++;
    return index;
}

//static void remove_cart_merch_with_name(ioopm_list_t *list_of_carts, char *name){}

//static void remove_from_cart_with_name(ioopm_list_t *list_of_carts);

static void remove_cart_with_name_from_carts(ioopm_list_t *list_of_carts, char *name)
{

    for(size_t i = 0; i < ioopm_linked_list_size(list_of_carts); ++i)
    {
        cart_t *temp_cart = ioopm_linked_list_get(list_of_carts, i).element.p;

        ioopm_list_t *merch_list = temp_cart->cart_merchandise;

        for(size_t j = 0; j < ioopm_linked_list_size(merch_list); ++j)
        {
            cart_item_t *cart_item = ioopm_linked_list_get(merch_list, j).element.p;

            char *merch_in_cart = cart_item->merch->name;
            //int amount_of_merch = cart_item->amount;
            //This merch exists in cart, replace it with new one
            if(strcmp(merch_in_cart, name) == 0)
            {
                ioopm_linked_list_remove(merch_list, j);
                deallocate(cart_item);
                j = -1;
            }
        }
    }
}

//AUX FUNCTIONS FOR TESTS
//-------------------------------------------------------------------------------

char *merch_getdesc(ioopm_hash_table_t *warehouse, char *name)
{
    merch_t *merch = ioopm_hash_table_lookup(warehouse, (elem_t)
    {
        .str_value = name
    }).element.p;

    return merch->description;
}

int getAmountSingel(ioopm_hash_table_t *warehouse, char *name, int index)
{
    merch_t *merch = ioopm_hash_table_lookup(warehouse, (elem_t)
    {
        .str_value = name
    }).element.p;
    if(!merch->locations)
    {
        return 0;
    }

    stock_t *singel_stock = ioopm_linked_list_get(merch->locations, index).element.p;
    int amount = singel_stock->amount;

    return amount;
}



//-------------------------------------------------------------------------------
static ioopm_list_t *recreate_locations(ioopm_hash_table_t *shelf_db, merch_t *merch_to_remove, char *name)
{

    ioopm_list_t *new_loc = ioopm_linked_list_create(str_compare);

    for(size_t i = 0; i < ioopm_linked_list_size(merch_to_remove->locations); ++i)
    {
        //Re-create the linked-list with locations that we're about to destroy
        stock_t *stock_to_copy = ioopm_linked_list_get(merch_to_remove->locations, i).element.p;

        elem_t stock_p = create_stock(strdup(stock_to_copy->location), stock_to_copy->amount);

        //ioopm_linked_list_append(new_loc, stock_p);
        ioopm_linked_list_insert(new_loc, ioopm_linked_list_size(new_loc), stock_p);
    }

    for(size_t i = 0; i < ioopm_linked_list_size(new_loc); ++i)
    {

        stock_t *loc_stock = ioopm_linked_list_get(new_loc, 0).element.p;
        char *location = loc_stock->location;

        elem_t location_key;
        location_key.str_value = location;

        elem_t name_value;
        name_value.str_value = name;

        ioopm_hash_table_insert(shelf_db, location_key, name_value);
    }

    return new_loc;
}

static bool cart_item_exists(ioopm_list_t *list_of_merch, elem_t name_of_merch, int index, int user_amount)
{
    for(size_t i = 0; i < ioopm_linked_list_size(list_of_merch); ++i)
    {
        cart_item_t *cart_item_in_cart = ioopm_linked_list_get(list_of_merch, i).element.p;
        merch_t *merch_in_cart = cart_item_in_cart->merch;
        char *merch_name_in_cart = merch_in_cart->name;

        if(strcmp(merch_name_in_cart, name_of_merch.str_value) == 0)
        {
            // printf("This merchandise already exists in cart %d\n", index);
            printf("Increasing %s stock by %d\n", merch_in_cart->name, user_amount);
            cart_item_in_cart->amount += user_amount;

            return true;
        }
    }

    return false;
}

///***** Public functions *****///

//Creates the warehouse hashtable that uses names as keys and stores merch info
ioopm_hash_table_t *warehouse_create()
{
    ioopm_hash_table_t *warehouse = ioopm_hash_table_create(hash_string,
                                    str_compare, str_compare);

    return warehouse;
}

//Creates the shelf database that uses shelf locations as keys and
//stores amount and merch info
ioopm_hash_table_t *shelf_db_create()
{
    ioopm_hash_table_t *shelf_db = ioopm_hash_table_create(hash_string,
                                   str_compare, str_compare);

    return shelf_db;
}

ioopm_list_t *create_list_of_carts()
{
    ioopm_list_t *list_of_carts = ioopm_linked_list_create(str_compare);

    return list_of_carts;
}

bool warehouse_key_has_locations(ioopm_hash_table_t *warehouse, elem_t key)
{
    merch_t *merch = ioopm_hash_table_lookup(warehouse, key).element.p;

    return merch->locations != NULL;
}

void warehouse_remove_merch_with_key(ioopm_hash_table_t *warehouse, ioopm_hash_table_t *db_shelf, elem_t key_to_remove, ioopm_list_t *list_of_carts)
{

    merch_t *merch_to_remove = ioopm_hash_table_lookup(warehouse, key_to_remove).element.p;

    //If merch has a location associated with it, remove associated values
    if(merch_to_remove->locations)
    {
        remove_locations(db_shelf, merch_to_remove);
    }

    if(list_of_carts)
    {
        remove_cart_with_name_from_carts(list_of_carts, merch_to_remove->name);
    }

    //Remove entry in ht, remove pointer pointing at merch
    ioopm_hash_table_remove(warehouse, key_to_remove);

    //Remove entries in struct as they are allocated with strdup

    free(merch_to_remove->name);
    free(merch_to_remove->description);
    //free the struct
    deallocate(merch_to_remove);
    //free the linked list of keys
}

static void warehouse_destroy_aux(ioopm_hash_table_t *warehouse, ioopm_hash_table_t *db_shelf, elem_t key_to_remove)
{
    {

        merch_t *merch_to_remove = ioopm_hash_table_lookup(warehouse, key_to_remove).element.p;

        //If merch has a location associated with it, remove associated values
        if(merch_to_remove->locations)
        {
            remove_locations(db_shelf, merch_to_remove);
        }

        //Remove entry in ht, remove pointer pointing at merch
        ioopm_hash_table_remove(warehouse, key_to_remove);

        //Remove entries in struct as they are allocated with strdup

        free(merch_to_remove->name);
        free(merch_to_remove->description);
        //free the struct
        release(merch_to_remove);
        //free the linked list of keys
    }
}

void warehouse_add_merch_with_fields(ioopm_hash_table_t *warehouse, char *name, char *description, int price, ioopm_list_t *locations)
{

    //Allocate space for the merch struct
    merch_t *merch_to_add = allocate(sizeof(merch_t), doNothing);

    //Set user inputs as fields of merch
    merch_to_add->name = name;
    merch_to_add->description = description;
    merch_to_add->price = price;
    merch_to_add->locations = locations;
    //Init key as name of merch
    elem_t warehouse_key;
    warehouse_key.str_value = name;

    //Init value as void pointer, pointing to merch
    elem_t value;
    value.p = merch_to_add;//pointer_to_merch;
    //Insert item in warehouse db
    ioopm_hash_table_insert(warehouse, warehouse_key, value);
}

bool is_merch_in_any_cart(ioopm_list_t *list_of_carts, char *name)
{
    for(size_t i = 0; i < ioopm_linked_list_size(list_of_carts); ++i)
    {
        cart_t *cart = ioopm_linked_list_get(list_of_carts, i).element.p;

        ioopm_list_t *list_of_merch = cart->cart_merchandise;

        for(size_t j = 0; j < ioopm_linked_list_size(list_of_merch); ++j)
        {
            cart_item_t *cart_item = ioopm_linked_list_get(list_of_merch, j).element.p;

            char *name_to_compare = cart_item->merch->name;

            if(strcmp(name, name_to_compare) == 0)
            {
                printf("This merch is stored in a cart, remove it from the cart before editing \n");
                return true;
            }
        }

    }
    return false;
}

//Edits an existing merchandise in the warehouse, first remove value to
//edit then insert it again as by changing its name we change its key
void warehouse_edit_merchandise_with_fields(ioopm_hash_table_t *warehouse, \
        ioopm_hash_table_t *shelf_db, \
        char *name, char *description, \
        int price, elem_t key_to_edit, \
        ioopm_list_t *list_of_carts)
{

    merch_t *merch_to_remove = ioopm_hash_table_lookup(warehouse, key_to_edit).element.p;

    if(merch_to_remove->locations)
    {
        //recreate locations we're about to mess up by editing
        ioopm_list_t *new_loc = recreate_locations(shelf_db, merch_to_remove, name);

        //remove old entry
        warehouse_remove_merch_with_key(warehouse, shelf_db, key_to_edit, list_of_carts);

        //Insert new entry with new name
        warehouse_add_merch_with_fields(warehouse, name, description, price, new_loc);

        return;
    }
    //remove merch from cart
    ///remove_cart_with_name_from_carts(list_of_carts, key_to_edit.str_value);

    //remove old entry with name
    warehouse_remove_merch_with_key(warehouse, shelf_db, key_to_edit, list_of_carts);

    //Insert new entry with new name
    warehouse_add_merch_with_fields(warehouse, name, description, price, NULL);

    //re-add merch to cart
}

void warehouse_destroy(ioopm_hash_table_t *warehouse, ioopm_hash_table_t *shelf_db, ioopm_list_t *list_of_carts)
{
    ioopm_list_t *list_of_values = ioopm_hash_table_keys(warehouse);
    //Nr of entered values
    size_t nr_of_values = ioopm_linked_list_size(list_of_values);

    //Dont list items if the warehouse is empty
    if(nr_of_values > 0)
    {
        //Use array to more easily traverse pointers
        char **stored_keys = calloc(nr_of_values, sizeof(char *));

        //Insert values from linked list to array
        for(size_t i = 0; i < nr_of_values; ++i)
        {
            stored_keys[i] = ioopm_linked_list_get(list_of_values, i).element.str_value;

            elem_t temp_key;
            temp_key.str_value = stored_keys[i];

            // warehouse_remove_merch_with_key(warehouse, shelf_db, temp_key, list_of_carts);
            warehouse_destroy_aux(warehouse, shelf_db, temp_key);
        }
        free(stored_keys);
    }

    ioopm_linked_list_destroy(list_of_values);
    ioopm_hash_table_destroy(warehouse);

    if(shelf_db)
    {
        ioopm_hash_table_destroy(shelf_db);
    }

    if(list_of_carts)
    {
        warehouse_destroy_carts(list_of_carts);
        ioopm_linked_list_destroy(list_of_carts);
    }

}


bool is_shelf_available(ioopm_hash_table_t *warehouse, ioopm_hash_table_t *shelf_db, char *shelf, elem_t key)
{
    merch_t *merch = ioopm_hash_table_lookup(warehouse, key).element.p;

    char *name_of_merch = merch->name;

    //all locations stored in the database
    ioopm_list_t *list_of_values = ioopm_hash_table_keys(shelf_db);
    //Nr of entered values
    size_t nr_of_values = ioopm_linked_list_size(list_of_values);

    char **stored_shelves = calloc(nr_of_values, sizeof(char *));

    for(size_t i = 0; i < nr_of_values; ++i)
    {
        stored_shelves[i] = ioopm_linked_list_get(list_of_values, i).element.str_value;

        if(strcmp(shelf, stored_shelves[i]) == 0)
        {
            elem_t temp_key;
            temp_key.str_value = stored_shelves[i];
            char *name_to_compare = ioopm_hash_table_lookup(shelf_db, temp_key).element.str_value;
            if(strcmp(name_to_compare, name_of_merch) == 0)
            {
                ioopm_linked_list_destroy(list_of_values);
                free(stored_shelves);

                return true;
            }
            else
            {
                printf("This location is already occupied by another merchandise, try another shelf!\n");
                ioopm_linked_list_destroy(list_of_values);
                free(stored_shelves);
                free(shelf);

                return false;
            }
        }
    }
    ioopm_linked_list_destroy(list_of_values);
    free(stored_shelves);

    return true;
}


void warehouse_store_location(ioopm_hash_table_t *warehouse, ioopm_hash_table_t *shelf_db, char *shelf, elem_t name_of_merch, int amount)
{
    merch_t *merch_to_add_list_to = ioopm_hash_table_lookup(warehouse, name_of_merch).element.p;

    if(!merch_to_add_list_to->locations)
    {
        //Check if a merch with that name already exists, as names are unique
        elem_t stock_p = create_stock(shelf, amount);
        //create linked list and store stock info in list
        ioopm_list_t *all_item_locations = ioopm_linked_list_create(str_compare);

        ioopm_linked_list_insert(all_item_locations, ioopm_linked_list_size(all_item_locations), stock_p);

        elem_t shelf_key;
        shelf_key.str_value = shelf;
        //Insert item in shelf_db
        ioopm_hash_table_insert(shelf_db, shelf_key, name_of_merch);

        merch_to_add_list_to->locations = all_item_locations;

        return;
    }
    else
    {
        //If the shelf is already a part of the merch, just increase the amount
        for(size_t i = 0; i < ioopm_linked_list_size(merch_to_add_list_to->locations); ++i)
        {
            stock_t *temp_stock = ioopm_linked_list_get(merch_to_add_list_to->locations, i).element.p;
            if(strcmp(temp_stock->location, shelf) == 0)
            {
                temp_stock->amount += amount;
                free(shelf);

                return;
            }
        }
        elem_t stock_p = create_stock(shelf, amount);

        //ioopm_linked_list_append(merch_to_add_list_to->locations, stock_p);
        ioopm_list_t *list = merch_to_add_list_to->locations;
        ioopm_linked_list_insert(list, 0, stock_p);

        elem_t shelf_key;
        shelf_key.str_value = shelf;
        //Insert item in shelf_db
        ioopm_hash_table_insert(shelf_db, shelf_key, name_of_merch);
    }
}

void warehouse_list_locations(ioopm_hash_table_t *warehouse, elem_t key)
{
    merch_t *merch = ioopm_hash_table_lookup(warehouse, key).element.p;

    ioopm_list_t *locations = merch->locations;

    printf("The merch: %s is stored at the following locations:\n", merch->name);

    for(size_t i = 0; i < ioopm_linked_list_size(locations); ++i)
    {
        stock_t *stock = ioopm_linked_list_get(locations, i).element.p;
        printf("[%d]: Shelf: %s Amount: %d\n \n \n", (int)i+1, stock->location, stock->amount);
    }
}

void warehouse_add_cart(ioopm_list_t *list_of_carts)
{
    cart_t *cart = allocate(sizeof(cart_t), doNothing);

    int index_of_cart = get_next_cart_index();
    cart->index = index_of_cart;

    ioopm_list_t *list_of_merch = ioopm_linked_list_create(str_compare);
    cart->cart_merchandise = list_of_merch;

    elem_t pointer_to_cart;
    pointer_to_cart.p = cart;

    ioopm_linked_list_insert(list_of_carts, ioopm_linked_list_size(list_of_carts), pointer_to_cart);
    printf("Adding cart with index: %d\n", index_of_cart);
}

void warehouse_delete_cart_with_index(ioopm_list_t *list_of_carts, int index)
{
    cart_t *cart_to_remove = ioopm_linked_list_get(list_of_carts, index).element.p;

    ioopm_list_t *list_of_merch = cart_to_remove->cart_merchandise;

    // size_t size_of_list = ioopm_linked_list_size(list_of_merch);

    for(size_t i = 0; i < ioopm_linked_list_size(list_of_merch); ++i)
    {
        cart_item_t *cart_item = ioopm_linked_list_get(list_of_merch, i).element.p;

        ioopm_linked_list_remove(list_of_merch, i);
        deallocate(cart_item);

        //reset i
        i = -1;
    }

    ioopm_linked_list_destroy(list_of_merch);
    ioopm_linked_list_remove(list_of_carts, index);

    deallocate(cart_to_remove);
}

int get_cart_index(ioopm_list_t *list_of_carts, int index)
{

    cart_t *cart = ioopm_linked_list_get(list_of_carts, index).element.p;

    if(!cart)
    {
        return 0;
    }

    return cart->index;
}

void warehouse_destroy_carts(ioopm_list_t *list_of_carts)
{
    while(ioopm_linked_list_size(list_of_carts) > 0)
    {
        warehouse_delete_cart_with_index(list_of_carts, 0);
    }
}

void add_merchandise_to_cart_with_index(ioopm_hash_table_t *warehouse, \
                                        ioopm_list_t *list_of_carts, int index, \
                                        elem_t name_of_merch, int user_amount)
{
    merch_t *merch_to_insert = ioopm_hash_table_lookup(warehouse, name_of_merch).element.p;
    cart_t *cart_to_add_to = ioopm_linked_list_get(list_of_carts, index).element.p;

    ioopm_list_t *list_of_merch = cart_to_add_to->cart_merchandise;

    if(!cart_item_exists(list_of_merch, name_of_merch, index, user_amount))
    {
        cart_item_t *cart_item = allocate(sizeof(cart_item_t), doNothing);

        cart_item->merch = merch_to_insert;
        cart_item->amount = user_amount;

        elem_t merch_p;
        merch_p.p = cart_item;

        ioopm_linked_list_insert(list_of_merch,ioopm_linked_list_size(list_of_merch), merch_p);
        //ioopm_linked_list_append(list_of_merch, merch_p);
    }
}

void warehouse_list_cart_merch(ioopm_list_t *list_of_carts, int index)
{
    cart_t *cart = ioopm_linked_list_get(list_of_carts, index).element.p;

    ioopm_list_t *list_of_merch = cart->cart_merchandise;

    printf("cart with index %d contains the following items and amounts:\n", cart->index);

    for(size_t i = 0; i < ioopm_linked_list_size(list_of_merch); ++i)
    {
        cart_item_t *cart_item = ioopm_linked_list_get(list_of_merch, i).element.p;

        int amount_to_show = cart_item->amount;
        char *name_of_merch = cart_item->merch->name;

        printf("[%d] %d of %s \n", (int)i+1,  amount_to_show, name_of_merch);
    }
}

void remove_cart_merch_with_index(ioopm_list_t *list_of_carts, int cart_index, int merch_index, int amount_to_remove)
{
    cart_t *cart = ioopm_linked_list_get(list_of_carts, cart_index).element.p;

    ioopm_list_t *list_of_merch = cart->cart_merchandise;

    cart_item_t *cart_item = ioopm_linked_list_get(list_of_merch, merch_index).element.p;

    //If user wants to remove more or as many items as there are in cart, remove merch entirely
    if(amount_to_remove >= cart_item->amount)
    {
        deallocate(cart_item);
        ioopm_linked_list_remove(list_of_merch, merch_index);

    }
    else
    {
        cart_item->amount -= amount_to_remove;

    }
}

size_t warehouse_total_cost_of_cart_with_index(ioopm_list_t *list_of_carts, int index)
{

    size_t cost = 0;
    cart_t *cart_to_calc = ioopm_linked_list_get(list_of_carts, index).element.p;

    ioopm_list_t *list_of_merch = cart_to_calc->cart_merchandise;
    size_t nr_of_merch = ioopm_linked_list_size(list_of_merch);

    for(size_t i = 0; i < nr_of_merch; ++i)
    {
        cart_item_t *cart_item = ioopm_linked_list_get(list_of_merch, i).element.p;

        int amount = cart_item->amount;
        int price = cart_item->merch->price;

        cost += amount*price;
    }

    return cost;
}


bool amount_to_add_available(ioopm_hash_table_t *warehouse, elem_t key, int amount_to_add)
{
    merch_t *merch = ioopm_hash_table_lookup(warehouse, key).element.p;
    size_t amount_stored = 0;

    ioopm_list_t *locs = merch->locations;
    if(!locs)
    {
        printf("There is not enough merchandise available to complete the action with your specified amount, use the replenish function to add more merchandise\n");
        return false;
    }

    for(size_t i = 0; i < ioopm_linked_list_size(locs); ++i)
    {
        stock_t *stock = ioopm_linked_list_get(locs, i).element.p;

        amount_stored += stock->amount;
    }
    if(amount_stored >= (size_t) amount_to_add)
    {
        return true;
    }
    else
    {

        printf("There is not enough merchandise available to complete the action with your specified amount, use the replenish function to add more merchandise\n");

        return false;
    }
}

bool is_merch_stocked(ioopm_hash_table_t *warehouse, elem_t key)
{
    merch_t *merch = ioopm_hash_table_lookup(warehouse, key).element.p;

    if(merch->locations)
    {
        return true;
    }
    else
    {
        printf("This merch is currently not stored anywhere, use the replenish function to store it somewhere first!\n");

        return false;
    }
}

size_t warehouse_size_of_cart(ioopm_list_t *list_of_carts, int index)
{
    cart_t *cart = ioopm_linked_list_get(list_of_carts, index).element.p;

    if(!cart)
    {
        return 0;
    }

    ioopm_list_t *list_of_merch = cart->cart_merchandise;

    return ioopm_linked_list_size(list_of_merch);
}

static void remove_amount_from_merch(merch_t *merch, int amount, ioopm_hash_table_t *shelf_db)
{
    ioopm_list_t *locations = merch->locations;

    //size_t nr_of_locations = ioopm_linked_list_size(locations);

    for(size_t i = 0; i < ioopm_linked_list_size(locations); ++i)
    {
        stock_t *stock = ioopm_linked_list_get(locations, i).element.p;

        int amount_in_stock = stock->amount;
        //If we need to remove more amount than what is stored at place X, remove all from place X and keep going through locations
        if (amount >= amount_in_stock)
        {
            char *remove_shelf = stock->location;
            elem_t key_to_remove;
            key_to_remove.str_value = remove_shelf;
            ioopm_hash_table_remove(shelf_db, key_to_remove);

            ioopm_linked_list_remove(locations, i);
            free(stock->location);
            deallocate(stock);
            //reset for loop
            i = -1;
            //reduce amount to remove, by amount removed
            amount -= amount_in_stock;

            if(ioopm_linked_list_is_empty(locations))
            {

                ioopm_linked_list_destroy(locations);
                merch->locations = NULL;

                return;
            }
        }
        else
        {
            stock->amount -= amount;

            return;
        }
    }
}


void warehouse_checkout_cart_aux(ioopm_hash_table_t *warehouse, ioopm_list_t *list_of_carts, int index, ioopm_hash_table_t *shelf_db)
{
    //1. Remove amount stored in cart from each merch
    //2. Remove cart
    cart_t *cart = ioopm_linked_list_get(list_of_carts, index).element.p;

    if(!cart)
    {
        return;
    }

    ioopm_list_t *cart_merchandise = cart->cart_merchandise;

    for(size_t i = 0; i < ioopm_linked_list_size(cart_merchandise); ++i)
    {
        cart_item_t *cart_item = ioopm_linked_list_get(cart_merchandise, i).element.p;

        merch_t *merchandise = cart_item->merch;
        elem_t key;
        key.str_value = merchandise->name;

        int amount_to_remove = cart_item->amount;


        if(amount_to_add_available(warehouse, key, amount_to_remove))
        {
            remove_amount_from_merch(merchandise, amount_to_remove, shelf_db);
        }
        else
        {
            return;
        }
    }

    warehouse_delete_cart_with_index(list_of_carts, index);
}

bool is_cart_empty(ioopm_list_t *list_of_carts, int index)
{
    cart_t *cart = ioopm_linked_list_get(list_of_carts, index).element.p;

    if(!cart)
    {
        printf("There is not a cart at this index! \n");
        return true;
    }

    ioopm_list_t *cart_merch = cart->cart_merchandise;

    if(!ioopm_linked_list_is_empty(cart_merch))
    {
        return false;
    }
    else
    {
        printf("The cart is empty! Try adding something to it first\n");

        return true;
    }
}
