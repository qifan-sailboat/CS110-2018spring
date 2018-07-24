
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "utils.h"
#include "tables.h"

const int SYMBOLTBL_NON_UNIQUE = 0;
const int SYMBOLTBL_UNIQUE_NAME = 1;

/*******************************
 * Helper Functions
 *******************************/

void allocation_failed() {
    write_to_log("Error: allocation failed\n");
    exit(1);
}

void addr_alignment_incorrect() {
    write_to_log("Error: address is not a multiple of 4.\n");
}

void name_already_exists(const char* name) {
    write_to_log("Error: name '%s' already exists in table.\n", name);
}

void write_sym(FILE* output, uint32_t addr, const char* name) {
    fprintf(output, "%u\t%s\n", addr, name);
}

/*******************************
 * Symbol Table Functions
 *******************************/

/* Creates a new SymbolTable containg 0 elements and returns a pointer to that
   table. Multiple SymbolTables may exist at the same time. 
   If memory allocation fails, you should call allocation_failed(). 
   Mode will be either SYMBOLTBL_NON_UNIQUE or SYMBOLTBL_UNIQUE_NAME. You will need
   to store this value for use during add_to_table().
 */
SymbolTable* create_table(int mode) {
    SymbolTable* new_Table = (SymbolTable *) malloc(sizeof(SymbolTable));
    /* Allocate memory to store the new_Table */

    if (new_Table == NULL) allocation_failed();
    /* If allocation fails, call allocation_failed() function. */

    new_Table -> mode = mode;
    /* Give the mode value to the table.*/
    new_Table -> tbl = (Symbol*) malloc(0);
    /* In this table, we have several ways to implement the table(tbl) */
    /* For convenience, we can just allocate a very large static table one time (for example, 10000 * sizeof(Symbol)) */
    /* However it takes too much storage space and need a lot of time to allocate. */
    /* To save storage space and be efficient, I choose to use a live table to data. */
    /* whose size varies with the number of the elements we want to store. */
    if (new_Table -> tbl == NULL){
      /* If mem allocation to a table fails, */
      /* In order to prevent mem leak, we need to free the new_Table itself */
      /* Before calling the allocation_failed() function. */
      free(new_Table);
      allocation_failed();
    }

    new_Table -> len = 0;
    /* No elements, therefore the length is 0. */
    new_Table -> cap = 1;
    /* Till now, according to the mem allocation, we can only hold 1 element. */

    return new_Table;
}

/* Frees the given SymbolTable and all associated memory. */
void free_table(SymbolTable* table) {
    uint32_t i;
    /* use as a uint32_t var as a temp var*/
    /* for table -> len is a uint32_t var, */
    /* we have to use uint32_t instead of int. */

    for (i = 0; i <= table -> len; i++) free(table -> tbl[i].name);
    /* free every name in the Symbol table */
    free(table -> tbl);
    /* free the Symbol table */
    free(table);
    /* free the whole table. */
}

/* Adds a new symbol and its address to the SymbolTable pointed to by TABLE. 
   1. ADDR is given as the byte offset from the first instruction. 
   2. The SymbolTable must be able to resize itself as more elements are added. 

   3. Note that NAME may point to a temporary array, so it is not safe to simply
   store the NAME pointer. You must store a copy of the given string.

   4. If ADDR is not word-aligned, you should call addr_alignment_incorrect() 
   and return -1. 

   5. If the table's mode is SYMTBL_UNIQUE_NAME and NAME already exists 
   in the table, you should call name_already_exists() and return -1. 

   6.If memory allocation fails, you should call allocation_failed(). 

   Otherwise, you should store the symbol name and address and return 0.
 */
int add_to_table(SymbolTable* table, const char* name, uint32_t addr) {
    uint32_t i = 0;
    /* use as a uint32_t var as a temp var*/
    /* for table -> len is a uint32_t var, */
    /* we have to use uint32_t instead of int. */

    if (addr % 4 != 0){
    /* if ad address is word-aligned, */
    /* then the addr could be perfertly divided by 4. */
        addr_alignment_incorrect();
        /* if it could not be perfectly divided by 4 */
        /* then addr is not word-aligned. */
        return -1;
    }
   if (table -> mode == SYMBOLTBL_UNIQUE_NAME){
    /* the table mode is 1, which means the table could only hold unique names */
      for (i = 0; i < table -> len; i++){
        /* we need to check every element in the table */
        /* to find whether there is the name we want to find */
        if (strcmp(table -> tbl[i].name, name) == 0){
          /* the name is already existed */
          name_already_exists(name);
          /* report the existed name is existed. */
          return -1;
        }
      }
    }
    /* Here, it means that we have passed the validation test */
    /* it means the name is new to the table */
    /* we should add the new element to the table */
   
    table -> tbl = (Symbol*) realloc(table -> tbl, (table -> len + 1) * sizeof(Symbol));
    /* Firstly, we need to reallocate the table and extend the size to (len + 1) Symbols */
    /* In order to store one more element. */
    if (table -> tbl == NULL) allocation_failed();
    /* call the function if we fail to get the mem. */

    table -> tbl[table -> len].name = malloc(sizeof(char));
    /* allocate mem for name in the table. */
    if (table -> tbl[table -> len].name == NULL) allocation_failed();
    /* call the function if we fail to get the mem. */

    strcpy(table -> tbl[table -> len].name, name);
    /* Copy the name into the name, for the name may lose */
    table -> tbl[table -> len].addr = addr;
    /* add the address to the symbol table. */

    table -> len += 1;
    /* extend length one step larger */
    return 0;
}

/* Returns the address (byte offset) of the given symbol. If a symbol with name
   NAME is not present in TABLE, return -1.
 */
int64_t get_addr_for_symbol(SymbolTable* table, const char* name) {
    uint32_t i = 0;
    /* use as a uint32_t var as a temp var*/
    /* for table -> len is a uint32_t var, */
    /* we have to use uint32_t instead of int. */

    for (i = 0; i < table -> len; i++){
      /* explore the whole table */
        if (strcmp(table -> tbl[i].name, name) == 0){
          /* if target name meets with one of the name in the tbl */
          return table -> tbl[i].addr;
          /* return the address. */
        }
    }   

    return -1;
}

/* Writes the SymbolTable TABLE to OUTPUT. You should use write_sym() to
   perform the write. Do not print any additional whitespace or characters.
 */
void write_table(SymbolTable* table, FILE* output) {
    uint32_t i = 0;
    /* use as a uint32_t var as a temp var*/
    /* for table -> len is a uint32_t var, */
    /* we have to use uint32_t instead of int. */

    for (i = 0; i < table -> len; i++) write_sym(output, table -> tbl[i].addr, table -> tbl[i].name);
    /* print out all the elements in the table */
}
