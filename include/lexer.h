#ifndef LEXER_H
#define LEXER_H

#include "types.h"
#include <vector>
#include <string.h>

#include <unicode/unistr.h>

typedef std::pair <Hash32, std::string> lemma;
typedef std::vector <lemma> lemma_list;

class lexer {
    public:
        lexer();
        virtual ~lexer();
        virtual lemma_list parse(const UnicodeString str) = 0;
    protected:
        static uint32_t hash(const char *key, size_t len);
    private:
};

#endif // LEXER_H
