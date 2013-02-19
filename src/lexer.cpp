#include "lexer.h"

lexer::lexer() {
    //ctor
}

lexer::~lexer() {
    //dtor
}
/**
    Хеш функция Дженкинса
*/
uint32_t lexer::hash(const char *key, size_t len) {
    uint32_t hash, i;
    for(hash = i = 0; i < len; ++i) {
        hash += key[i];
        hash += (hash << 10);
        hash ^= (hash >> 6);
    }
    hash += (hash << 3);
    hash ^= (hash >> 11);
    hash += (hash << 15);
    return hash;
}
