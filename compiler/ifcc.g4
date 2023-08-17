grammar ifcc;

axiom : prog ;

prog : functionList;

functionList : function+
             ;
             
function : TYPE VAR '(' parametersList ')' bloc ;

bloc : '{' instructionsList '}'
     ;

instructionsList : instruction instructionsList
                | instruction
                ;

instruction : declaration ';'
            | affectation ';'
            | RETURN expr ';'
            | ifStatement
            | whileStatement
            | function
            ;

declaration : TYPE varPar (',' varPar)*;

varPar : VAR
       | '(' VAR ')'
       ;

affectation : VAR EGAL functionCall          # affectationSimpleFunction
            | declaration EGAL functionCall  # affectationDeclarationFunction
            | VAR EGAL expr                  # affectationSimple
            | declaration EGAL expr          # affectationDeclaration
            ;

ifStatement : 'if' '(' expr ')' bloc ( 'else' bloc )? ;

whileStatement : 'while' '(' expr ')' bloc ;


parametersList : parameter (',' parameter)*
                | 
                ;

parameter : TYPE VAR ;

expr : expr prodoper expr            # prodop
     | expr sumoper expr             # sumop
     | expr binoper expr             # binop
     | expr compoper expr            # compop
     | MINUS constante               # negConst
     | MINUS expr                    # neg
     | NOT expr                      # not
     | constante                     # const
     | VAR                           # var
     | '(' expr ')'                  # par
     | functionCall                  # funcCall
     ;

functionCall : VAR '('  ')' ;

argumentsList : expr (',' expr)*
                | 
                ;

sumoper : MINUS | PLUS ;
prodoper : MULTIPLY | DIVIDE ;
binoper : AND | OR | XOR ;
compoper : EQUAL | NOTEQUAL | LESS | LESSEQUAL | GREATER | GREATEREQUAL;
constante : NUMBER            # num
          | CHAR              # char
          ;

RETURN : 'return' ;
TYPE : 'int' | 'char' | 'void';
DIVIDE : '/' ;
MULTIPLY : '*' ;
MINUS : '-' ;
NOT : '!' ;
PLUS  : '+' ;
AND : '&' ;
OR : '|' ;
XOR : '^' ;
EGAL : '=' ;
EQUAL : '==' ;
NOTEQUAL : '!=' ;
LESS : '<' ;
LESSEQUAL : '<=' ;
GREATER : '>' ;
GREATEREQUAL : '>=' ;
NUMBER : [0-9]+ ;
CHAR : '\''[a-zA-Z]'\'';
VAR :  [a-zA-Z][0-9a-zA-Z_]* ;
COMMENT : '/*' .*? '*/' -> skip ;
DIRECTIVE : '#' .*? '\n' -> skip ;
WS    : [ \t\r\n] -> channel(HIDDEN);
