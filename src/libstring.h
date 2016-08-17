/*
The MIT License (MIT)

Copyright (c) 2015 - Latino

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
*/

#ifndef _LIBSTRING_H_
#define _LIBSTRING_H_

#include "latino.h"
#include "object.h"
#include "vm.h"

/** \file libstring.h
*
* Contiene las funciones para el manejo de cadenas
*
*/

/** Guarda un cadena en hashtable
  *
  * \param p: Apuntador a la cadena a guardar
  * \param len: Longitud de la cadena
  * \return lat_objeto: Un objeto tipo cadena
  *
  */
lat_objeto* __str_cadena_hash(const char* p, size_t len);

/**\brief Duplica una cadena
  *
  * \param s: Apuntador a una cadena de caracteres
  * \return char*: Apuntador a una cadena de caracteres
  */
char* __str_duplicar(const char* s);

/**\brief Analiza una cadena de caracteres
  *
  * \param s: Apuntador a una cadena de caracteres
  * \param n: Longitud de la cadena
  * \return char*: Apuntador a una cadena de caracteres
  */
char* __str_analizar(const char* s, size_t n);

/**\brief concatena dos cadenas
  *
  *\param s1: Cadena 1 a concatena
  *\param s2: Cadena 2 a concatena
  *\return char*: Apuntador a cadena resultado de s1 + s2
  */
char* __str_concatenar(char* s1, char* s2);

/**\brief Convierte un entero a cadena
  *
  *\param i: Valor entero largo
  *\return char*: Apuntador a cadena
  */
char* __str_entero_a_cadena(long i);

/**\brief Convierte un decimal a cadena
  *
  *\param d: Valor decimal
  *\return char*: Apuntador a cadena
  */
char* __str_numerico_a_cadena(double d);

/**\brief Convierte un caracter a cadena
  *
  *\param c: Valor del caracter
  *\return char*: Apuntador a cadena
  */
char* __str_caracter_a_cadena(char c);

/**\brief Convierte un valor logico
  *
  *\param i: Valor logico 0, 1
  *\return char*: Apuntador a cadena
  */
char* __str_logico_a_cadena(int i);

/**\brief Valida si una cadena inicia con
  *
  *\param base: Cadena original
  *\param str: Cadena a buscar
  *\return bool: true si la cadena base inicia con el valor de str
  */
bool __str_empieza_con(const char* base, const char* str);

/**\brief Valida si una cadena termina con
  *
  *\param base: Cadena original
  *\param str: Cadena a buscar
  *\return bool: true si la cadena base termina con el valor de str
  */
bool __str_termina_con(char* base, char* str);

/**\brief Regresa el indice en donde se encontro la cadena str
  *
  *\param base: Cadena original
  *\param str: Cadena a buscar
  *\return int: Indice dentro de cadena original
  */
int __str_posicion(char* base, char* str);

/**\brief Regresa el indice en donde se encontro la cadena str a partir de una posicion
  *
  *\param base: Cadena original
  *\param str: Cadena a buscar
  *\param startIndex: Indice inicial
  *\return int: Indice dentro de cadena original
  */
int __str_intercambiar_posicion(char* base, char* str, int startIndex);

/**\brief Regresa el ultimo indice en donde se encontro la cadena str
  *
  *\param base: Cadena original
  *\param str: Cadena a buscar
  *\return int: Indice dentro de cadena original
  */
int __str_ultima_posicion(char* base, char* str);

/**\brief __str_insertara una subcadena dentro de una cadena a partir de una posicion
  *
  *\param dest: Cadena destino
  *\param src: Cadena a insertarar
  *\param pos: Posicion en la que se insertara la cadena
  *\return char*: Apuntador a la cadena resultante
  */
char* __str_insertar(char *dest, char* src, int pos);

/**\brief Rellena el espacio izquierdo de una cadena con un caracter especificado
  *
  *\param base: Cadena original
  *\param n: Longitud de la cadena resultante
  *\param c: Caracter con el que se rellenara
  *\return char*: Apuntador a la cadena resultante
  */
char* __str_rellenar_izquierda(char* base, int n, char* c);

/**\brief Rellena el espacio derecho de una cadena con un caracter especificado
  *
  *\param base: Cadena original
  *\param n: Longitud de la cadena resultante
  *\param c: Caracter con el que se rellenara
  *\return char*: Apuntador a la cadena resultante
  */
char* __str_rellenar_derecha(char *base, int n, char* c);

/**\brief Reemplaza una subcadena con otra subcadena
  *
  *\param str: Cadena original
  *\param orig: Cadena a reemplazar
  *\param rep: Cadena reemplazo
  *\return char*: Apuntador a la cadena resultante
  */
char *__str_reemplazar(char *str, char *orig, char *rep);

/**\brief Crea una subcadena especificando posicion inicial y longitud
  *
  *\param str: Cadena original
  *\param beg: Posicion de inicio
  *\param n: Longitud de la cadena resultante
  *\return char*: Apuntador a la cadena resultante
  */
char *__str_subcadena(const char* str, int beg, int n);

/**\brief Convierte a minusculas la cadena
  *
  *\param str: Cadena original
  *\return char*: Apuntador a la cadena resultante
  */
char *__str_minusculas(const char* str);

/**\brief Convierte a mayusculas la cadena
  *
  *\param str: Cadena original
  *\return char*: Apuntador a la cadena resultante
  */
char *__str_mayusculas(const char* str);

/**\brief Elimina espacios vacios al inicio y fin de la cadena
  *
  *\param str: Cadena original
  *\return char*: Apuntador a la cadena resultante
  */
char *__str_quitar_espacios(const char* str);
/** Comparar: compara dos cadenas
  * valor de retorno	indica
  * -------------------------------
  * <0	|   la cadena 1 es menor que la cadena 2
  * 0	|   las dos cadena son iguales
  * >0	|   la cadena 2 es mayor que la cadena 2
  *
  * \param vm: Maquina virtual de latino
  *
  */
void lat_comparar(lat_vm* vm);

/** concatenar: Une dos cadenas
  *
  * \param vm: Maquina virtual de latino
  *
  */
void lat_concatenar(lat_vm* vm);

/** Contiene: Valida si una cadena contiene una subcadena determinada
  *
  * \param vm: Maquina virtual de latino
  *
  */
void lat_contiene(lat_vm* vm);

/** Copiar: Copia el contenido de una cadena
  *
  * \param vm: Maquina virtual de latino
  *
  */
void lat_copiar(lat_vm* vm);

/** Termina con: Valida si una cadena termina con una subcadena determinada
  *
  * \param vm: Maquina virtual de latino
  *
  */
void lat_termina_con(lat_vm* vm);

/** Es igual a: Valida si dos cadenas son iguales
  *
  * \param vm: Maquina virtual de latino
  *
  */
void lat_es_igual(lat_vm* vm);
//void lat_format(lat_vm* vm);

/** Indice: Regresa el indice de una subcadena determinada
  *
  * \param vm: Maquina virtual de latino
  *
  */
void lat_indice(lat_vm* vm);

/** lat_insertar: inserta una subcadena en una cadena en el indice indicado
  *
  * \param vm: Maquina virtual de latino
  *
  */
void lat_insertar(lat_vm* vm);

/** Ultimo indice: Regresa la ultima posicion en la que se encontro una subcadena
  *
  * \param vm: Maquina virtual de latino
  *
  */
void lat_ultimo_indice(lat_vm* vm);

/** Rellenar izquierda: Rellena una cadena con caracteres a la izquierda
  *
  * \param vm: Maquina virtual de latino
  *
  */
void lat_rellenar_izquierda(lat_vm* vm);

/** Rellenar derecha: Rellena una cadena con caracteres a la derecha
  *
  * \param vm: Maquina virtual de latino
  *
  */
void lat_rellenar_derecha(lat_vm* vm);

/** Eliminar: Elimina una subacadena determinada
  *
  * \param vm: Maquina virtual de latino
  *
  */
void lat_eliminar(lat_vm* vm);

/** Esta vacia: Valida si una cadena esta vacia
  *
  * \param vm: Maquina virtual de latino
  *
  */
void lat_esta_vacia(lat_vm* vm);

/** Longitud: Regresa la longitud de una cadena
  *
  * \param vm: Maquina virtual de latino
  *
  */
void lat_longitud(lat_vm* vm);

/** Reemplazar: Reemplaza una subcadena por otra subcadena
  *
  * \param vm: Maquina virtual de latino
  *
  */
void lat_reemplazar(lat_vm* vm);
//void lat_split(lat_vm* vm);

/** Empieza con: Valida si una cadena inicia con una subcadena determinada
  *
  * \param vm: Maquina virtual de latino
  *
  */
void lat_empieza_con(lat_vm* vm);

/** Subcadena: Crea una subcadena
  *
  * \param vm: Maquina virtual de latino
  *
  */
void lat_subcadena(lat_vm* vm);

/** Minusculas: Convierte a minusculas toda la cadena
  *
  * \param vm: Maquina virtual de latino
  *
  */
void lat_minusculas(lat_vm* vm);

/** Mayusculas: Convierte a mayusculas toda la cadena
  *
  * \param vm: Maquina virtual de latino
  *
  */
void lat_mayusculas(lat_vm* vm);

/** Quitar espacios: Quita los espacios vacios al inicio y fin de la cadena
  *
  * \param vm: Maquina virtual de latino
  *
  */
void lat_quitar_espacios(lat_vm* vm);
//void lat_valueOf(lat_vm* vm);

/** Es numero: Valida si una cadena es un numero
  *
  * \param vm: Maquina virtual de latino
  *
  */
void lat_es_numero(lat_vm* vm);

#endif // !_LIBSTRING_H_
