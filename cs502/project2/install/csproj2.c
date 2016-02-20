#include <stdio.h>
#include "config.h"
#include "system.h"
#include "coretypes.h"
#include "tm.h"
#include "tree.h"
#include "cgraph.h"
#include "hashtab.h"
#include "langhooks.h"
#include "tree-iterator.h"
#include "cp/cp-tree.h"
#include "gcc-plugin.h"
#include "plugin-version.h"
#include "tree-iterator.h"
int plugin_is_GPL_compatible;
// Counters for each of the staticstics
int function_counter = 0;  // Global varibale that keeps the number of functions
int global_vars_counter = 0;  // Global variable that keeps the number of global variables
int local_vars_counter = 0;  // Global varibale that keeps the number of local variables
int num_of_stmts = 0;  // Global variable that keeps the number of statements
char * temp2;
char *temp;
char *temp3;
char *caller;
char **function_names;
char **function_order;
char **function_order_clean;
int order_counter = 0;
// a linked list for keeping the information for each data variable.
struct node_var
{
    char * function_name;
    char * variable;
    int value;
    int level;
    int isglobal;
    struct node_var *next;

};


// A map data structure to map each argument inf a function to parameter in a function
struct pointer_map
{
    char *function_name;
    char *caller;
    int type;
    char *param;
    char *argument;


};
int map_counter = 0;
int map_counter2 = 0;
struct pointer_map **pointer_map;

//int flag_while = 0;
// Defining structures
struct node_var *mainDef = NULL;
struct node_var *result_local = NULL;
struct node_var *mainGlobal = NULL;
//struct node_var *mainDecl = NULL;
struct node_var *result_global = NULL;


// Linked List functions
// Searching a list to see there is a specefic variable or not
//static struct node_var * search_llist(struct node_var* llist, char *variable, char * function_name, int level);
// Adding an element to a linked list
//static struct node_var * add_llist(struct node_var* llist, char *variable, char * function_name, int level);
// Create a linked list
//static struct node_var * create_llist(struct node_var* llist, char *variable, char * function_name, int level);
// print the linked list
//static  void print_llist(struct node_var* llist);

// This function is used to create a linked list and return a list
static struct node_var* create_llist(struct node_var * llist, char *variable, char * function_name, int level)
{
    struct node_var *ptr_llist = (struct node *)xmalloc(sizeof(struct node_var));
    if(ptr_llist == NULL)
    {
        printf("The list can not be created\n");
        return NULL;

    }

    ptr_llist->variable = variable;
    ptr_llist->function_name = function_name;
    ptr_llist->next = NULL;

    ptr_llist->value = 0;
    ptr_llist->level = level;


    llist = ptr_llist;
    return llist;

}


static struct node_var * add_llist_end(struct node_var *first_list, struct node_var *second_list)
{

    if(second_list == NULL)
        return;

    struct node_var  *temp = first_list;
    while(temp->next !=NULL)
    {
        temp = temp->next;


    }
    temp->next = second_list;

    return temp;



}

// This function is used to add nodes to the list.
static struct node_var * add_llist(struct node_var* llist, char * variable, char * function_name, int level)
{

    if(llist == NULL)
    {
        llist = create_llist(llist,variable, function_name, level);
        return llist;

    }

    struct node_var *ptr_llist = llist;
    struct node_var *ptr_node = (struct node_var *)xmalloc(sizeof(struct node_var));

    if(ptr_node == NULL)
    {
        printf("Can not create a node var\n");
        return NULL;
    }
    while(ptr_llist->next !=NULL)
    {
        ptr_llist = ptr_llist->next;

    }

    ptr_node->variable = variable;
    ptr_node->function_name = function_name;
    ptr_node->next = NULL;
    ptr_node->value = 0;
    ptr_node->level = level;


    ptr_llist->next = ptr_node;


    return llist;


}

// search inside of the list
static struct node_var * search_llist(struct node_var *llist, char *variable, char * function_name, int level)
{
    struct node_var *ptr_llist = llist;

    while(ptr_llist !=NULL)
    {
        if(!strcmp(ptr_llist->variable, variable) && !strcmp(ptr_llist->function_name, function_name))
        {

            return ptr_llist;

        }
        else
            ptr_llist = ptr_llist->next;

    }

    return NULL;
}

static struct node_var * search_llist_case(struct node_var *llist, char *variable, char * function_name, int level)
{
    struct node_var *ptr_llist = llist;

    while(ptr_llist !=NULL)
    {
        if(!strcmp(ptr_llist->variable, variable) && !strcmp(ptr_llist->function_name, function_name) && ptr_llist->level == level )
        {

            return ptr_llist;

        }
        else
            ptr_llist = ptr_llist->next;

    }

    return NULL;
}



// This function is used to print a linked list
static void print_llist(struct node_var *llist)
{

    struct node_var *ptr_llist = llist;

    while(ptr_llist !=NULL)
    {

        //printf("%s:%s:%d\n", ptr_llist->function_name, ptr_llist->variable ,ptr_llist->value);
        //ptr_llist = ptr_llist->next;

    }


}
//Search a speecfic node in the linked list (Check the function name and name of the variable at the same time)
static int search_list_node(struct node_var *llist, char *variable, char *function_name)
{
    struct node_var *ptr_llist = llist;

    while(ptr_llist !=NULL)
    {
        if(!strcmp(ptr_llist->variable, variable) && !strcmp(ptr_llist->function_name, function_name))
        {

            return 1;

        }
        else
            ptr_llist = ptr_llist->next;

    }

    return 0;


}
// wirte  the Local and global uninitialized variables into output.txt
static void print_llist_local(struct node_var * llist, struct node_var *global_llist)
{
    struct node_var *ptr_llist = llist;
    struct node_var  *ptr_llist_clean;
    ptr_llist_clean = (struct node_var *)xmalloc(sizeof(struct node_var));
    ptr_llist_clean->function_name = "NULL";
    ptr_llist_clean->variable = "NULL";
    ptr_llist_clean->level = "NULL";
    ptr_llist_clean->next = NULL;
    struct node_var *ptr_to_clean2 = ptr_llist_clean;
    while(ptr_llist !=NULL)
    {

        int flag =search_list_node(ptr_llist_clean,ptr_llist->variable,ptr_llist->function_name);
        if(flag == 0)
            ptr_llist_clean = add_llist(ptr_llist_clean,ptr_llist->variable,ptr_llist->function_name,ptr_llist->level);
        ptr_llist = ptr_llist->next;

    }



    struct node_var *ptr_to_clean = ptr_llist_clean;
    int counter = 0;
    int i;
    int flag = 0;

    FILE *fd = fopen("output.txt", "w");
    int order = 0;
    int j;
    int flag_order = 0;
    for(i=0; i < order_counter; i++)
    {

        flag_order = 0;
        for(j = 0; j< order; j++)
        {
            if(!strcmp(function_order[i],function_order_clean[j]))
            {
                flag_order = 1;

            }


        }
        if(flag_order == 0)
        {
            function_order_clean[order] = function_order[i];
            order++;

        }

    }


    for(i=0; i < order; i++)
    {
        counter =0;
        ptr_to_clean = ptr_llist_clean;
        while(ptr_to_clean != NULL)
        {

            if((!strcmp(ptr_to_clean->function_name,function_order_clean[i])) && counter == 0 && flag == 0)
            {
                fprintf(fd,"%s:%s",ptr_to_clean->function_name,ptr_to_clean->variable);
                counter = 1;
                flag = 1;
            }
            else if((!strcmp(ptr_to_clean->function_name,function_order_clean[i])) && counter == 0 && flag == 1)
            {

                fprintf(fd,"\n%s:%s",ptr_to_clean->function_name,ptr_to_clean->variable);
                counter = 1;

            }
            else if((!strcmp(ptr_to_clean->function_name,function_order_clean[i])) && counter == 1)
            {
                fprintf(fd,",%s",ptr_to_clean->variable);

            }


            ptr_to_clean = ptr_to_clean->next;
        }


    }

    struct node_var *ptr_llist_global = global_llist;
    //struct node_var *ptr_llist_global_clean = global_llist;






    counter = 0;
    while(ptr_llist_global !=NULL)
    {

        if(counter == 0 && flag == 0)
        {
            fprintf(fd,"Global:%s", ptr_llist_global->variable);
            counter = 1;

        }
        else if(counter == 0 && flag == 1)
        {
            fprintf(fd,"\nGlobal:%s", ptr_llist_global->variable);
            counter = 1;


        }
        else if(counter == 1)
        {

            fprintf(fd,",%s", ptr_llist_global->variable);

        }
        ptr_llist_global = ptr_llist_global->next;

    }
    fclose(fd);

}


static void print_llist_global(struct node_var *llist)
{
    struct node_var *ptr_llist = llist;
    int counter  = 0;
    while(ptr_llist !=NULL)
    {

        printf("%s:%s\n", "Global", ptr_llist->variable);
        ptr_llist = ptr_llist->next;

    }

}

// Print the function names
void print_function_names()
{
    int i;
    i = 0;

    for(i; i< function_counter; i++)
    {
        printf("Function name:%s\n", function_names[i]);

    }

    for(i = 0; i < order_counter; i++)
    {
        printf("order:%s\n", function_order[i]);

    }

}



// Copy on linked list to another and return a pointer to new one.
static struct node_var *copy_llist(struct node_var *llist)
{

    if(llist == NULL)
        return;
    struct node_var *temp = (struct node_var *)xmalloc(sizeof(struct node_var));

    temp->variable = llist->variable;
    temp->function_name = llist->function_name;

    temp->value = llist->value;
    temp->level = llist->level;
    temp->next = copy_llist(llist->next);
    return temp;

}
// Function for printing the map
void print_map()
{
    int i;
    printf("-------------------print map---------------\n");
    for(i=0; i<map_counter; i++)
    {
        printf("%s:%s:%s:%s:%d\n",pointer_map[i]->caller,pointer_map[i]->function_name,pointer_map[i]->param, pointer_map[i]->argument, pointer_map[i]->type);

    }
    printf("-----------------------------------------------\n");


}


// Count local variables
static void local_var_counter(tree t)
{
    enum tree_code code = TREE_CODE(t);
    temp = tree_code_name[(int)code];

    if(!strcmp(temp,"decl_expr")) // DECL_EXPR represent a local declaration
    {
        local_vars_counter++;

    }


}
// Parse Tree Recursively
static void parse_tree(tree t, void (*callback)(tree t), char * function_name, struct node_var * blockDef, int *flag_while, int level)
{

// Tree is null then  return
    if(t == 0)
        return;

    struct node_var *ptr_llist;

    enum tree_code code2 = TREE_CODE(t);
    temp = tree_code_name[(int)code2];

    if(!strcmp(temp, "nop_expr"))
    {


    }
    // Handling var_decl
    if(!strcmp(temp, "var_decl"))
    {
        tree id = DECL_NAME(t);
        const char *var_name = id ? IDENTIFIER_POINTER(id): "<unnamed>";

        // Check in the list of variables that are defined locally. if we can find it it is OK and then return.
        ptr_llist = search_llist(blockDef, var_name, function_name, level);
        if(ptr_llist != NULL)
            return;


        ptr_llist = search_llist(mainDef, var_name, function_name,0);
        if(ptr_llist !=NULL)
            return;

        // Check in the list of the global variables that have defined locally.
        ptr_llist = search_llist(blockDef, var_name, "global", level);
        if(ptr_llist != NULL)
            return;
        // If it is a global variable, the value paramter should be set to one that indicates it is initialized
        ptr_llist = search_llist(mainGlobal, var_name, "global",0);
        if(ptr_llist !=NULL)
        {
            if(ptr_llist->value == 1)
                return;
            else
            {

                if(search_llist(result_global, var_name, function_name,0) == NULL)
                {
                    // The variable is global and should be added to the list of uninitialized global variables
                    if(search_llist(result_global, var_name, "global", level) == NULL)
                        result_global = add_llist(result_global, var_name,"global", level);
                }

            }
        }
        else
        {
            // The variable is a not global and should be added to the list of unitialized local variables.

            result_local = add_llist(result_local, var_name, function_name, level);

        }

        return;
    }
// if tree code is STATEMENT_LIST call the function recursively for all subtrees.

    if(TREE_CODE(t) == STATEMENT_LIST)  // Check the code for the current tree is a STATEMENTLIST or Not
    {
        //struct node_var *blockDefWhile = copy_llist(blockDef);
        struct node_var ** blockDefWhile = (struct node_var *)xmalloc(sizeof(struct node_var *) * 40);
        int while_count = 0;
        blockDefWhile[0] = blockDef;
        while_count++;

        struct node_var *blockDefWhile_new = blockDefWhile[0];

        tree_stmt_iterator it;    // iterator for statement

        for(it = tsi_start(t); !tsi_end_p(it); tsi_next(&it))
        {

            char *stmt_code = tree_code_name[TREE_CODE(tsi_stmt(it))];

            if(!strcmp(stmt_code, "goto_expr"))
            {
                tsi_next(&it);
                if(tsi_end_p(it))
                    break;

                if(TREE_CODE(tsi_stmt(it)) == LABEL_EXPR)
                {

                    struct node_var *temp = copy_llist(blockDefWhile[while_count - 1]);
                    blockDefWhile[while_count] = temp;
                    blockDefWhile_new = temp;
                    while_count++;
                    continue;

                }

            }

            else if(TREE_CODE(tsi_stmt(it)) == LABEL_EXPR)
            {

                tsi_next(&it);
                if(tsi_end_p(it))
                    break;
                if(TREE_CODE(tsi_stmt(it)) == COND_EXPR)
                {
                    tree t2 = tsi_stmt(it);

                    tsi_next(&it);
                    if(tsi_end_p(it))
                        break;
                    if(TREE_CODE(tsi_stmt(it)) == LABEL_EXPR)
                    {
                        while_count--;
                        blockDefWhile_new = blockDefWhile[while_count - 1];
                        parse_tree(t2, callback, function_name, blockDefWhile_new, flag_while, level);
                        continue;


                    }


                }


            }

            if(tsi_end_p(it))
                break;
            parse_tree(tsi_stmt(it), callback, function_name, blockDefWhile_new, flag_while, level);


        }

        return;
    }


    if(!strcmp(temp, "float_expr"))
    {

        parse_tree(TREE_OPERAND(t,0), callback, function_name,blockDef, flag_while, level);
        return;

    }
    if(DECL_P(t) || EXCEPTIONAL_CLASS_P(t) || CONSTANT_CLASS_P(t))
    {
        return;
    }
// Handling switch case here
    if(!strcmp(temp, "switch_expr"))
    {

        parse_tree(TREE_OPERAND(t,0), callback, function_name, blockDef, flag_while, level);
        tree body_switch = TREE_OPERAND(t,1);
        const char *body_name = tree_code_name[TREE_CODE(body_switch)];
        if(!strcmp(body_name, "statement_list"))
        {
            // The function that is written to support switch case.
            case_handler1(body_switch, function_name, level, blockDef,flag_while);

        }


        else if(!strcmp(body_name,"bind_expr"))
        {
            tree body_switch_op1 = TREE_OPERAND(body_switch,1);
            case_handler1(body_switch_op1, function_name, level, blockDef,flag_while);

        }
        return;

    }
// if it is BIND_EXPR call the second and third child to parse
    if(!strcmp(temp, "bind_expr"))
    {

        parse_tree(TREE_OPERAND(t,1), callback, function_name, blockDef, flag_while, level);
        parse_tree(TREE_OPERAND(t,2), callback, function_name, blockDef, flag_while, level);
        return;

    }


    if(!strcmp(temp, "modify_expr"))
    {

        // Call the second child of MODFIY_EXPR tree to extract variables in the right hand side.
        parse_tree(TREE_OPERAND(t,1), callback, function_name, blockDef, flag_while, level);

        tree var_modify = TREE_OPERAND(t,0);
        tree id = DECL_NAME(var_modify);
        const char *var_name;
        // Checking the left hand side is indirect ref or not
        if(!strcmp(tree_code_name[TREE_CODE(var_modify)], "parm_decl"))
        {
            return;
        }
        else if(!strcmp(tree_code_name[TREE_CODE(var_modify)], "indirect_ref"))
        {
            tree op0_var_modify = TREE_OPERAND(var_modify,0);
            tree id_op0 = TREE_OPERAND(op0_var_modify,0);
            var_name = id_op0 ? IDENTIFIER_POINTER(id_op0):"<unnamed>";

        }
        else
        {
            id = DECL_NAME(var_modify);
            var_name = id ? IDENTIFIER_POINTER(id):"<unnamed>";

        }

        int i;
        // Here I handle the pointer case. If it is a pointer and it is initialized I add it to the mainDef


        i = map_counter - 1;
        while(i>=0)
        {
            if(!strcmp(pointer_map[i]->argument,var_name) && !strcmp(pointer_map[i]->function_name, function_name) )
            {
                mainDef = add_llist(mainDef, pointer_map[i]->param, pointer_map[i]->caller, level);

                break;
            }

            i--;

        }
        // Check that the variable is a global or not if it is global we add it to blockDef with global label.
        ptr_llist = search_llist(mainGlobal, var_name, "global", 0);
        if(ptr_llist!=NULL)
        {

            blockDef = add_llist(blockDef, var_name, "global", level);
            return;
        }

        // if it is not a global and it is defined add it to blockDef.
        blockDef = add_llist(blockDef, var_name, function_name, level);
        return;
    }


    if(!strcmp(temp,"decl_expr")) // DECL_EXPR represent a local declaration
    {

        tree var = TREE_OPERAND(t,0);
        tree id = DECL_NAME(var);
        copy_llist(blockDef);
        const char *var_name = id ? IDENTIFIER_POINTER(id): "<unnamed>";
        // Check the declration is initilized or not if it is initialized add to the blockDef.
        if(DECL_INITIAL(TREE_OPERAND(t,0)))
        {
            parse_tree(DECL_INITIAL(TREE_OPERAND(t,0)), callback, function_name, blockDef, flag_while, level);
            add_llist(blockDef, var_name, function_name, level);


        }
        return;
    }

    if(!strcmp(temp, "return_expr"))   // Check the code name is return_expr if yes then count the return as a statement.
    {
        // Call parser for first opernad of return tree in order to extract the variables inside of return (if there is a experission)
        parse_tree(TREE_OPERAND(t,0), callback, function_name, blockDef, flag_while, level);
        num_of_stmts++;
        return;
    }


    if(!strcmp(temp, "call_expr")) // Check the code name is a call_expr or not if it is function call should be counted as a statement.
    {
        tree addr_node;
        addr_node = TREE_OPERAND(t,1);
        code2 = TREE_CODE(addr_node);

        temp2 = tree_code_name[(int)code2];
        if(!strcmp(temp2, "addr_expr"))
        {
            tree decl_node = TREE_OPERAND(addr_node,0);
            code2 = TREE_CODE(decl_node);
            temp2 = tree_code_name[(int)code2];
            if(!strcmp(temp2, "function_decl"))
            {
                // This part of the code is writtne in order to generate a map between the paramters that should be send to a function
                // and function arguments that have been defined locally. I have used a map to differentiate pointer variables from non pointer variables
                // to handle pointer part. pointer_map is a data strucutre that is used to keep the mapping between each function argument and paramter.
                temp2 = IDENTIFIER_POINTER(DECL_NAME(decl_node));
                if(strcmp(temp2,"printf") && strcmp(temp2, "scanf"))
                {


                    tree arg;
                    call_expr_arg_iterator iter;
                    FOR_EACH_CALL_EXPR_ARG(arg,iter,t)
                    {
                        const char *argname = tree_code_name[TREE_CODE(arg)];
                        tree type_arg = TREE_TYPE(arg);

                        if(TREE_CODE(type_arg) != POINTER_TYPE)
                        {

                            parse_tree(arg, callback, function_name, blockDef, flag_while, level);
                        }
                        if(TREE_CODE(arg) != INTEGER_CST && TREE_CODE(arg)!=REAL_CST && TREE_CODE(arg)!=STRING_CST)
                        {
                            tree id = DECL_NAME(arg);

                            if(TREE_CODE(id) == IDENTIFIER_NODE)
                            {

                                const char *var_name = id ? IDENTIFIER_POINTER(id): "<unnamed>";
                                tree type_var = TREE_TYPE(arg);

                                const char * type = tree_code_name[TREE_CODE(type_var)];
                                pointer_map[map_counter]->function_name = temp2;
                                if(!strcmp(type,"pointer_type"))
                                    pointer_map[map_counter]->type = 1;
                                else
                                    pointer_map[map_counter]->type = 0;
                                pointer_map[map_counter]->param = var_name;
                                pointer_map[map_counter]->caller = function_name;

                            }
                            else if(TREE_CODE(id) == VAR_DECL)
                            {

                                tree id2 = DECL_NAME(id);
                                const char *var_name = id2 ? IDENTIFIER_POINTER(id2): "<unnamed>";
                                tree type_var = TREE_TYPE(arg);
                                const char * type = tree_code_name[TREE_CODE(type_var)];
                                pointer_map[map_counter]->function_name = temp2;
                                if(!strcmp(type,"pointer_type"))
                                    pointer_map[map_counter]->type = 1;
                                else
                                    pointer_map[map_counter]->type = 0;
                                pointer_map[map_counter]->param = var_name;
                                pointer_map[map_counter]->caller = function_name;


                            }
                            else if(TREE_CODE(id) == ADDR_EXPR)
                            {
                                tree op0 = TREE_OPERAND(id,0);
                                tree id2 = DECL_NAME(op0);
                                const char *var_name = id2 ? IDENTIFIER_POINTER(id2): "<unnamed>";
                                tree type_var = TREE_TYPE(arg);
                                const char * type = tree_code_name[TREE_CODE(type_var)];
                                pointer_map[map_counter]->function_name = temp2;
                                if(!strcmp(type,"pointer_type"))
                                    pointer_map[map_counter]->type = 1;
                                else
                                    pointer_map[map_counter]->type = 0;
                                pointer_map[map_counter]->param = var_name;
                                pointer_map[map_counter]->caller = function_name;

                            }
                        }


                        map_counter++;
                    }
                    struct cgraph_node *node1;
                    //Extracting Paramters of a function
                    for(node1 = cgraph_nodes; node1; node1 = node1->next)
                    {

                        tree fundecl1;
                        fundecl1 = node1->decl;

                        const char *function_name1 = IDENTIFIER_POINTER(DECL_NAME(fundecl1));
                        if(!strcmp(temp2, function_name1))
                        {
                            tree args = DECL_ARGUMENTS(fundecl1);
                            while(args)
                            {

                                const char *name = tree_code_name[TREE_CODE(args)];
                                if(!strcmp(name,"parm_decl"))

                                {
                                    tree type_var = TREE_TYPE(args);
                                    const char * type = tree_code_name[TREE_CODE(type_var)];
                                    //printf("parm:%s:%s:%s\n", temp,IDENTIFIER_POINTER(DECL_NAME(args)), type);
                                    pointer_map[map_counter2]->argument = IDENTIFIER_POINTER(DECL_NAME(args));
                                }
                                map_counter2++;
                                args = TREE_CHAIN(args);


                            }
                        }

                    }
                    struct node_var * calleeBlockDef =  copy_llist(blockDef);
                    // When we reach to a function call we traverse the function to consider the flow of the program in our analysis.
                    function_order[order_counter] = temp2;
                    order_counter++;
                    parse_function_call(temp2, calleeBlockDef);

                    struct node_var * temp_blockDef = calleeBlockDef;

                    while(temp_blockDef !=NULL)
                    {

                        if(!strcmp(temp_blockDef->function_name,"global"))
                        {


                            if(search_llist(blockDef, temp_blockDef->variable,temp_blockDef->function_name, level) == NULL)
                            {

                                blockDef = add_llist(blockDef, temp_blockDef->variable, temp_blockDef->function_name,level );
                            }



                        }
                        temp_blockDef = temp_blockDef->next;

                    }


                }
                // Extracting the arguments of printf function.
                else if(!strcmp(temp2, "printf"))
                {
                    tree arg;
                    call_expr_arg_iterator iter;
                    FOR_EACH_CALL_EXPR_ARG(arg,iter,t)
                    {
                        const char *argname = tree_code_name[TREE_CODE(arg)];
                        parse_tree(arg, callback, function_name, blockDef, flag_while, level);

                    }

                }

                // Extracting arguments of scanf function.
                else if(!strcmp(temp2, "scanf"))
                {

                    tree arg;
                    call_expr_arg_iterator iter;
                    // iterate through all of the arguments of scanf functions.
                    FOR_EACH_CALL_EXPR_ARG(arg,iter,t)
                    {
                        tree temp = TREE_OPERAND(arg,0);

                        if(TREE_CODE(temp) == VAR_DECL)
                        {
                            tree id = DECL_NAME(temp);
                            const char *var_name = id ? IDENTIFIER_POINTER(id):"<unnamed>";

                            if(search_llist(blockDef, var_name,function_name,level) == NULL)
                            {
                                blockDef = add_llist(blockDef, var_name, function_name, level);
                            }

                        }

                    }

                }


            }


        }

        num_of_stmts++;
        return;

    }

    // Differentiate GOTO_EXPR for different things such as while loop, break and continue.

    if(!strcmp(temp, "goto_expr"))
    {

        return;

    }
    // Handling the condition expr for detecting uninitialized variables.
    if(!strcmp(temp, "cond_expr")) // This part is used in order to count if statements
    {
        temp2 = tree_code_name[TREE_CODE(TREE_OPERAND(t,1))];

        if(TREE_OPERAND(t,2) != NULL)
        {
            temp3 = tree_code_name[TREE_CODE(TREE_OPERAND(t,2))];

            if(!strcmp(temp2, "goto_expr") && !strcmp(temp3, "goto_expr")) //  This part is used to filter conditions that are in the while condition.
            {
                parse_tree(TREE_OPERAND(t,0), callback, function_name, blockDef, flag_while, level);
                return;
            }

            else
            {

                num_of_stmts++;

            }
        }
        else
        {
            num_of_stmts++;

        }


        ////// handling IF else condition to detect uninitalized variables //////////

        struct node_var *blockDefIf = copy_llist(blockDef);
        struct node_var *blockDefElse = copy_llist(blockDef);

        parse_tree(TREE_OPERAND(t,0), callback, function_name, blockDef, flag_while, level);
        parse_tree(TREE_OPERAND(t,1), callback, function_name, blockDefIf, flag_while, level + 1);
        parse_tree(TREE_OPERAND(t,2), callback, function_name, blockDefElse, flag_while, level + 1);


        struct node_var * temp1 = blockDefIf;
        struct node_var *temp2 = blockDefElse;

        while(temp1 !=NULL)
        {
            if(search_llist(blockDefElse,temp1->variable, temp1->function_name, level+1) !=NULL)
            {
                if(search_llist(blockDef, temp1->variable,temp1->function_name, level) == NULL)
                {
                    blockDef = add_llist(blockDef, temp1->variable,temp1->function_name, level);

                }

            }
            temp1 = temp1->next;

        }

        return;

    }

    parse_tree(TREE_OPERAND(t,0), callback, function_name,blockDef, flag_while, level );  // First child of tree is send as a tree to parse function (Recursive call)

    if(UNARY_CLASS_P(t))
        return;
    enum tree_code code = TREE_CODE(t);   // return the code for a tree

    if(code != RETURN_EXPR && code != LABEL_EXPR && code != GOTO_EXPR
            && code != NOP_EXPR  && code != DECL_EXPR && code !=ADDR_EXPR
            && code != INDIRECT_REF && code != COMPONENT_REF && code !=PREDICT_EXPR && code!=COND_EXPR)
    {
        parse_tree(TREE_OPERAND(t,1), callback, function_name, blockDef, flag_while, level);   // The second child of the current tree is send to the parse_tree (recursive call)

    }

    // This part of the code is used to count other statements that we don't cover in previous parts such as break, continue, while loop. Basically we filter
// the statements that we counted them before.
    if(code != BIND_EXPR &&  code!= LABEL_EXPR && code!=CASE_LABEL_EXPR
            && code != GT_EXPR && code != NE_EXPR && code != LE_EXPR && code !=EQ_EXPR
            && code !=PLUS_EXPR && code !=MINUS_EXPR && code !=MULT_EXPR && code !=TRUNC_DIV_EXPR
            && code !=ADDR_EXPR  && code != TRUTH_ANDIF_EXPR && code != TRUTH_NOT_EXPR && code != LT_EXPR
            && code != TRUTH_ORIF_EXPR && code!=COND_EXPR && code!= PREDICT_EXPR && code!=TRUNC_MOD_EXPR
            && code!= NEGATE_EXPR  && code!=BIT_AND_EXPR
            && code!=BIT_IOR_EXPR && code!=FIX_TRUNC_EXPR && code!= CALL_EXPR)
    {

        num_of_stmts++;

    }

}


void case_handler1(tree body_switch, char *function_name, int level, struct node_var *localDefSwitch, int flag_while)
{


    tree_stmt_iterator it_case = tsi_start(body_switch);
    tree_stmt_iterator it_case2 = tsi_start(body_switch);
    int default_;
    int index = 0;
    int count = 0;
    int *breaks = xmalloc(sizeof(int) * 40);

    int * caseLen = xmalloc(sizeof(int) * 40);

    while(index < 40)
    {

        caseLen[index] = 0;
        breaks[index] = 0;
        index++;

    }

    for(index = 0; !tsi_end_p(it_case) && TREE_CODE(tsi_stmt(it_case)) != CASE_LABEL_EXPR; tsi_next(&it_case));

    while(1)
    {

        if(tsi_end_p(it_case))
            break;
        if(TREE_CODE(tsi_stmt(it_case)) == CASE_LABEL_EXPR)
        {
            tree def = CASE_LOW(tsi_stmt(it_case));
            if(def == NULL)
                default_ = 1;


        }
        count = count + 1;
        tsi_next(&it_case);

        while((!tsi_end_p(it_case)) && TREE_CODE(tsi_stmt(it_case)) !=CASE_LABEL_EXPR)
        {
            if(TREE_CODE(tsi_stmt(it_case)) == GOTO_EXPR)
            {
                it_case2 = it_case;
                tsi_next(&it_case2);
                if ((!tsi_end_p(it_case2)) && TREE_CODE(tsi_stmt(it_case2)) == CASE_LABEL_EXPR)
                {
                    breaks[index] = 1;


                }


            }
            caseLen[index] = caseLen[index] + 1;
            tsi_next(&it_case);

        }
        index++;

    }

    for (it_case = tsi_start(body_switch); !tsi_end_p(it_case) && TREE_CODE(tsi_stmt(it_case)) != CASE_LABEL_EXPR; tsi_next(&it_case))
    {


    }


    struct node_var **localScopeArray = (struct node_var *)xmalloc(sizeof(struct node_var *) * 40);

    for(index = 0 ; index < count; index++)
    {
        int caseIndex;
        tsi_next(&it_case);
        struct node_var * localScopeCase = copy_llist(localDefSwitch);

        for(caseIndex = 0; caseIndex < caseLen[index]; caseIndex++)
        {

            parse_tree(tsi_stmt(it_case), local_var_counter,function_name, localScopeCase, flag_while, level + 1);

            tsi_next(&it_case);

        }

        localScopeArray[index] = localScopeCase;

    }

    int tmp_counter;
    struct node_var *copy_list;

    for(tmp_counter = count - 2; tmp_counter>=0; tmp_counter--)
    {

        if(breaks[tmp_counter] == 0)
        {
            copy_list = copy_llist(localScopeArray[tmp_counter + 1]);
            add_llist_end(localScopeArray[tmp_counter], copy_list);
        }

    }


/// Checking the last case in the switch is default or not
    if(default_ == 1)
    {
        struct node_var *temp2;
        temp2 = localScopeArray[0];
        while(temp2 !=NULL)
        {
            int flag = 1;
            for(index = 1; index < count; index++)
                if(search_llist(localScopeArray[index], temp2->variable, temp2->function_name, temp2->level) == NULL)
                {

                    flag = 0;
                    break;


                }

            if(flag == 1 && search_llist(localDefSwitch, temp2->variable, temp2->function_name, temp2->level)== NULL)
            {
                add_llist(localDefSwitch, temp2->variable, temp2->function_name, temp2->level);

            }
            temp2 = temp2->next;


        }
    }

}


// This function is called for the main function.
static void function_parse(tree fundecl)
{
// Check TREE_CODE is a function declration or not
    struct node_var *temp = (struct node_var *)xmalloc(sizeof(struct node_var));
    temp->variable = "??";
    temp->function_name = "??";
    temp->value = 0;
    temp->isglobal = 0;
    temp->next = NULL;
    int flag_while = 0;
    if(TREE_CODE(fundecl) == FUNCTION_DECL)
    {
        tree id = DECL_NAME(fundecl);
        char * function_name = IDENTIFIER_POINTER(id);

        // Check TREE_CODE is a function body
        tree funbody = DECL_SAVED_TREE(fundecl);

        if(TREE_CODE(funbody) == BIND_EXPR)       // local block of a function
        {
            tree t = TREE_OPERAND(funbody, 1);   // The second child of Function body
            tree t_decl = TREE_OPERAND(funbody,2);  // The third child of function body

            parse_tree(t_decl, local_var_counter, function_name,temp, &flag_while,0);  // call parse tree function for third child of function body
            parse_tree(t, local_var_counter,function_name,temp, &flag_while,0 );   // call parse tree function for the second child of function body
        }

    }

}
// Function for extracting global variables and save it into a linked list.
void global_vars_parse()
{

    // this structure is used to keep global varibales.
    struct varpool_node *var_node;
    tree vardecl;
    for (var_node = varpool_nodes; var_node; var_node = var_node->next)
    {
        vardecl = var_node->decl;
        tree id = DECL_NAME(vardecl);
        char *glob_name = IDENTIFIER_POINTER(id);
        // Main global is a data structure for keeping the global variables.
        mainGlobal = add_llist(mainGlobal,glob_name, "global", 0);
        tree init = DECL_INITIAL(vardecl);
        if(init)
        {
            struct node_var *ptr_global = mainGlobal;
            while(ptr_global !=NULL)
            {
                if(!strcmp(ptr_global->variable, glob_name))
                {
                    ptr_global->value = 1;

                }
                ptr_global = ptr_global->next;

            }

        }

        global_vars_counter++;
        num_of_stmts++;


    }

}
// Call parse_tree function for each function call in the program.
// This function is used to retrive the control flow graph implicitly.
void parse_function_call(char *function_name2, struct node_var *temp)
{
    struct cgraph_node *node1;

    int flag_while = 0;
    for(node1 = cgraph_nodes; node1; node1 = node1->next)
    {

        tree fundecl1;
        fundecl1 = node1->decl;

        const char *function_name1 = IDENTIFIER_POINTER(DECL_NAME(fundecl1));
        if(!strcmp(function_name2, function_name1))
        {

            tree funbody = DECL_SAVED_TREE(fundecl1);
            if(TREE_CODE(funbody) == BIND_EXPR)
            {
                tree t = TREE_OPERAND(funbody, 1);
                tree t_decl = TREE_OPERAND(funbody ,2);
                parse_tree(t_decl, local_var_counter, function_name1,temp, &flag_while, 0);
                parse_tree(t,local_var_counter,function_name1, temp, &flag_while, 0);

            }


        }

    }


}
// Extract the name of all functions and save them in an array.
void function_names_extraction(tree fundecl)
{

    // Function names extraction and save them into function_names
    if(TREE_CODE(fundecl) == FUNCTION_DECL)
    {
        function_names[function_counter] = IDENTIFIER_POINTER(DECL_NAME(fundecl));
        function_counter++;


    }

}



// Initialize pointer_map and function_names data structures
void init()
{
    // allocating memory to  the data structures such as function_names and pointer_map
    function_names = (char **)xmalloc(100 * sizeof(char *));
    function_order = (char **)xmalloc(100 * sizeof(char *));
    function_order_clean = (char **)xmalloc(100 * sizeof(char *));
    pointer_map = xmalloc(sizeof(struct pointer_map *)* 100);
    int i=0;
    for(i=0; i<100; i++)
    {
        pointer_map[i]= xmalloc(sizeof(struct pointer_map));

    }
}

void cs502_proj2()
{


    init();
    // Function for Extracting global variables and make sure that is initialized or not
    global_vars_parse();

    // Extracting of functon names

    function_order[order_counter] = "main";
    order_counter++;
    struct cgraph_node *node1;
    for(node1 = cgraph_nodes; node1; node1= node1->next)
    {
        tree fundecl1;
        fundecl1 = node1->decl;
        function_names_extraction(fundecl1);

    }


    struct cgraph_node *node;
    node = cgraph_nodes;
    tree fundecl;
    fundecl = node->decl;
    function_parse(fundecl);  // Call function parse to start parsing of the main function


    print_llist_local(result_local, result_global);



}






