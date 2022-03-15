# 6. Derivatives solver

The task is to write a program for diffirentiating function given with text format expression. This includes
- recursive descent parsing of expression to represent function as a tree, whose nodes are either operations (+, -, /, sinh, etc) or numbers or variables
- differentiating the tree
- simplify the tree to convert, for example, log(exp(f(x))) into f(x), or 1+2+3+0 into 6 
- print the resulting function
- generating .tex file with "laboratory work about differentiating functions"

example.tex and example.pdf are examples of program output.
