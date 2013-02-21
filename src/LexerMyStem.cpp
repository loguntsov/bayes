#include "LexerMyStem.h"
#include <iostream>

#include <unistd.h>
#include <stdio.h>

#include "child.h"


using namespace std;

LexerMyStem::LexerMyStem() {
    UErrorCode status = U_ZERO_ERROR;
    this->matcher = new RegexMatcher("=(.+?)([|]{1}|$)",0, status);
    this->queue_size = 1;

    this->mystem = new child("./mystem -e utf8 -n -i -l");
}

LexerMyStem::~LexerMyStem() {
    delete this->matcher;
    delete this->mystem;
}

void LexerMyStem::parse_begin() {
    while(!this->queue.empty()) this->queue.pop();
    lexer::parse_begin();
}

void LexerMyStem::parse_end() {
    this->lemma_mul(this->queue);
    lexer::parse_end();
}

void LexerMyStem::parse(const UnicodeString str) {
    string utf8;
    UnicodeString text = str;
    text.trim();

    if (text == "") return;

    text.toUTF8String(utf8);

    stringstream *result = this->mystem->result(utf8);

    string line;

    do {

        std::getline(*result,line);
        //cout << line << endl;
        if (line == "") break;
        UnicodeString unicode = UnicodeString::fromUTF8(line);

        this->matcher->reset(unicode);
        vector <string> list;

        while(this->matcher->find()) {
            UErrorCode status = U_ZERO_ERROR;
            string str;
            this->matcher->group(1,status).toUTF8String(str);
            //cout << str << endl;
            list.push_back(str);
        }
        this->queue.push(list);

    } while(true);
    this->generate_lemmas();
}

void LexerMyStem::generate_lemmas() {
    if (this->queue.size()<this->queue_size) return;

     while(this->queue.size() > this->queue_size) {
        lemmas_q q = this->queue;
        this->lemma_mul(q);
        this->queue.pop();
    }
}

void LexerMyStem::lemma_mul(lemmas_q &q) {

    vector <string> Result;
    int n = 0;
    while(q.size() > 0 && n < this->queue_size) {
        n++;
        vector <string> Res0,Res1 = q.front();
        q.pop();
        if (Result.empty()) {
            Result = Res1;
            continue;
        }

        for(vector <string>::const_iterator i = Result.begin(); i != Result.end(); i++) {
            for(vector <string>::const_iterator j = Res1.begin(); j != Res1.end(); j++ ) {
                Res0.push_back(*j + string("**") + *i);
            }
        }

        Result = Res0;
    }

    for(vector <string>::const_iterator i = Result.begin(); i != Result.end(); i++) {
        this->lemmas.push(this->create_lemma(*i,1));
        //cout << *i << endl;
    }

}
