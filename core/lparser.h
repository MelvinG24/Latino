#ifndef _LPARSER_H_
#define _LPARSER_H_

#include "latino.h"
#include "lio.h"
#include "llex.h"


typedef struct parser_state{

}parser_state;

int parser_init(lstring path);
int lparser(lex_state *ls);
#endif /* _LPARSER_H_ */
