#include <stdio.h>
#include <math.h>

#include "../../NewOnegin/NewOnegin/String++.h"
#include "Differ_tree.hpp"


const char END = '\0';

#define CHK_NOMEM                                      \
	if (dest == nullptr)                               \
	{                                                  \
		runtime_state = RuntimeState::ERR_NO_MEMORY;   \
		return;                                        \
	}


class Formula
{
public:
	char* line;
	size_t index;
	errno_t runtime_state;

	enum RuntimeState
	{
		OK = 0,
		SYNTAX_ERROR = 1,
		ERR_DIV_ZERO = 2,
		ERR_NO_MEMORY = 3,
	};

	Formula(char* line)
	{
		this->line = line;
		this->index = 0;
		this->runtime_state = RuntimeState::OK;
	}

	char chkchar()
	{
		return this->line[this->index];
	}
	char getchar()
	{
		return this->line[this->index ++];
	}

	inline void SyntaxError()
	{
		printf("Syntax error: %10s...", this->line + this->index);
		this->runtime_state = RuntimeState::SYNTAX_ERROR;
	}

	inline void SyntaxRequire(bool criterion)
	{
		if (!criterion)
		{
			SyntaxError();
		}
	}

	#define CHECK ; {if (runtime_state != OK) { return; } }
	void GetNum(Node* dest)
	{
		CHK_NOMEM
		int ch = chkchar();
		SyntaxRequire(isdigit(Formula::getchar()))CHECK;
		double lhs = 0;
		while (true)
		{
			lhs = lhs * 10 + ch - '0';
			ch = Formula::chkchar();
			if (!isdigit(ch)) break;
			Formula::getchar();
		}
		ResetNode(dest, NUM, lhs);
	}
	//Unar:: = (!digit)→["sin", "cos", "tan", "log", '-'] ↑ nil Prim
	void GetChrVar(Node* dest)
	{
		CHK_NOMEM

		char var[MAX_CONSTANT_LEN + 1] = {}; //consider avoiding copying if longer variables are permitted
		int varlen = 0;

		for (; varlen <= MAX_CONSTANT_LEN; varlen++)
		{
			var[varlen] = Formula::chkchar();
			if (!isalpha(var[varlen]))
			{
				var[varlen] = '\0';
				break;
			}
			Formula::getchar();
		}
		SyntaxRequire(varlen <= MAX_CONSTANT_LEN);
		if (strcmp(var, "pi") == 0)
		{
			ResetNode(dest, CONST_PI, 1);
		}
		else if (strcmp(var, "e") == 0)
		{
			ResetNode(dest, CONST_E, 1);
		}
		else
		{
			SyntaxRequire(varlen == 1);
			ResetNode(dest, VAR, (double)*var);
		}
	}
	void GetVar(Node* dest)
	{
		if (isdigit(Formula::chkchar()))
		{
			GetNum(dest)CHECK;
		}
		else
		{
			GetChrVar(dest)CHECK;
		}
	}
	void GetPrim(Tree* tree, Node* dest)
	{
		CHK_NOMEM

		if (Formula::chkchar() == '(')
		{
			Formula::getchar();
			GetSum(tree, dest)CHECK;
			SyntaxRequire(Formula::chkchar() == ')')CHECK;
			Formula::getchar();
		}
		else
		{
			GetVar(dest)CHECK;
		}
	}
	void GetUnar(Tree* tree, Node* dest)
	{
		CHK_NOMEM

		if (!isalpha(Formula::chkchar()))
		{
			GetPrim(tree, dest)CHECK;
			return;
		}
		char* command = (char*)line + index;
		char now = 0;
		int cmdlen = 0;

		for (; true; cmdlen++)
		{
			now = Formula::chkchar();
			if (!isalpha(now))
			{
				command[cmdlen] = '\0';
				break;
			}
			Formula::getchar();
		}
		

	#define MAKE_UNAR(oper)                             \
		command[cmdlen] = now;                          \
        Node* rhs = HangEmpty(dest);                    \
		dest->type = UNAR;                              \
		dest->value = UNAR_##oper;                      \
		GetUnar(tree, rhs)CHECK;

		ENUMERATE_UNAR(command,
			MAKE_UNAR(sin)
			,
			MAKE_UNAR(cos)
			,
			MAKE_UNAR(log)
			,
			MAKE_UNAR(sinh)
			,
			MAKE_UNAR(cosh)
			,
			command[cmdlen] = now;
			index -= cmdlen;
			GetChrVar(dest)CHECK;
		)
	}
	void GetTwer(Tree* tree, Node* dest)
	{
		CHK_NOMEM

		dest->type = BINAR;
		dest->value = BINAR_pow;

		Node* lhs = HangEmpty(dest);
		GetUnar(tree, lhs)CHECK;

		while (Formula::chkchar() == '^')
		{
			Formula::getchar();
			lhs = HangEmpty(dest);
			GetUnar(tree, lhs)CHECK;
		}
	}
	void GetMul(Tree* tree, Node* dest)
	{
		CHK_NOMEM

		ResetNode(dest, BINAR, BINAR_mul);

		Node* lhs = HangEmpty(dest);
		GetTwer(tree, lhs)CHECK;

		while (chkchar() == '*' || chkchar() == '/')
		{
			bool multiply = Formula::getchar() == '*';
			if (multiply)
			{
				lhs = HangEmpty(dest);
				GetTwer(tree, lhs)CHECK;
			}
			else
			{
				Node* rhs = Hang(dest, BINAR, BINAR_pow);
				lhs = HangEmpty(rhs);
				GetTwer(tree, lhs)CHECK;
				lhs = Hang(rhs, NUM, -1);
			}
		}
	}
	void GetSum(Tree* tree, Node* dest)
	{
		CHK_NOMEM

		ResetNode(dest, BINAR, BINAR_add);

		Node* lhs = HangEmpty(dest);
		GetMul(tree, lhs)CHECK;
		while (chkchar() == '+' || chkchar() == '-')
		{
			if (Formula::getchar() == '-')     
			{
				if (!(lhs = Hang(dest, UNAR, UNAR_sub)) ||
					!(lhs = HangEmpty(lhs)))                                                
				{
					runtime_state = ERR_NO_MEMORY;
					return;
				}
			
			}
			else
			{
				lhs = HangEmpty(dest);
			}
			GetMul(tree, lhs)CHECK;
		}
	}
};
/*
G    ::=  Sum $
Sum  ::=  Mul  { ['+', '-'] Mul  } *
Mul  ::=  Twer { ['*', '/'] Tower } *
Tower::=  Unar { '^' Unar } *
Unar ::=  (!digit) → ["sin", "cos", "tan", "exp", ... , '-'] ↑ nil Prim
Prim ::=  '(' → { Sum ')' } ↑ Var
Var  ::=  (digit) → Num ↑ Alpha
Alpha::=  {'pi', (alpha)}
Num  ::=  ['0'...'9'] +
*/
static errno_t cooler_Import(const char* filename, Tree* tree)
{
	char skip[] = " \n\t";
	char* const formula = ReadFile(filename, skip);
	printf("%s\n", formula);
	if (formula == NULL)
	{
		printf("Error while reading file\n");
		return 1;
	}

	Formula cnt = Formula(formula);

	Node* dest = &(tree->root);
	cnt.GetSum(tree, dest);
	cnt.SyntaxRequire(cnt.getchar() == END);

	free(formula);
	return cnt.runtime_state;
}
