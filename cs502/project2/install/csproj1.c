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
/*int plugin_is_GPL_compatible;
// Counters for each of the staticstics
int function_counter = 0;  // Global varibale that keeps the number of functions
int global_vars_counter = 0;  // Global variable that keeps the number of global variables
int local_vars_counter = 0;  // Global varibale that keeps the number of local variables
int num_of_stmts = 0;  // Global variable that keeps the number of statements
char * temp2;
char *temp;
char *temp3;

// Count local variables
static void local_var_counter(tree t)
{
    enum tree_code code = TREE_CODE(t);
    if(!strcmp(tree_code_name[(int)code],"decl_expr") ) // DECL_EXPR represent a local declaration
    {
        local_vars_counter++;
    }

}
// Parse Tree Recursively
static void parse_tree(tree t, void (*callback)(tree t))
{

// Tree is null return
    if(t == 0)
        return;
    (*callback)(t);
// if tree code is STATEMENT_LIST call the function recursively for all subtrees.
    if(TREE_CODE(t) == STATEMENT_LIST)  // Check the code for the current tree is a STATEMENTLIST or Not
    {
        tree_stmt_iterator it;    // iterator for statement

        for(it = tsi_start(t); !tsi_end_p(it); tsi_next(&it))
        {

            parse_tree(tsi_stmt(it), callback);   // call parse tree function for each of the statements in the statement list.

        }

        return;
    }

    if(DECL_P(t) || EXCEPTIONAL_CLASS_P(t) || CONSTANT_CLASS_P(t))
    {
        return;
    }


    enum tree_code code2 = TREE_CODE(t);
    temp = tree_code_name[(int)code2];

    if(!strcmp(temp, "return_expr"))   // Check the code name is return_expr if yes then count the return as a statement.
    {

        num_of_stmts++;
        return;
    }
    if(!strcmp(temp, "call_expr")) // Check the code name is a call_expr or not if it is function call should be counted as a statement.

    {
        num_of_stmts++;
        return;

    }
    if(!strcmp(temp, "modify_expr"))   // MODIFY_EXPR represent assignment. In this part of code we count each of the assignment as a statement.
    {
        num_of_stmts++;
        return;

    }
    if(!strcmp(temp, "cond_expr")) // This part is used in order to count if statements
    {
        temp2 = tree_code_name[TREE_CODE(TREE_OPERAND(t,1))];
        if(TREE_OPERAND(t,2) != NULL)
        {
            temp3 = tree_code_name[TREE_CODE(TREE_OPERAND(t,2))];

            if(!strcmp(temp2, "goto_expr") && !strcmp(temp3, "goto_expr")) //  This part is used to filter conditions that are in the while condition.
            {

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

    }
    if(strcmp(temp,"cond_expr"))
        parse_tree(TREE_OPERAND(t,0), callback);  // First child of tree is send as a tree to parse function (Recursive call)

    if(UNARY_CLASS_P(t))
        return;
    enum tree_code code = TREE_CODE(t);   // return the code for a tree

    if(code != RETURN_EXPR && code != LABEL_EXPR && code != GOTO_EXPR
            && code != NOP_EXPR && code != DECL_EXPR && code !=ADDR_EXPR
            && code != INDIRECT_REF && code != COMPONENT_REF && code !=PREDICT_EXPR)
    {
        parse_tree(TREE_OPERAND(t,1), callback);   // The second child of the current tree is send to the parse_tree (recursive call)

    }

    if(code == COND_EXPR)
    {
        parse_tree(TREE_OPERAND(t,2), callback);  // If the code is COND_EXPR then it is possible that it has a else body as well. So we need to call the third child of the tree which represents the else.

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

static void function_parse(tree fundecl)
{
// Check TREE_CODE is a function declration or not
    if(TREE_CODE(fundecl) == FUNCTION_DECL)
    {
        tree id = DECL_NAME(fundecl);

        function_counter++;  // count number of functions

        // Check TREE_CODE is a function body
        tree funbody = DECL_SAVED_TREE(fundecl);

        if(TREE_CODE(funbody) == BIND_EXPR)       // local block of a function
        {
            tree t = TREE_OPERAND(funbody, 1);   // The second child of Function body
            tree t_decl = TREE_OPERAND(funbody,2);  // The third child of function body

            parse_tree(t_decl, local_var_counter);  // call parse tree function for third child of function body
            parse_tree(t, local_var_counter);   // call parse tree function for the second child of function body
        }

    }

}
// Function for extracting global variables
void global_vars_parse()
{

    // this structure is used to keep global varibales.
    struct varpool_node *var_node;
    tree vardecl;
    for (var_node = varpool_nodes; var_node; var_node = var_node->next)
    {
        vardecl = var_node->decl;
        global_vars_counter++;
        num_of_stmts++;
        parse_tree(vardecl, local_var_counter);

    }

}*/

void cs502_proj1()
{

    // Function for Extracting global variables
    /* global_vars_parse();

     // Extract function declration and function body from the AST
     struct cgraph_node *node;
     for (node = cgraph_nodes; node; node = node->next)
     {
         tree fundecl;
         fundecl = node->decl;
         function_parse(fundecl);  // Call function parse in order to count the number of functions

     }
     FILE *fd = fopen("output.txt", "w");
     if(fd == NULL)
     {
         printf("File can not be opend!\n");
         exit(1);
     }
    // Write the output into a text file.
     fprintf(fd,"#functions: %d\n", function_counter);
     fprintf(fd,"#global vars: %d\n", global_vars_counter);
     fprintf(fd,"#local vars: %d\n", local_vars_counter);
     fprintf(fd,"#statements: %d\n",num_of_stmts);
     fclose(fd);*/

}


