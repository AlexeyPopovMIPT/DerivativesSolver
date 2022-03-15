#pragma once
#include "Differ_tree.hpp"

#define CALL_TEX(tp, op, downer)                            \
        { bool needbrackets = NeedBrackets(tp, op, downer); \
        if (needbrackets) { fputc('(', out); }              \
        TeX_dump(tree, downer, out);                        \
        if (needbrackets) { fputc(')', out); } } 


static void TeX_dump(Tree*, Node*, FILE*);


static bool NeedBrackets(Types type, double oper, Node* down)
{
    bool ret = false;

    switch (type)
    {

    case UNAR:
        if (oper == UNAR_sub)
        {
            if (down->type == NUM || down->type == VAR || down->type == CONST_E || down->type == CONST_PI)
                return false;
        }
        return true;
    case NUM: case VAR: case CONST_E: case CONST_PI: return oper < 0;

    case BINAR: DECLS_GUARD__

        bool ret = false;
        if (oper == BINAR_mul || oper == BINAR_pow)
            ret |= down->type == BINAR && (down->value == (double)BINAR_add || down->value == (double)UNAR_sub);
        if (oper == BINAR_pow)
            ret |= down->type == BINAR && (down->value == (double)BINAR_mul || down->value == (double)BINAR_pow);
        return ret;

        __DECLS_GUARD
    default:
        return false;
    }

}


static void printDouble(double val, FILE* out)
{
    const int MAX_DOUBLE_LEN = 500;

    char nbr[MAX_DOUBLE_LEN] = {};

    sprintf_s(nbr, _countof(nbr), "%lf", val);

    if (nbr[0] == '\0')
    {
        printf("ERROR: unexpectedly big double length\n");
        return;
    }

    char* dot = strchr(nbr, '.');
    if (dot == nullptr)
    {
        fprintf(out, "%s", nbr);
        return;
    }
    char* zero = strrchr(dot, '0');
    if (zero == nullptr)
    {
        fprintf(out, "%s", nbr);
        return;
    }

    while (*(--zero) == '0');
    if (*zero == '.')
    {
        zero[0] = '\0';
    }
    else
    {
        zero[1] = '\0';
    }

    fprintf(out, "%s", nbr);
    return;

}

static int DividingCase(Tree* tree, Node* node, FILE* out)
{
    const int NO_DIVIDING = 0;
    const int DIVIDED = 1;

    if (node->type != BINAR || node->value != BINAR_pow)
        return NO_DIVIDING;

    if (GetNext(lst, node->ind_l) != node->ind_r)
        return NO_DIVIDING;


    if (R(node)->type == UNAR && R(node)->value == UNAR_sub)
    {
        fputs(" \\frac {1} {{", out);
        CALL_TEX(node->type, node->value, L(node))
        //TeX_dump(tree, L(node), out);
        fputs("} ^ {", out);
        if(!(L(R(node))->type == NUM && IsZero(L(R(node))->value - 1)))
            TeX_dump(tree, L(R(node)), out);
        fputs("}}", out);
        return DIVIDED;
    }

    if (R(node)->type == NUM || R(node)->type == CONST_E || R(node)->type == CONST_PI)
    {
        if (R(node)->value >= 0 || IsZero(R(node)->value))
            return NO_DIVIDING;

        fputs(" \\frac {1} {{", out);
        if (R(node)->type == NUM && IsZero(R(node)->value + 1))
        {
            TeX_dump(tree, L(node), out);
            fputs("}}", out);
        }
        else
        {
            CALL_TEX(node->type, node->value, L(node))
            fputs("} ^ {", out);
            printDouble(-(R(node)->value), out);
            fprintf(out, "%s}}", R(node)->type == CONST_E ? "e" : R(node)->type == CONST_PI ? "pi" : "");
        }

        return DIVIDED;
    }

    return NO_DIVIDING;
}

static void TeX_dump(Tree* tree, Node* node, FILE* out)
{
    Operations op = (Operations)Round(node->value);
    switch (node->type)
    {
    case NUM: case CONST_E: case CONST_PI:
        fputc('{', out);
        printDouble(node->value, out);
        fprintf(out, "%s}", node->type == CONST_E ? "e" : node->type == CONST_PI ? "pi" : "");
        break;
    case VAR:
        fputc(Round(node->value), out);
        break;
    case UNAR:

    #define CASE(oper) case UNAR_##oper: fprintf(out, #oper); break;

        switch (op)
        {
            CASE(sin)
                CASE(cos)
                CASE(tan)
                CASE(sinh)
                CASE(cosh)
                CASE(tanh)
                CASE(log)

        case UNAR_sub:
            fputc('-', out);
            break;

        }
        CALL_TEX(node->type, op, L(node))

        break;

    #undef CASE
    case BINAR: DECLS_GUARD__
        if (!DividingCase(tree, node, out))
        {
            USE_LVLDOWN_ITERATION

            if (op == BINAR_add)
            {
                CALL_TEX(node->type, op, L(node));
                DO_FOR_EACH_SON_excST(node,
                    fprintf(out, "%s", (*son)->type == UNAR && (*son)->value == UNAR_sub ? "" : "+");
                    CALL_TEX(node->type, op, *son)
                )
            }
            else if (op == BINAR_pow)
            {
                int cnt = 1; fputc('{', out);
                DO_FOR_EACH_SON_excEND(node,
                    CALL_TEX(node->type, op, *son)
                    fprintf(out, "^{"); cnt++;
                )
                CALL_TEX(node->type, op, R(node))
                while(cnt--) fputc('}', out);
            }
            else
            {
                DO_FOR_EACH_SON_excEND(node,
                    CALL_TEX(node->type, op, *son)
                    fprintf(out, " \\cdot ");
                )
                CALL_TEX(node->type, op, R(node))
            }
        }
        break; __DECLS_GUARD
    }
}
