#include <time.h>
#include "../../NewOnegin/NewOnegin/String++.h"
#include "texdump.h"

const char* const phrases_al[] =
{
	u8"Ноль, целковый...",
	u8"Здесь ограничимся магическим комментарием \"Очевидно, что:\"",
	u8"Кто не верит, пусть загонит в Wolfram",
	u8"Вот прям взяли дельта икс и устремили к нулю, честное слово",
	u8"Ландау такое в 14 лет мог посчитать:",
	u8"После этих слов в украинском поезде начался сущий кошмар:",
	u8"H-H Не спускайся вниз\\\\\nH-H\\\\\nH-H\\\\\nH-H",
	u8"H-H Чтож, если ты хочешь нажать \"Мне нравится\", тогда спускайся ..\\\\\nH-H\\\\\nH-H\\\\\nH-H",
	u8"H-H Не надоело еще?\\\\\nH-H\\\\\nH-H\\\\\nH-H",
	u8"Если вы думаете, что это больно, вспомните о тех, кто купил киберпанк на 4-ю плойку:",
	u8"Каждый комментарий со словом \"Полторашка\" увеличивает оценку за прогу на 1 балл:",
	u8"Каждый комментарий со словом \"Полторашка\" увеличивает оценку за прогу на 1 балл:",
	u8"Не хотите купить книжку про эфир?",
	u8"Чуть было не забыл эту формулу на ЕГЭ:",
	u8"Надоел совсем матан, доставай стакан, братан.",
	u8"щывывщафзывщпыхвыпщпуцщауощршпцпушумншпяюгщшпшпгшиташндзхвыаф:",
	u8"Внимание, анекдот! Купил мужик шляпу, а она ему как раз!",
	u8"Кто будет плохо слушать, те сделают это на листочке!",
	u8"О Боже. На это даже взглянуть страшно.",
	u8"Это даже моя программа посчитала устно:",
	u8"А Дуня разливает чай, А Петербург неугомонный, ...",
	u8"Путём несложных математических преобразований получаем",
	u8"Не существует такой эпсилон-окрестности этого шага, в которой нет гадости:",
	u8"Творческое задание: доказать справедливость формулы, проинтегрировав правую её часть:",
	u8"Доказательство этого факта оставим читателю в качестве упражнения.",
	u8"А вы когда-нибудь задумывались, что"
};
const char* const phrases_BINAR_mul[] =
{
	u8"Derivative of multiplication goes brrr",
	u8"Умножение, это, с одной стороны, весело, но отладка...",
	u8"Умножение, оно как сложение, но и как степень, только умножение.",
	u8"Двести тысяч логарифмов продифференцировано и ещё миллион на подходе",
	u8"Надеюсь, что где-то тут будет ноль",
	u8"Не показывайте это кафедре вышмата!!"

};
const char* const phrases_BINAR_pow[] =
{
	u8"Здесь нам приходит на помощь логарифм.",
	u8"Задачка со звёздочкой из учебника 11 класса:",
	u8"Единица - и всё. Всё пропало."
};
const char* const phrases_BINAR_add[] =
{
	u8"Сложим всё вместе:"
};
const char* const phrases_UNAR_cos[] =
{
	u8"Вспомнишь .. ортогональное проектирование, а вот и косинус.",
	u8"Не перепутайте с синусом!",
	u8"Вычтем это из $\\frac {\\pi} {2}$ и сведём задачу к предыдущей, уже решённой:",
	u8"МИНУС СИНУС!!!",
	u8"Это вообще очевидно, если вам не очевидно, сдвиньте график по иксу на $\\frac {3 \\pi} {2}$"
};
const char* const phrases_UNAR_sub[] =
{
	u8"Очевидно, что: (ну действительно очевидно же)",
	u8"Вот вроде формула сложная, а посмотрите, как красиво сворачивается: ",
	u8"Говорят, один программист не признавал унарный минус и ему приходилось дифференцировать произведение (-1) на это выражение",
	u8"Запускаю мини-тред: сколько лет вам было, когда вы узнали про отрицательные числа?",
	u8"Вынесем минус за знак производной:"
};
const char* const phrases_UNAR_sin[] =
{
	u8"А сколько будет синус пудовичка?",
	u8"Главное - не забыть домножить на производную внутренней функции.",
	u8"Ну что, покажем ему, как чётным быть?",
	u8"*подглядывает в таблицу производных*",
	u8"Не говорите кафедре вышмата, что",
};
const char* const phrases_UNAR_sinh[] =
{
	u8"Так, вроде выучил... Чёрт, так нужен минус или нет?",
	u8"Читатель может убедиться в справедливости формулы, выразив функцию через экспоненту: ",
	u8"Если там $log|cosh(x)|$, то я не знаю, что это такое",
	u8"А теперь применим небольшой фокус:",
	u8"Аналогично синусу,"
};
const char* const phrases_UNAR_log[] =
{
	u8"Обратная к возведению в степень операция - логарифм: ",
	u8"Под бурные аплодисменты, логарифм покидает нас.",
	u8"Не каждому дано познать производную логарифма:",
	u8"Логаводная произрифма:",
	u8"Ну и очередной логарифм:"
};
const char* const phrases_NUM[]
{
	u8"Здесь совсем всё легко.",
	u8"Первая строка таблицы производных гласит:",
	u8"Любопытно отметить, что верно данное тождество:"
};


#define fwr(str) fwrite(str, 1, (size_t)_countof(str) - 1, out)

FILE* MakeTemplate()
{
	srand((unsigned)time(nullptr));
	
	FILE* out = nullptr;
	if (fopen_s(&out, TeX_file, "w"))
	{
		printf("Error while opening %s\n", TeX_file); 
		return nullptr;
	}
	if (out) {
		fwr(
			u8"\\documentclass{article}\n"
			"\\usepackage[russian]{babel}\n"

			"\\title{ Лабораторная работа \"Вычисление производных элементарных функций\" }\n"
			"\\author{ Попов Алексей }\n"
			"\\begin{document}\n"
			"\\maketitle\n"

			"\\section{ С гордостью представляю вам: }\n"

			"Функция"

		);
		fwr("!\n\\begin{center}$");
	}
	
	return out;
}


inline void PrintDeriv(Tree* tree, Node* node, FILE* file)
{
	
	fputc('(', file);
	TeX_dump(tree, node, file);
	fputs(")\'", file);
}


void FuncPrint(FILE* out, Tree* tree, Node* root)
{
	TeX_dump(tree, root, out);

	fwr(
		u8"$\\end{center}\n\ Найдём её производную.\\\\\n"
		"\\section{ Дифференцирование }\n"
	);
}

void BeforeSimpify(Tree* tree, FILE* out)
{
	fwr(
		u8"\\section{ Анализ результатов }\n"
		"Многочасовая работа и километры исписанной бумаги позволили нам получить следующие результаты:"
	);
	fwr("\\\\\n\\begin{center}$");

	PrintDeriv(tree, &(tree->root), out);
	fwr("= ");
	TeX_dump(tree, &(tree->subroot), out);
	fwr(
		u8"$.\\end{center}\n\n"
		"На первый взгляд формула кажется объёмной. Но её можно упростить:\\\\\n"
	);
}

void End(FILE* out)
{
	fwr(u8"\\end{document}");
	fclose(out);
}

void Literature(FILE* out)
{
	fwr(
		u8"\\section{ Источники: }"
		"1) Ю.С.Рыбников. Счёт древних русов. НеМ: НеПросвещение, 2015 \\\\\n"
		"2) ГДЗ по матеше 5 класс - Н.Я.Виленкин. Сайт: https://gdz.ru/class-5/matematika/vilenkin/""\\\\\n"
		"3) Кричалки Красноярской летней школы - полное собрание. Красноярск: СФУ, 2020\\\\\n"
		"4) Дифференциальное и интегральное исчисления. Функции одной переменной. Л.Н.Знаменская. Где-то в Долгопрудном: в каком-то издательстве, когда-то в прошлом\\\\\n"

	);
	End(out);
}


void AfterSimplify(Tree* tree, FILE* out)
{
	fwr(
		u8"Окончательно имеем:"
	);
	fwr("\\\\\n\\begin{center}$");
	PrintDeriv(tree, &(tree->root), out);
	fwr(u8" = ");
	TeX_dump(tree, &(tree->subroot), out);
	fwr(
		u8"$.\\end{center}\n"
	);

	Literature(out);

}



//derivative of goes brrr



int getRandomNumber(int min, int max)
{
	static const double fraction = 1.0 / (static_cast<double>(RAND_MAX) + 1.0);
	
	return static_cast<int>(rand() * fraction * ((__int64)max - min + 1) + min);
}



#define FUNC(type)                                                                                 \
void RandPhrase_##type(FILE* file)                                                                 \
{                                                                                                  \
	fprintf(file, u8"%s\\\\", phrases_##type [getRandomNumber(0, _countof(phrases_##type) - 1)]);  \
}

FUNC(BINAR_mul)
FUNC(BINAR_add)
FUNC(BINAR_pow)
FUNC(UNAR_sub)
FUNC(UNAR_cos)
FUNC(UNAR_sin)
FUNC(UNAR_sinh)
FUNC(UNAR_log)
FUNC(NUM)

void RandPhrase_al(FILE * file)                                                           
{                                                                                            
	fprintf(file, u8"%s\\\\", phrases_al [getRandomNumber(0, _countof(phrases_al) - 1)]);
}
#undef FUNC



#define PHR(type)                \
if (getRandomNumber(0, 1) == 1)  \
	RandPhrase_al(out);          \
else                             \
	RandPhrase_##type(out);      \
fwr("\n\\begin{center}$");


void Top_BINAR_add(Tree* tree, Node* node, FILE* out)
{
	PHR(BINAR_add)
	USE_LVLDOWN_ITERATION

	PrintDeriv(tree, node, out); fputc(u8'=', out);

	DO_FOR_EACH_SON_excEND(node,
		fputc('(', out);
		TeX_dump(tree, *son, out);
		fputs(")\' + ", out);
	)

	son = GetByPhInd(lst, son_index);
	fputc(u8'(', out);
	TeX_dump(tree, *son, out);
	fputs(u8")\'$.\\end{center}", out);
}

void Top_BINAR_mul(Tree* tree, Node* node, FILE* out)
{
	PHR(BINAR_mul)
	USE_LVLDOWN_ITERATION

	PrintDeriv(tree, node, out); fputc('=', out);
	TeX_dump(tree, node, out);
	fputs(u8"\\cdot (", out);

	DO_FOR_EACH_SON_excEND(node,
		fputs(u8"Log(", out);
		TeX_dump(tree, *son, out);
		fputs(u8")\' + ", out);
	)

	son = GetByPhInd(lst, son_index);
	fputs(u8"log(", out);
	TeX_dump(tree, *son, out);
	fputs(u8")\')$.\\end{center}\n", out);
}

void Top_BINAR_pow(Tree* tree, Node* node, FILE* out)
{
	PHR(BINAR_pow)

	PrintDeriv(tree, node, out); 
	fputc('=', out);
	TeX_dump(tree, node, out);
	fwr(u8" \\cdot (");

	node->ind_l = GetNext(lst, node->ind_l);
	PrintDeriv(tree, node, out);
	node->ind_l = GetPrev(lst, node->ind_l);

	fwr(u8"\\cdot log(");
	TeX_dump(tree, L(node), out);
	fwr(") + ");

	node->ind_l = GetNext(lst, node->ind_l);
	TeX_dump(tree, node, out);
	node->ind_l = GetPrev(lst, node->ind_l);

	fwr(u8" \\cdot (log");
	TeX_dump(tree, L(node), out);
	fwr(u8")\')$.\\end{center}\n");
}

void Top_UNAR_sub(Tree* tree, Node* node, FILE* out)
{
	PHR(UNAR_sub)

	PrintDeriv(tree, node, out);

	fwr("= -(");
	TeX_dump(tree, L(node), out);
	fwr(")$.\\end{center}\n");
}

void Top_UNAR_sin(Tree* tree, Node* node, FILE* out)
{
	PHR(UNAR_sin)

	PrintDeriv(tree, node, out);

	fwr("= cos(");
	TeX_dump(tree, L(node), out);
	fwr(")$.\\end{center}\n");
}

void Top_UNAR_cos(Tree* tree, Node* node, FILE* out)
{
	PHR(UNAR_cos)

	PrintDeriv(tree, node, out);

	fwr("= -sin(");
	TeX_dump(tree, L(node), out);
	fwr(")$.\\end{center}\n");
}

void Top_UNAR_sinh(Tree* tree, Node* node, FILE* out)
{
	PHR(UNAR_sinh)

	PrintDeriv(tree, node, out);

	fwr("= cosh(");
	TeX_dump(tree, L(node), out);
	fwr(")$.\\end{center}\n");
}

void Top_UNAR_cosh(Tree* tree, Node* node, FILE* out)
{
	PHR(UNAR_sinh)

	PrintDeriv(tree, node, out);

	fwr("= sinh(");
	TeX_dump(tree, L(node), out);
	fwr(")$.\\end{center}\n");
}

void Top_UNAR_log(Tree* tree, Node* node, FILE* out)
{
	PHR(UNAR_log)

	PrintDeriv(tree, node, out);

	fwr("= \\frac { ");

	PrintDeriv(tree, L(node), out);

	fwr("} {");

	TeX_dump(tree, L(node), out);

	fwr("}$.\\end{center}\n");
}

void Top_NUM(Tree* tree, Node* node, FILE* out)
{
	PHR(NUM)

	PrintDeriv(tree, node, out);

	if (node->type == VAR && node->value == 'x') 
		fwr(" = 1$.\\end{center}\n");
	else
		fwr(" = 0$.\\end{center}\n");
}

#define FUNC(type)                                                   \
void Bottom_##type(Tree* tree, Node* func, Node* result, FILE* out)  \
{                                                                    \
	PHR(##type)                                                      \
	PrintDeriv(tree, func, out);                                     \
	fwr("= ");                                                       \
	TeX_dump(tree, result, out);                                     \
	fwr("$.\\end{center}\n");                                        \
}

FUNC(BINAR_add)
FUNC(BINAR_mul)
FUNC(BINAR_pow)
FUNC(UNAR_sin)
FUNC(UNAR_sinh)
FUNC(UNAR_cos)
FUNC(UNAR_sub)
FUNC(UNAR_log)

void Bottom_UNAR_cosh(Tree* tree, Node* func, Node* result, FILE* out)
{                                                                    
	Bottom_UNAR_sinh(tree, func, result, out);
}
#undef FUNC

