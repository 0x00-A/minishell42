# mini-shell
creat a simple shell. using c langage 
h

expression : simple_command redirection pipeline
           | redirection simple_command pipeline

simple_command : WORD arguments

arguments : WORD arguments
          | empty

redirection :  WORD '>'
            | '<' WORD | '<<' EOF | DATA '>>' WORD | empty

pipeline : '|' expression
         | empty
