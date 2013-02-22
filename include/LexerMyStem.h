#ifndef LEXERMYSTEM_H
#define LEXERMYSTEM_H

#include "lexer.h"
#include <unicode/regex.h>
#include <queue>
#include <list>
#include "child.h"

/**
    Лексер для распарсивания ответов от программы mystem (Яндекс)
*/

class LexerMyStem : public lexer {
    typedef std::queue <std::list <std::string> > lemmas_q;

    public:
        LexerMyStem(std::string mystem_path, std::string match = "");
        unsigned char queue_size;
        void parse_begin();
        void parse_end();
        virtual ~LexerMyStem();
        virtual void parse(const UnicodeString str);
    protected:
        child *mystem;
        void generate_lemmas(unsigned int min_size);
        void parse_buffer();
        void lemma_mul(lemmas_q &q);
        RegexMatcher *matcher;
        RegexMatcher *filter;
        lemmas_q queue;
        UnicodeString buffer;
    private:
};

#endif // LEXERMYSTEM_H
