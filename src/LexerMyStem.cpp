#include "LexerMyStem.h"
#include <iostream>

#include <unistd.h>
#include <stdio.h>

#include "child.h"
#include <assert.h>


using namespace std;

LexerMyStem::LexerMyStem(string mystem_path, string match) {
    UErrorCode status = U_ZERO_ERROR;
    this->matcher = new RegexMatcher("=((A|ADV|ADVPRO|ANUM|APRO|COM|CONJ|INTJ|NUM|PART|PR|S|SPRO|V)(.+?))([|]{1}|$)",0, status);
    assert( status == U_ZERO_ERROR);

    status = U_ZERO_ERROR;
    this->filter = (match == "") ? NULL : new RegexMatcher(match.c_str(), UREGEX_CASE_INSENSITIVE|UREGEX_UWORD, status);
    assert( status == U_ZERO_ERROR);

    this->queue_size = 1;
    const string path = string(mystem_path + "mystem -e utf8 -n -i -l");
    this->mystem = new child(path.c_str());
}

LexerMyStem::~LexerMyStem() {
    delete this->matcher;
    delete this->filter;
    delete this->mystem;
}

void LexerMyStem::parse_begin() {
    while(!this->queue.empty()) this->queue.pop();
    this->buffer = "";
    lexer::parse_begin();
}

void LexerMyStem::parse_end() {
    this->parse_buffer();
    this->generate_lemmas(this->queue_size);
    lexer::parse_end();
}

void LexerMyStem::parse(const UnicodeString str) {

    UnicodeString text = str;
    text.trim();

    if (text == "") return;

    this->buffer += (text + " ");

    if (this->buffer.length() < 2048) return;

    this->parse_buffer();

}

void LexerMyStem::parse_buffer() {

    if (this->buffer.length() < 1) return;

    string utf8;

    this->buffer.toUTF8String(utf8);
    this->buffer = "";

    stringstream *result = this->mystem->result(utf8);

    string line;

    while (!result->eof()) {

        std::getline(*result,line);
        //cout << "line: " << line << endl;
        if (line == "") continue;

        UnicodeString unicode = UnicodeString::fromUTF8(line);

        this->matcher->reset(unicode);
        list <string> list;

        while(this->matcher->find()) {
            UErrorCode status = U_ZERO_ERROR;
            UnicodeString lexema = this->matcher->group(1,status);


            if (this->filter != NULL) {
                this->filter->reset(lexema);
                while(this->filter->find()) {
                    string str;
                    this->filter->group(status).toUTF8String(str);

                    if (str == "") continue;
                    cout << str << endl;
                    list.push_back(str);
                }
            } else {
                string str;
                lexema.toUTF8String(str);
                if (str == "") continue;
                cout << str << endl;
                list.push_back(str);
            }
        }
        if (list.size() > 0) {
            this->queue.push(list);
            cout << endl;
        }
    }
    this->generate_lemmas(this->queue_size);
}

void LexerMyStem::generate_lemmas(unsigned int min_size) {
    if (this->queue.size()<min_size) return;

     while(this->queue.size() >= min_size) {
        lemmas_q q = this->queue;
        this->lemma_mul(q);
        this->queue.pop();
    }
}

void LexerMyStem::lemma_mul(lemmas_q &q) {

    if (q.size() == 0) return;

    list <string> Result = q.front();
    q.pop();
    int n = 1;
    while(q.size() > 0 && n < this->queue_size) {
        n++;
        list <string> Res0;
        list <string> Res1 = q.front();
        q.pop();
        for(list <string>::const_iterator i = Result.begin(); i != Result.end(); i++) {
            for(list <string>::const_iterator j = Res1.begin(); j != Res1.end(); j++ ) {
                Res0.push_back(*j + string("**") + *i);
            }
        }
        Result = Res0;
    }
    for(list <string>::const_iterator i = Result.begin(); i != Result.end(); i++) {
        this->lemmas.push(this->create_lemma(*i,1));
        cout << "+++" << *i << endl;
    }
    cout << endl;
}
