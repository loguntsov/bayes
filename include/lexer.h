#ifndef LEXER_H
#define LEXER_H

#include "types.h"
#include <queue>
#include <string.h>

#include <unicode/unistr.h>

typedef struct {
    Hash32 hash;
    unsigned int weight;
    std::string name;
} lemma;

typedef std::queue <lemma> lemma_queue;

class lexer {
    public:
        lexer();
        virtual ~lexer();
        virtual void parse(const UnicodeString str) = 0;
        virtual void parse_begin();
        virtual void parse_end();
        lemma get_next_lemma();
        bool lemma_eof() const;
        void word_stat(WordsStat &stats);
    protected:
        lemma_queue lemmas;
        static uint32_t hash(const char *key, size_t len);
    private:
};

#endif // LEXER_H
