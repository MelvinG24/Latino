#ifndef _LLEX_H_
#define _LLEX_H_

#include "latino.h"
#include "lio.h"

#define FIRST_RESERVED 256

enum RESERVED {
    TK_SI = FIRST_RESERVED,
    TK_SINO,
    TK_FIN,
    TK_ELEGIR,
    TK_CASO, /*260*/
    TK_DEFECTO,
    TK_DESDE,
    TK_HASTA,
    TK_CUANDO,
    TK_SALTO,
    TK_MIENTRAS,
    TK_HACER,
    TK_ROMPER,
    TK_CONTINUAR,
    TK_FUNCION, /*270*/
    TK_RETORNO,
    TK_VERDADERO,
    TK_FALSO,
    TK_CLASE,
    TK_PROPIEDAD,
    TK_CONSTRUCTOR,
    TK_ESTA,
    /* data types */
    TK_CARACTER,
    TK_ENTERO,
    TK_DECIMAL, /*280*/
    TK_CADENA,
    TK_IDENT,
    /* compare operators */
    TK_IGUALDAD,
    TK_DIFERENTE,
    TK_MENOR_IGUAL,
    TK_MAYOR_IGUAL,
    TK_Y_LOGICO,
    TK_O_LOGICO,
    TK_EOL,     /* end of line */
    TK_EOS      /*289*/
};

typedef union {
    int r;
    lstring ts;
} semantic;

typedef struct token {
    int token;
    semantic sem_info;
} token;

typedef struct lex_state {
    int current; /* current character */
    int linenumber; /* line counter */
    int colnumber; /* column counter */
    int pos; /* position in buffer */
    int lastline; /* line of last token consumed */
    token currtoken; /* current token */
    token lookahead; /* look ahead token */
    lbuffer *inputfile; /* input stream */
} lex_state;

LAT_FUNC lex_state *lex_init();
LAT_FUNC void lex_destroy(lex_state *ls);
LAT_FUNC void lex_next(lex_state *ls);
LAT_FUNC int lex_lookahead(lex_state *ls);

#endif /*_LLEX_H_*/
