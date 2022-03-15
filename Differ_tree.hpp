//******************************************************************************************
//Differentiator by AlexeyPopovMIPT
#pragma once
#ifndef INC_APDIFTR
#define INC_APDIFTR


#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <ctype.h>
#include <string.h>

const int MAX_UNAR_LEN = 4;
const int MAX_CONSTANT_LEN = 2;
const double DBL_PRECISION = 1e-12;

typedef int Types;

const Types VOID = 0,
            UNAR = 1,
            BINAR = 2,
            NUM = 4,
            VAR = 5,
            CONST_E = 7,
            CONST_PI = 8;


typedef int Operations;

const Operations BINAR_add = 1,
                 BINAR_mul = 2,
                 UNAR_recp = 4,
                 BINAR_pow = 8,
                 UNAR_sin = 32,
                 UNAR_cos = 64,
                 UNAR_tan = 128,
                 UNAR_log = 256,
                 UNAR_sinh = 512,
                 UNAR_cosh = 1024,
                 UNAR_tanh = 2048,
                 UNAR_sub = 4096,
                 UNDEFINED = 0;



struct Node
{
    double value;
    Types type;
    int ind_l;
    int ind_r;
};

#define List_elem_t Node*

#include "../../Dvusvyazniy list/Dvusvyazniy list/The List.h"

#ifdef graphvisfile
#undef graphvisfile
#endif
#define graphvisfile "graphvis.dot"

#define TeX_file "derivative.tex"


enum tree_errors
{
    ERR_NO_MEMORY = 1,
    ERR_NO_DEST_NODE = 2,
    SYNTAX_ERROR = 3,
    ERR_UNSUPPORTED = 4
};

struct Tree
{
    Node root;
    Node subroot;
    List_t refs;
};
//-----------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------
#define NOMEM                              \
    printf("ERROR: formula is too big");   \
    return ERR_NO_MEMORY;                  \
//-----------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------
#define lst &(tree->refs)
//-----------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------
#define ENUMERATE_UNAR(oper, _sin, _cos, _log, _sinh, _cosh, _dflt)      \
    if      (strcmp(oper, "sin" ) == 0) {_sin}                           \
    else if (strcmp(oper, "cos" ) == 0) {_cos}                           \
    else if (strcmp(oper, "log" ) == 0) {_log}                           \
    else if (strcmp(oper, "sinh") == 0) {_sinh}                          \
    else if (strcmp(oper, "cosh") == 0) {_cosh}                          \
    else {_dflt}
//-----------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------
#define UNAR_TO_INT(dest, oper)                                                \
    ENUMERATE_UNAR(oper, dest = UNAR_sin;, dest = UNAR_cos;,                   \
    ,dest = UNAR_log;,dest = UNAR_sinh;, dest = UNAR_cosh;, dest = UNDEFINED;)
//-----------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------
#define USE_LVLDOWN_ITERATION Node** son = nullptr; int son_index = 0;
//-----------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------
#define DO_FOR_EACH_SON_excST(node, code)                                                \
    if ((node)->ind_l != -1 && (node)->ind_l != (node)->ind_r) {                         \
    for (son_index  = GetNext(lst, (node)->ind_l);                                       \
         ;                                                                               \
         son_index  = GetNext(lst, son_index))                                           \
    {                                                                                    \
        son = GetByPhInd(lst, son_index);                                                \
        code;                                                                            \
        if (son_index == (node)->ind_r) break;                                           \
    }}
//-----------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------
#define DO_FOR_EACH_SON_excEND(node, code)                                                                 \
    if ((node)->ind_l != -1) {                                                                             \
    for (son_index = (node)->ind_l; son_index != (node)->ind_r; son_index = GetNext(lst, son_index))       \
    {                                                                                                      \
        son = GetByPhInd(lst, son_index);                                                                  \
        code;                                                                                              \
    }}
//-----------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------
#define DO_FOR_EACH_SON(node, code)                      \
    DO_FOR_EACH_SON_excEND(node, code)                   \
    son = GetByPhInd(lst, node->ind_r); if (son) { code }
//-----------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------

inline Node* L(Tree* tree, Node* node) { return *GetByPhInd(lst, node->ind_l); }
#define L(node) L(tree, node)
//-----------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------
inline Node* R(Tree* tree, Node* node) { return *GetByPhInd(lst, node->ind_r); }
#define R(node) R(tree, node)
//-----------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------
#define cutleft(node)                                         \
    {                                                         \
        int temprry = GetNext(lst, (node)->ind_l);            \
        if (temprry == (node)->ind_l ||                       \
            (node)->ind_l == (node)->ind_r)                   \
            temprry = -1;                                     \
        RemoveAtPos(lst, (node)->ind_l, nullptr);             \
        (node)->ind_l = temprry;                              \
    }
//-----------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------
#define cutright(node)                                        \
    {                                                         \
        int temprry = GetPrev(lst, (node)->ind_r);            \
        if (temprry == (node)->ind_r ||                       \
            (node)->ind_r == (node)->ind_l)                   \
            temprry = -1;                                     \
        RemoveAtPos(lst, (node)->ind_r, nullptr);             \
        (node)->ind_r = temprry;                              \
    }
//-----------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------
#define DECLS_GUARD__ {
#define __DECLS_GUARD }
//-----------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------
       

static inline int Round(double val)
{
    return (int)(val + (val > 0 ? +0.5 : -0.5));
}

inline static bool IsZero(double expr)
{
    return expr > -DBL_PRECISION && expr < DBL_PRECISION;
}



inline static void ResetNode(Node* node, Types new_type, double new_value)
{
    node->type = new_type;
    node->value = new_value;
}
inline static void ResetNode(Node* node, Types new_type, Operations new_value)
{
    node->type = new_type;
    node->value = (double)new_value;
}

static int Tree_ctor(Tree* tree, Types opertype, double value)
{
    assert(tree);

    ResetNode(&(tree->root), opertype, value);
    ResetNode(&(tree->subroot), opertype, value);
    tree->root.ind_l = -1;
    tree->root.ind_r = -1;
    tree->subroot.ind_l = -1;
    tree->subroot.ind_r = -1;

    if (MakeList(&(tree->refs), nullptr))         return 1;

    return 0;
}



static int printType(Types type, FILE* out)
{

#define CASE(type) case type: fprintf(out, #type); return 0;

    switch (type)
    {

        CASE(BINAR)
        CASE(CONST_E)
        CASE(CONST_PI)
        CASE(NUM)
        CASE(VAR)
        CASE(UNAR)
        CASE(VOID)

    default:
        fprintf(out, "UNKNOWN\n%d", type);
        return 1;

    }

#undef CASE

}

static void printValue(Types oper, double value, FILE* out)
{
    Operations int32_value = (Operations)(value + 0.5);
    switch (oper)

    {

    case CONST_E:
        fprintf(out, "%lfe", value); break;
    case CONST_PI:
        fprintf(out, "%lfpi", value); break;
    case VAR:
        if (isalpha((int)int32_value)) fputc((int)int32_value, out);
        else fprintf(out, "%lf", value);

        return;

    case UNAR: case BINAR:
        break;

    case NUM: default:
        fprintf(out, "%lf", value);
        return;

    }


    switch (int32_value)

    {

    case BINAR_add:
        fputs("+ ", out); return;
    case BINAR_mul:
        fputs("*", out); return;
    case UNAR_recp:
        fputs("1/", out); return;
    case BINAR_pow:
        fputs("^", out); return;
    case UNAR_sub:
        fputs("-", out); return;

    #define CASE(oper) case UNAR_##oper: fprintf(out, #oper" "); return;

        CASE(sin)
        CASE(cos)
        CASE(tan)
        CASE(sinh)
        CASE(cosh)
        CASE(tanh)
        CASE(log)

    #undef CASE

    default:
        fprintf(out, "UNKNOWN\n%lf", value);
    }
}


static void printNode(Tree* tree, Node* node, Node* parent, FILE* out)
{
    USE_LVLDOWN_ITERATION

    fprintf(out, R"(elem%p [shape=record,color=black,style=filled,fillcolor=)"
        R"("#FF7BFF"  label=" {{0x%p | <main> )",
        node, node);

    printType(node->type, out);
    fprintf(out, "|");
    printValue(node->type, node->value, out);

    fprintf(out, " } | { ");


    DO_FOR_EACH_SON(node,
        fprintf(out, " %s <adr%p> 0x%p", son_index == node->ind_l ? "" : "|", *son, *son);
    )
    fputs("}}\" ]; \n", out);

    if (parent) 
        fprintf(out, "elem%p:<adr%p> -> elem%p:<main>[color=\"#01740A\"];\n",
            parent, node, node);
}


static void Dump(Tree* tree, struct Node* node, FILE* graph_file)
{
    USE_LVLDOWN_ITERATION
        printf("\nNode at 0x%p\n", node);
    if (node == NULL) return;

    printf("Contains: %d %lf\n", node->type, node->value);
    printf("Left index is %d, right %d\n", node->ind_l, node->ind_r);
    printf("Node(-s) downer:\n");
    DO_FOR_EACH_SON(node, printf("0x%p\n", *son); fflush(stdout);)

    if (graph_file)
    {
        DO_FOR_EACH_SON(node,

        printNode(tree, *son, node, graph_file);

        )
    }

    fflush(stdout);
}


static void RecDump(Tree* tree, Node* node, FILE* out)
{
    Dump(tree, node, out);

    USE_LVLDOWN_ITERATION
    DO_FOR_EACH_SON(node, RecDump(tree, *son, out);)
}


static void CallDump(Tree* tree, Node* node)
{
    static int calls = 0;
    FILE* dmp = nullptr;
    if (fopen_s(&dmp, graphvisfile, "w") || dmp == nullptr)
        return;

    fprintf(dmp, "digraph tree {\n");

    printNode(tree, node, nullptr, dmp);
    RecDump(tree, node, dmp);

    fputc('}', dmp);
    fclose(dmp);

    char dotcall[] = "..\\..\\Graphviz\\bin\\dot.exe -Tpng:cairo \"" graphvisfile "\" >\"img dumps\\res1000.png\"";
    dotcall[_countof(dotcall) - 10] += (calls % 10000) / 1000;
    dotcall[_countof(dotcall) - 9] += (calls % 1000) / 100;
    dotcall[_countof(dotcall) - 8] += (calls % 100) / 10;
    dotcall[_countof(dotcall) - 7] += (calls % 10) / 1;
    system(dotcall);
    calls++;
}


static unsigned int DestroyBranch(Tree* tree, Node* node)
{
    USE_LVLDOWN_ITERATION

    //Dump(tree, node, nullptr);
    if (node == NULL) return 0;

    unsigned int ret = 0;
    DO_FOR_EACH_SON(node, ret += DestroyBranch(tree, *son);)

    free(node);

    return ret + 1;
}


static int Tree_dtor(Tree* tree)
{
    USE_LVLDOWN_ITERATION

    int ret = 0;
    DO_FOR_EACH_SON((&(tree->root)), ret += DestroyBranch(tree, *son);)
    DO_FOR_EACH_SON((&(tree->subroot)), ret += DestroyBranch(tree, *son);)
    ListDistruct(&(tree->refs));
    return ret;
}

#define BAD_ALLOC goto error;
#define CATCH_BAD_ALLOC error:

static errno_t TryRealloc(void** buf, size_t old_cpc, size_t new_cpc)
{
    void* newbuf = realloc(*buf, new_cpc);
    if (newbuf)
    {
        *buf = newbuf;
        if (new_cpc > old_cpc)
            memset((char*)*buf + old_cpc, 0, new_cpc - old_cpc);
        return 0;
    }

    return 1;
}

static Node* MakeNode(Types type, double value)
{
    Node* ret = (Node*)calloc(1, sizeof(Node));
    if (ret == nullptr) return nullptr;

    ret->ind_l = -1;
    ret->ind_r = -1;
    ret->type = type;
    ret->value = value;

    return ret;
}

static Node* HangBranch(Tree* tree, Node* node, Node* source)//TODO: возвращаемое значение
{
    if (node->ind_l == -1)
    {
        node->ind_l = AddToEnd(lst, source);
        if (node->ind_l == -1) return nullptr;

        node->ind_r = node->ind_l;
        return *GetByPhInd(lst, node->ind_r);
    }
    int new_ind = AddAfter(lst, node->ind_r, source);
    if (new_ind == -1) return nullptr;

    node->ind_r = new_ind;
    return *GetByPhInd(lst, node->ind_r);
}
#define HangBranch(node, source) HangBranch(tree, node, source)


static Node* Hang(Tree* tree, Node* node, Types opertype, double value)
{
    assert(node);

    Node* ret = (Node*)calloc(1, sizeof(*ret));

    if (ret == nullptr)
        BAD_ALLOC

        ret->value = value;
    ret->type = opertype;

    ret->ind_l = -1;
    ret->ind_r = -1;

    if (HangBranch(node, ret) == nullptr)
    {
        BAD_ALLOC
    }

    return ret;


    CATCH_BAD_ALLOC

    free(ret);
    return nullptr;


}
#define Hang(node, opertype, value) Hang(tree, node, opertype, value)


inline static Node* HangEmpty(Tree* tree, Node* node)
{
    return Hang(node, VOID, 0);
}
#define HangEmpty(node) HangEmpty(tree, node)

/*
static int Import(Node* dest, FILE* source)
{
    char next = SkipSpaces_get(source);
    if (next != '(')
    {
        printf("[ERROR: expected (, got <%c %d>]", next, next);
        return 1;
    }


    next = SkipSpaces_chk(source);
    if (next == '(')
    {
        if (!dest->left) MakeNode(dest, 0, 0, false);
        Import(dest->left, source);
        next = SkipSpaces_get(source);

        dest->type = BINAR;

        switch (next)
        {
        case '+': dest->value = BINAR_add; break;
        case '-': dest->value = UNAR_sub; break;
        case '*': dest->value = BINAR_mul; break;
        case '/': dest->value = BINAR_div; break;
        case '^': dest->value = BINAR_pow; break;
        default:
            printf("[ERROR: unknown operation %c %d]\n", next, next);
            return 1;
        }

        if (!dest->right) MakeNode(dest, 0, 0, true);
        Import(dest->right, source);
    }

    else if (isdigit(next))
    {
        dest->type = NUM;
        fscanf_s(source, "%lf", &(dest->value));
    }

    else if (isalpha(next))
    {
        dest->type = CONST;
        switch (next)
        {
        case 'e':
            dest->value = E;
            break;
        case 'p':
            dest->value = PI;
            break;
        case 's': case 'c': case 't': case 'l':
        {
            int oper = GetUnar(source);
            if (oper != 0) {
                dest->type = UNAR;
                dest->value = oper;
                if (!dest->right) MakeNode(dest, 0, 0, true);
                Import(dest->right, source);
                break;
            }
        }
        default:
            dest->type = VAR;
            dest->value = (double)(int)next;
            fgetc(source);
        }


    }

    else
    {
        printf("[Syntax error: %c %d]", next, next);
        return 1;
    }

    if ((next = SkipSpaces_get(source)) != ')')
    {
        printf("[ERROR: expected ), got <%c %d>]\n", next, next);
        return 1;
    }

    return 0;
}
*/

static Node* Deepcopy(Tree* tree, const Node* src)
{
    USE_LVLDOWN_ITERATION

    Node* ret = MakeNode(src->type, src->value);
    Node* lhs = nullptr;

    DO_FOR_EACH_SON(src,
        lhs = Deepcopy(tree, *son);
        HangBranch(ret, lhs);
    )

    return ret;
}
#define Deepcopy(src) Deepcopy(tree, src);

static Node* HangCopy(Tree* tree, Node* node, Node* source)
{
    if (0) abort();
    Node* copy = Deepcopy(source);

    if (copy == nullptr)
        return nullptr;

    return HangBranch(node, copy);
}
#define HangCopy(node, source) HangCopy(tree, node, source)

static void DestroyNode(Tree* tree, Node* node)
{
    for (int pos = node->ind_l; true; pos = GetNext(lst, pos))
    {
        RemoveAtPos(lst, pos, nullptr);
        if (pos == node->ind_r) break;
    }
    if (node != &(tree->root))
        free(node);
}
#define DestroyNode(node) DestroyNode(tree, node)

static void DestroySonNode(Tree* tree, Node* parent, Node** nodeptr)
{
    DestroyNode(*nodeptr);
    if (GetByPhInd(lst, parent->ind_l) == nodeptr)
    {
        if (GetByPhInd(lst, parent->ind_r) == nodeptr)
        {
            parent->ind_l = -1;
            parent->ind_r = -1;
        }
        else
        {
            parent->ind_l = GetNext(lst, parent->ind_l);
        }
    }
    else if (GetByPhInd(lst, parent->ind_r) == nodeptr)
    {
        parent->ind_r = GetPrev(lst, parent->ind_r);
    }

}
#define DestroySonNode(parent, nodeptr) DestroySonNode(tree, parent, nodeptr)


////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/*!
params_out & 0b10 == "return parent pointer by 1st various arg"
params_out & 0b01 == "return stack size by 1st/2nd various arg"
[parameter == NEW] == "start walking by 1st various arg"
[parameter == NEXT] == "go to next node"

static Node* WalkOnTree(Parameter parameter, int params_out, ...)
{

#define RETURN va_end(ap); return

#define PROCESS_ARGS(parent)                      \
        if (params_out & 0b01)                    \
        {                                         \
            int* dest = va_arg(ap, int*);         \
            *dest = GetStackSize(stk);            \
        }                                         \


    va_list ap = nullptr;
    va_start(ap, params_out);

    struct Node_pp
    {
        Node* node;
        int ind;
    };

    static Stack* stk = NULL;

    if (parameter == Parameter::NEW)
    {
        if (stk) StackDistruct(&stk);

        NewStack(stk, Node_pp);

        Node* root = va_arg(ap, Node*);

        StackPush(stk, &root);

        if (params_out & 0b01)
        {
            int* dest = va_arg(ap, int*);
            *dest = GetStackSize(stk);
        }

        RETURN root;
    }
    else if (parameter == Parameter::NEXT)
    {
        Node* top = NULL;
        StackPeek(stk, &top);
        if (top->left != NULL)
        {
            StackPush(stk, &(top->left));
            PROCESS_ARGS(top)
            RETURN top->left;
        }
        if (top->right != NULL)
        {
            StackPush(stk, &(top->right));
            PROCESS_ARGS(top)
            RETURN top->right;
        }

        Node* deeper = NULL;
        StackPop(stk, &top);

        while (true)
        {
            if (StackPeek(stk, &deeper))
            {
                RETURN NULL;
            }
            if (deeper->left == top)
            {
                if (deeper->right)
                {
                    StackPush(stk, &(deeper->right));
                    PROCESS_ARGS(deeper)
                        RETURN deeper->right;
                }
            }
            else if (deeper->right != top)
            {
                printf("ERROR: incorrect pair %p(%d|%lf)[%p][%p] and %p(%d|%lf)",
                    deeper, deeper->type, deeper->value, deeper->left, deeper->right, top, top->type, top->value);
                RETURN NULL;
            }

            StackPop(stk, NULL);
            top = deeper;
        }

    }
}
*/
/*
static void TreeGraphDump(Node* root)
{
    assert(root);

    static int calls = 0;

    FILE* dump = fopen(graphvisfile, "w");

    if (dump == NULL)
    {
        printf("ERROR: cannot open file for dump\n");
        return;
    }

    fprintf(dump, "digraph tree {\n");

    Node* position = WalkOnTree(Parameter::NEW, 0b00, root);
    int depth = 1;
    Node* parent = NULL;

    do
    {
        //создать узел с адресом, контентом и left, right
        fprintf(dump, R"(elem%p [shape=record,color=black,style=filled,fillcolor=)"
                      R"("#FF7BFF"  label=" {{0x%p | <main> )",
            position, position);

        printType(position->type, dump);
        fprintf(dump, "|");
        printValue(position->type, position->value, dump);
        fprintf(dump, " } | { ");
        for (int i = 1; i < position->downcpc; i++)
        {
            if (position->down[i])
            {
                fprintf(dump, " | %p", position->down[i]);
                //look at me
            }
        }

        //обозначить его ранг равным depth
        fprintf(dump, "{ rank = same; \"%d\", elem%p }\n", depth, position);

        //стрелка в parent
        if (parent)
            fprintf(dump, "elem%p:<%s> -> elem%p:<main>[color=\"#01740A\"]\n",
                parent, parent->left == position ? "left" : "right", position);

    } while (position = WalkOnTree(Parameter::NEXT, 0b11, &parent, &depth));

    fputc('}', dump);
    fclose(dump);

    putchar('\n');

    char dotcall[] = "..\\..\\Graphviz\\bin\\dot.exe -Tpng:cairo \"" graphvisfile "\" >\"img dumps\\res0000.png\"";
    dotcall[_countof(dotcall) - 10] += (calls % 10000) / 1000;
    dotcall[_countof(dotcall) - 9] += (calls % 1000) / 100;
    dotcall[_countof(dotcall) - 8] += (calls % 100) / 10;
    dotcall[_countof(dotcall) - 7] += (calls % 10) / 1;

    system(dotcall);
    calls++;

}*/
#endif // !INC_APDIFTR
