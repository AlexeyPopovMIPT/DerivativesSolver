#pragma warning (disable: 4996)

#include <stdio.h>
#include <stdlib.h>
#include "import.cpp"
#include "simplify.h"
#include "Bruh.h"

#define TOP(type) Top_##type(tree, source, out)
#define BTM(type) Bottom_##type(tree, source, dest, out)

errno_t GetDerivative(Tree* tree, Node* dest, Node* source, char var, FILE* out) //var - свободная переменная
{
    USE_LVLDOWN_ITERATION

    if (dest == nullptr || source == nullptr)
    {
        printf("ERROR: formula ia too big");
        return 1;
    }

    Node* lhs = nullptr;
    Node* rhs = nullptr;

    switch (source->type)
    {
    case VOID:
        return 0;
    case NUM: case CONST_E: case CONST_PI:
        TOP(NUM);
        ResetNode(dest, NUM, 0);
        return 0;
    case VAR:
        TOP(NUM);
        ResetNode(dest, NUM, (double)IsZero(source->value - var));
        return 0;
    case UNAR: DECLS_GUARD__
        Node* param = *GetByPhInd(lst, source->ind_l);
        switch ((Operations)source->value)
        {
        case UNAR_cos: TOP(UNAR_cos);
            ResetNode(dest, BINAR, BINAR_mul);
            lhs = Hang(dest, UNAR, UNAR_sub);
            lhs = Hang(lhs, UNAR, UNAR_sin);
            lhs = HangCopy(lhs, param);
            lhs = HangEmpty(dest);
            GetDerivative(tree, lhs, param, var, out);
            BTM(UNAR_cos);
            break;
        case UNAR_tan: case UNAR_tanh:
            ResetNode(dest, BINAR, BINAR_mul);
            lhs = Hang(dest, BINAR, BINAR_pow);

            rhs = Hang(lhs, UNAR, source->value == UNAR_tan ? UNAR_cos : UNAR_cosh);
            HangCopy(rhs, param);

            rhs = Hang(lhs, NUM, -2);
            rhs = HangEmpty(dest);
            GetDerivative(tree, rhs, param, var, out);
            break;
        case UNAR_sin: TOP(UNAR_sin);
            ResetNode(dest, BINAR, BINAR_mul);
            lhs = Hang(dest, UNAR, UNAR_cos);
            lhs = HangCopy(lhs, param);
            rhs = HangEmpty(dest);
            GetDerivative(tree, rhs, param, var, out);
            BTM(UNAR_sin);
            break;
        case UNAR_cosh: TOP(UNAR_sinh);
            ResetNode(dest, BINAR, BINAR_mul);
            lhs = Hang(dest, UNAR, UNAR_sinh);
            lhs = HangCopy(lhs, param);
            rhs = HangEmpty(dest);
            GetDerivative(tree, rhs, param, var, out);
            BTM(UNAR_sinh);
            break;
        case UNAR_sinh: TOP(UNAR_sinh);
            ResetNode(dest, BINAR, BINAR_mul);
            lhs = Hang(dest, UNAR, UNAR_cosh);
            lhs = HangCopy(lhs, param);
            rhs = HangEmpty(dest);
            GetDerivative(tree, rhs, param, var, out);
            BTM(UNAR_sinh);
            break;
        case UNAR_sub: TOP(UNAR_sub);
            ResetNode(dest, UNAR, UNAR_sub);
            rhs = HangEmpty(dest);
            GetDerivative(tree, rhs, param, var, out);
            BTM(UNAR_sub);
            break;
        case UNAR_log: TOP(UNAR_log);
            ResetNode(dest, BINAR, BINAR_mul);
            lhs = Hang(dest, BINAR, BINAR_pow);
            rhs = HangCopy(lhs, param);
            rhs = Hang(lhs, NUM, -1);
            rhs = HangEmpty(dest);
            GetDerivative(tree, rhs, param, var, out);
            BTM(UNAR_log);
            break;
        }
        return 0; __DECLS_GUARD
    case BINAR:
        switch ((Operations)source->value)
        {
        case BINAR_add: DECLS_GUARD__
            TOP(BINAR_add);
            ResetNode(dest, BINAR, BINAR_add);
            DO_FOR_EACH_SON(source, lhs = HangEmpty(dest); GetDerivative(tree, lhs, *son, var, out);)
            BTM(BINAR_add);
            break; __DECLS_GUARD
        case BINAR_mul: DECLS_GUARD__ 
            TOP(BINAR_mul);
            ResetNode(dest, BINAR, BINAR_mul);
            lhs = Hang(dest, BINAR, (double)BINAR_add);
            rhs = HangEmpty(lhs);
            DO_FOR_EACH_SON(source,
                HangCopy(dest, *son);
                rhs = Hang(lhs, UNAR, (double)UNAR_log);
                HangBranch(rhs, *son);
            )
            int temp = lhs->ind_l;
            lhs->ind_l = GetNext(lst, lhs->ind_l);

            DO_FOR_EACH_SON(lhs,
                rhs = *GetPrevPtr(lst, son);
                GetDerivative(tree, rhs, *son, var, out);
                son = GetByPhInd(lst, son_index);
                RemoveAtPos(lst, (*son)->ind_l, nullptr);
                (*son)->ind_l = -1;
                (*son)->ind_r = -1;
            )

            lhs->ind_l = temp;

            cutright(lhs);
            BTM(BINAR_mul);
            break; __DECLS_GUARD
        case BINAR_pow: DECLS_GUARD__
            /*bool isLeftConstant = IsConstant_that_always_returns_zero(source->left);
            bool isRightConstant = IsConstant_that_always_returns_zero(source->left);
            if (isLeftConstant)
            {
                if (isRightConstant)
                {
                    dest->type = NUM;
                    dest->value = 0;
                    return 0;
                }
                SET_BINAR(mul)
                dest->left = (Node*)source;
                MakeNode(dest, UNAR, UNAR_log, true);
                dest->right->right = source->left;
            }
            if (isRightConstant)
            {
                SET_BINAR(mul)
                dest->left = source->right;
                MakeNode(dest, BINAR, BINAR_pow, true);
                dest->right->left = 0;здесь я забыл умножить на производную, и выше тоже
            }*/ 
            ResetNode(dest, BINAR, BINAR_mul);
            Node* main_mul = dest;
            son = GetByPhInd(lst, source->ind_l);
            for (int sn = source->ind_l; sn != source->ind_r; sn = GetNext(lst, sn))
            {
                TOP(BINAR_pow);

                lhs = Hang(main_mul, BINAR, (double)BINAR_pow);;
                int tw_floor = sn;
                while (true)
                {
                    HangCopy(lhs, *GetByPhInd(lst, tw_floor));
                    if (tw_floor == source->ind_r) break;
                    tw_floor = GetNext(lst, tw_floor);
                }
                rhs = Hang(main_mul, BINAR, (double)BINAR_add);
                main_mul = rhs;

                lhs = Hang(rhs, BINAR, (double)BINAR_mul);
                rhs = HangEmpty(lhs);
                GetDerivative(tree, rhs, *GetByPhInd(lst, sn), var, out);
                rhs = Hang(lhs, BINAR, (double)BINAR_pow);
                HangCopy(rhs, *GetByPhInd(lst, sn));
                Hang(rhs, NUM, -1);
                rhs = Hang(lhs, BINAR, (double)BINAR_pow);
                tw_floor = GetNext(lst, sn);
                while (true)
                {
                    HangCopy(rhs, *GetByPhInd(lst, tw_floor));
                    if (tw_floor == source->ind_r) break;
                    tw_floor = GetNext(lst, tw_floor);
                }

                lhs = Hang(main_mul, BINAR, (double)BINAR_mul);
                rhs = Hang(lhs, UNAR, (double)UNAR_log);
                HangCopy(rhs, *GetByPhInd(lst, sn));
                main_mul = Hang(lhs, BINAR, (double)BINAR_mul);
            }

            GetDerivative(tree, main_mul, *GetByPhInd(lst, source->ind_r), var, out);
            BTM(BINAR_pow);
            break; __DECLS_GUARD
        }
    }
    return 0;
}

#undef TOP
#undef BTM

//привести суммы, произведения, посчитать вычислимые поддеревья,
//устранить суммы, произведения и башни от одного поддерева,
//e^log, cos/sin/tan/cosh/sinh/tanh(0), log(1), log(e), log(tower),


int main()
{
    const char* source_file = "source.fml";

    Tree tree = {};
    Tree_ctor(&tree, VOID, 0);

    if (cooler_Import(source_file, &tree))
        return 0;

    bool changed = false;

    SimplifyPoliar(&tree, &(tree.root), &changed);
    SimplifyUnar(&tree, &(tree.root), &changed);
    SimplifySimilarBranches(&tree, &(tree.root), &changed);
    SimplifyPoliar(&tree, &(tree.root), &changed);

    FILE* out = MakeTemplate();
    if (out == nullptr)
        return 0;

    FuncPrint(out, &tree, &(tree.root));

    if (GetDerivative(&tree, &(tree.subroot), &(tree.root), 'x', out))
        return 0;
    BeforeSimpify(&tree, out);



    changed = true;
    while (changed)
    {
        changed = false;
        SimplifyPoliar(&tree, &(tree.subroot), &changed);
        SimplifyUnar(&tree, &(tree.subroot), &changed);
        SimplifySimilarBranches(&tree, &(tree.subroot), &changed);
        SimplifyNeighbouring(&tree, &(tree.subroot), &changed);
    }
    
    AfterSimplify(&tree, out);

    system(R"(..\..\..\..\AppData\Local\Programs\MiKTeX\miktex\bin\x64\pdflatex.exe ")" TeX_file "\"");

    return Tree_dtor(&tree);
}
