#ifndef LEXERSTEM_H
#define LEXERSTEM_H

#include "lexer.h"
#include "libstemmer.h"
#include <unicode/regex.h>

class LexerStem : public lexer {
    public:
        LexerStem();
        virtual ~LexerStem();
        virtual lemma_list parse(const UnicodeString str);
    protected:
        bool is_stop_word(const std::string word) const;
        sb_stemmer* stemmer_ru;
        sb_stemmer* stemmer_en;
        RegexMatcher *matcher;
    private:
};

#endif // LEXERSTEM_H
