#ifndef STRUTIL_H
#define STRUTIL_H

#include <stddef.h>


/*
 * Devuelve en un arreglo dinámico terminado en NULL todos los subsegmentos de
 * ‘str’ separados por el carácter ‘sep’. Tanto el arreglo devuelto como las
 * cadenas que contiene se ubicarán en nuevos espacios de memoria dinámica.
 *
 * La liberación de la memoria dinámica devuelta queda a cargo de quien llame a
 * esta función. La función devuelve NULL en caso de error.
 */
char **split(const char *str, char sep);

/*
 * Libera un arreglo dinámico de cadenas, y todas las cadenas que contiene.
 */
void free_arreglo_split(char *strv[]);

#endif  // STRUTIL_H