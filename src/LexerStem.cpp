#include "LexerStem.h"
#include <set>

using namespace std;

LexerStem::LexerStem() : lexer() {
    this->stemmer_ru = sb_stemmer_new("russian","UTF_8");
    this->stemmer_en = sb_stemmer_new("english","UTF_8");
    UErrorCode status = U_ZERO_ERROR;
    this->matcher = new RegexMatcher("([a-zа-я])+", 0, status);
}

LexerStem::~LexerStem() {
    sb_stemmer_delete(this->stemmer_ru);
    sb_stemmer_delete(this->stemmer_en);
    delete this->matcher;
}

void LexerStem::parse(const UnicodeString str) {

    this->matcher->reset(str);
    while (this->matcher->find()) {
        std::string str, str0;
        UErrorCode status = U_ZERO_ERROR;
        UnicodeString uWord = this->matcher->group(status);

        uWord.toUTF8String(str);

        if (this->is_stop_word(str)) continue;

        const char *s = str.c_str();
        const unsigned int n = str.length();
        const char *stem = (char * ) sb_stemmer_stem(stemmer_ru, sb_stemmer_stem(stemmer_en, (sb_symbol *) s,n), n);
        this->lemmas.push(this->create_lemma(string(stem), 1));
    }
}

bool LexerStem::is_stop_word(const string word) const {
	static set <string> stop_words;
	if (stop_words.size() == 0) {
		static string words[] = {"the","an","from","to","by","against","about","of","above","below","since","with","after","before","as",
		"из","на","под", "за","из-за","над","без","для","про","через",
		""};
		unsigned int i = 0;
		string *w;
		while( *(w = &words[i]) != "")  {
			stop_words.insert(*w);
			i++;
		}
	}

	return stop_words.find(word) != stop_words.end();
}

