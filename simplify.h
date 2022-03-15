#pragma once
#ifndef INC_APDIFFSIM
#define INC_APDIFFSIM

#include <stdio.h>
#include <stdlib.h>

#include "Differ_tree.hpp"
#include "../../Dvusvyazniy list/Dvusvyazniy list/The List.h"

#define c *changed = true;

//-----------------------------------------------------------------------------------//
//------------------------------вспомогательные функции------------------------------//
void setnum(Tree* tree, Node* nd, double val, bool* changed)
{
    ResetNode(nd, NUM, val);
    size_t ph_ind = nd->ind_l;
    while (true)
    {
        RemoveAtPos(lst, ph_ind, nullptr);
        if (ph_ind == nd->ind_r)
            break;
        ph_ind = GetNext(lst, ph_ind);
    }
    nd->ind_l = -1;
    nd->ind_r = -1;
    c
}

#define setnum(nd, val, changed) setnum(tree, nd, val, changed)

int getOddity(double val)
{
    int eqiv = (int)(val + 0.5);//todo: - 0.5 if val < 0, so new func
    if (!IsZero(val - eqiv))
        return -1;

    return eqiv % 2;
}


inline bool isInt(double val)
{
    return IsZero(val - (double)(int)(val + 0.5));
}

//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//вычислить суммы и произведения констант
errno_t SimplifySimilarBranches(Tree* tree, Node* source, bool* changed)
{
    USE_LVLDOWN_ITERATION

    if (source == nullptr)
    {
        NOMEM
    }

    Types type = Round(source->type);
    switch (type)
    {
    case BINAR:
        //ListTextDump(*lst, stdout);
        if (source->ind_l == source->ind_r)
        {
            break;
        }
        DECLS_GUARD__
        bool isAdd = false;
        if (source->value == (double)BINAR_add)
        {
            isAdd = true;
        }
        else if (source->value != (double)BINAR_mul)
        {
            break;
        }

        Node** dest_NUM = nullptr;
        Node** dest_CONST_E = nullptr;
        Node** dest_CONST_PI = nullptr;

        #define IF(Type)                                           \
        if ((*son)->type == Type)                                  \
        {                                                          \
            if (dest_##Type == nullptr)                            \
            {                                                      \
                 dest_##Type = son;                                \
                 next = GetNext(lst, i);                           \
            }                                                      \
            else                                                   \
            {                                                      \
                if (isAdd) (*dest_##Type)->value                   \
                      += (*son)->value;                            \
                else       (*dest_##Type)->value                   \
                      *= (*son)->value;                            \
                next = GetNext(lst, i);                            \
                CUT                                                \
            }                                                      \
        }
        #define CUT RemoveAtPos(lst, i, nullptr); c


        int next = 0;
        int i = 0;
        if (source->ind_l != -1)
        {
            for (i = source->ind_l; i != source->ind_r; i = next)
            {
                son = GetByPhInd(lst, i);
                if ((*son)->type == NUM)
                {
                    if (dest_NUM == nullptr)
                    {
                        dest_NUM = son;
                        next = GetNext(lst, i);
                    }
                    else
                    {
                        if (isAdd) {
                            (*dest_NUM)->value
                                += (*son)->value;
                        }
                        else {
                            (*dest_NUM)->value
                                *= (*son)->value;
                        }
                        next = GetNext(lst, i);
                        CUT
                    }
                }
                else if (isAdd)
                {
                    IF(CONST_E)
                    else IF(CONST_PI)
                    else next = GetNext(lst, i);
                }
                else
                {
                    next = GetNext(lst, i);
                }
            }
        }
        #undef CUT
        #define CUT cutright(source) c


        son = GetByPhInd(lst, i);
        if (son)
        {
            if ((*son)->type == NUM)                                  
            {                                                          
                if (dest_NUM == nullptr)
                {                                                      
                    dest_NUM = son;
                    next = GetNext(lst, i);                      
                }                                                      
                else                                                   
                {                                                      
                    if (isAdd) {
                        (*dest_NUM)->value
                            += (*son)->value;
                    }
                    else {
                        (*dest_NUM)->value
                            *= (*son)->value;
                    }
                    next = GetNext(lst, i);                       
                    CUT                                                
                }                                                      
            }
            else if (isAdd)
            {
                IF(CONST_E)
                else IF(CONST_PI)
            }
        }


        #undef CUT
        #undef IF

        #define rmv(type, num)                                                  \
            if (dest_##type && IsZero((*dest_##type)->value - num))             \
            {                                                                   \
                if (GetByPhInd(lst, source->ind_l) == dest_##type)              \
                    { cutleft(source) c }                                       \
                else if (GetByPhInd(lst, source->ind_r) == dest_##type)         \
                    { cutright(source) c }                                      \
                else                                                            \
                    { RemoveByPtr(lst, dest_##type); c }                        \
            } else
        if (isAdd)
        {
            rmv(NUM, 0)
            rmv(CONST_E, 0)
            rmv(CONST_PI, 0);

            if (source->ind_l == -1)
            {
                ResetNode(source, NUM, 0); c
            }
        }
        else
        {
            rmv(NUM, 1)
            rmv(CONST_E, 1)
            rmv(CONST_PI, 1);

            if (source->ind_l == -1)
            {
                ResetNode(source, NUM, 1); c
            }

            if (dest_NUM      && *dest_NUM      && IsZero((*dest_NUM)->value)     ||
                dest_CONST_E  && *dest_CONST_E  && IsZero((*dest_CONST_E)->value) ||
                dest_CONST_PI && *dest_CONST_PI && IsZero((*dest_CONST_PI)->value)   )
            {
                ResetNode(source, NUM, 0.0);
                DO_FOR_EACH_SON(source,
                    DestroyBranch(tree, *son);
                )
                source->ind_l = -1;
                source->ind_r = -1;
                c
            }
        }
        

        #undef rmv


        __DECLS_GUARD
        break; 
    default:
        break;
    }

    DO_FOR_EACH_SON(source,
        if (SimplifySimilarBranches(tree, *son, changed))
        {
            return 1;
        }
    );

    return 0;

}

//специальные случаи унарных операций типа sin(pi*n), log(e*m) и т.д.,
//а также --а = a, log(u*v) = log u + log v, log(u^v) = v log u
errno_t SimplifyUnar(Tree* tree, Node* source, bool* changed)
{
    USE_LVLDOWN_ITERATION

    if (source == nullptr)
    {
        NOMEM
    }


    if (source->type == UNAR)
    {
        Operations oper = (Operations)Round(source->value);
        Node* lhs = nullptr;
        Node* rhs = nullptr;
        switch (oper)
        {
        case UNAR_sin: case UNAR_cos:
            if (L(source)->type == NUM || L(source)->type == CONST_E)
            {
                if (IsZero(L(source)->value))
                {
                    setnum(source, oper == UNAR_cos, changed);
                    return 0;
                }
                else break;
            }
            if (L(source)->type == CONST_PI)
            {
                int oddity = getOddity(L(source)->value);
                if (oddity == 0)
                {
                    setnum(source, source->value == UNAR_cos, changed);
                    return 0;
                }
                else if (oddity == 1)
                {
                    setnum(source, -(source->value == UNAR_cos), changed);
                    return 0;
                }
                else
                {
                    oddity = getOddity(L(source)->value + 0.5);
                    if (oddity == -1) break;
                    setnum(source, source->value == UNAR_cos ? 0 : oddity == 0 ? -1 : 1, changed);
                    return 0;
                }
            }
            break;
        case UNAR_tan:
            if (isInt(L(source)->value - 0.25))
            {
                setnum(source, 1, changed);
            }
            else if (isInt(L(source)->value + 0.25))
            {
                setnum(source, -1, changed);
            }
            else if (isInt(L(source)->value))
            {
                setnum(source, 0, changed);
            }
            break;
        case UNAR_log:
            lhs = L(source);
            if (lhs->type == NUM && IsZero(lhs->value - 1))
            {
                /*optimize log(1) == 0*/
                setnum(source, 0, changed);
                break;
            }
            else if (L(source)->type == CONST_E)
            {
                /*optimize log(m*e) == 1 + log(m)*/
                c
                double temp = lhs->value;
                ResetNode(source, BINAR, BINAR_add);
                ResetNode(lhs, UNAR, (double)UNAR_log);
                Hang(lhs, NUM, temp);
                Hang(source, NUM, 1);
                break;
            }
            else if (L(source)->type == BINAR)
            {
                if (L(source)->value == BINAR_pow)
                {
                    /*optimize log(expr^tower) == tower * log(expr)*/
                    if (source->ind_l == source->ind_r)
                        break;

                    c
                    ResetNode(source, BINAR, BINAR_mul);
                    lhs = Hang(source, UNAR, UNAR_log);
                    HangBranch(lhs, L(L(source)));
                    cutleft(L(source));

                    break;
                }
                if (L(source)->value == BINAR_mul)
                {
                    /*optimize log(a*b*c) == log(a) + log(b) + log(c)*/
                    if (source->ind_l == source->ind_r)
                        break;

                    c
                    ResetNode(source, BINAR, BINAR_add);
                    lhs = L(source);
                    ResetNode(lhs, BINAR, BINAR_add);
                    DO_FOR_EACH_SON(lhs,
                        rhs = MakeNode(UNAR, UNAR_log);
                        HangBranch(rhs, *son);
                        *son = rhs;
                    )
                    break;
                }          
            }
            break;
        case UNAR_sub:
            lhs = L(source);
            if (lhs->value == UNAR_sub)
            {
                rhs = L(lhs);
                cutleft(lhs);
                cutleft(source);
                *source = *rhs;
                c
            }
            else if (lhs->type == NUM)
            {
                cutleft(source);
                *source = *lhs;
                source->value = -source->value;
                c
            }
            break;
        case UNAR_sinh: case UNAR_cosh: case UNAR_tanh:
            lhs = L(source);
            if (lhs->type == CONST_E || lhs->type == CONST_PI || lhs->type == NUM)
            {
                if (IsZero(lhs->value))
                {
                    ResetNode(source, NUM, source->value == UNAR_cosh);
                    DestroyNode(lhs);
                    cutleft(source);
                    c
                }
            }
            break;
        default:
            printf("ERROR on `%s`, line %d: unknown unar type %lf", __FUNCSIG__, __LINE__, source->value);
            return 1;
        }
    }
    DO_FOR_EACH_SON (source,
        if (SimplifyUnar(tree, *son, changed)) { return 1; }
    )
    return 0;
}

//устранить суммы, произведения и башни от одного поддерева
errno_t SimplifyPoliar(Tree* tree, Node* source, bool* changed)
{
    USE_LVLDOWN_ITERATION

    if (source == nullptr)
    {
        NOMEM
    }

    if (!(source->type == BINAR && source->ind_l == source->ind_r))
    {
        //ничего не делать, вызваться рекурсивно
        errno_t ret = 0;
        DO_FOR_EACH_SON(source,
            ret |= SimplifyPoliar(tree, *son, changed);)

        return ret;
    }

    Node* lhs = L(source);
    cutleft(source);
    *source = *lhs;

    c

    free(lhs);

    return SimplifyPoliar(tree, source, changed);
}

errno_t SimplifyNeighbouring(Tree* tree, Node* node, bool* changed)
{
    USE_LVLDOWN_ITERATION

    if(node == nullptr)
    {
        NOMEM
    }
    if (node->type == BINAR)
    {
        for (int next = 0, son_index = node->ind_l; true; son_index = next)
        {
            son = GetByPhInd(lst, son_index);
            if ((*son)->type == BINAR && (*son)->value == node->value)
            {
                c
                AddRangeAfter(lst, son_index, (*son)->ind_l, (*son)->ind_r);
                if (son_index == node->ind_r) node->ind_r = (*son)->ind_r;
                if (son_index == node->ind_l) node->ind_l = (*son)->ind_l;
                next = GetNext(lst, son_index);
                free(*son);
                RemoveByPtr(lst, son);
            }
            else
            {
                next = GetNext(lst, son_index);
            }
            if (son_index == node->ind_r) break;
        }
    }
    errno_t ret = 0;

    DO_FOR_EACH_SON(node,
        ret |= SimplifyNeighbouring(tree, *son, changed);
    )
    return ret;
}

#undef c
#endif //INC_APDIFFSIM
