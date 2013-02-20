#include "lexer.h"

lexer::lexer() {
    //ctor
}

lexer::~lexer() {
    //dtor
}

void lexer::parse_begin() {
    while(!this->lemmas.empty()) this->lemmas.pop();
    return;
}

void lexer::parse_end() {
    return;
}

lemma lexer::get_next_lemma() {
    lemma l = this->lemmas.front();
    this->lemmas.pop();
    return l;
}

bool lexer::lemma_eof() const {
    return this->lemmas.empty();
}

void lexer::word_stat(WordsStat &stats) {
    while(!this->lemmas.empty()) {
        lemma L = this->get_next_lemma();
        if (stats.find(L.hash) == stats.end()) {
            stats[L.hash] = L.weight;
        } else {
            stats[L.hash]+= L.weight;
        }
    }
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
