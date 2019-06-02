#ifndef trending_topic_H
#define trending_topic_H

#include <stdio.h>
#include "hash.h"

typedef struct trending_topic trending_topic_t;
typedef struct hashtag hashtag_t;

hashtag_t* hashtag_crear(const char *hashtag_char, countMinSketch_t* countMinSketch);

unsigned int hashtag_repeticiones(hashtag_t* hashtag);

void hashtag_destruir(hashtag_t* hashtag);

int hashtag_cmp(const void *a, const void *b);


trending_topic_t* trending_topic_crear(size_t cantidad, countMinSketch_t* countMinSketch);

void trending_topic_destruir(trending_topic_t* trending_topic);

void trending_topic_comparar(const char *hashtag_actual, trending_topic_t* trending_topic, countMinSketch_t* countMinSketch);

void trending_topic_recorrer(trending_topic_t* trending_topic, countMinSketch_t* countMinSketch);

#endif // trending_topic_H