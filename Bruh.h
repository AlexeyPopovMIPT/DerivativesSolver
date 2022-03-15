#pragma once
#ifndef INC_APDIFBRH
#define INC_APDIFBRH

#define FUNCS(type)                                       \
extern void Top_##type(Tree*, Node*, FILE*);              \
extern void Bottom_##type(Tree*, Node*, Node*, FILE*);

FUNCS(BINAR_mul)
FUNCS(BINAR_add)
FUNCS(BINAR_pow)
FUNCS(UNAR_sub)
FUNCS(UNAR_cos)
FUNCS(UNAR_sin)
FUNCS(UNAR_sinh)
FUNCS(UNAR_log)
FUNCS(NUM)

#undef FUNCS

extern FILE* MakeTemplate (void);
extern void  FuncPrint    (FILE* out, Tree* tree, Node* root);
extern void  Literature   (FILE* out);
extern void  End          (FILE* out);
extern void  BeforeSimpify(Tree* tree, FILE* out);
extern void  AfterSimplify (Tree* tree, FILE* out);

#endif //!INC_APDIFBRH

