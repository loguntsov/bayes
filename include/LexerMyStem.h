#ifndef LEXERMYSTEM_H
#define LEXERMYSTEM_H

#include "lexer.h"
#include <unicode/regex.h>
#include <queue>
#include <vector>
#include "child.h"

/**
    Лексер для распарсивания ответов от программы mystem (Яндекс)
*/

class LexerMyStem : public lexer {
    typedef std::queue <std::vector <std::string> > lemmas_q;

    public:
        LexerMyStem();
        unsigned char queue_size;
        void parse_begin();
        void parse_end();
        virtual ~LexerMyStem();
        virtual void parse(const UnicodeString str);
    protected:
        child *mystem;
        void generate_lemmas();
        void lemma_mul(lemmas_q &q);
        RegexMatcher *matcher;
        lemmas_q queue;
    private:
};

#endif // LEXERMYSTEM_H
