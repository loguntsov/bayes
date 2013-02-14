
#include <iostream>
#include <string.h>
#include <locale.h>
#include "libstemmer.h"
#include <unicode/unistr.h>
#include <unicode/regex.h>

#include <vector>
#include <map>
#include <stdlib.h>
#include <assert.h>
#include <sstream>

#include "BayesClassifier.h"

#include <fstream>
#include <math.h>
#include <algorithm>
#include <set>

using namespace std;

uint32_t jenkins_one_at_a_time_hash(const char *key, size_t len) {
    uint32_t hash, i;
    for(hash = i = 0; i < len; ++i)
    {
        hash += key[i];
        hash += (hash << 10);
        hash ^= (hash >> 6);
    }
    hash += (hash << 3);
    hash ^= (hash >> 11);
    hash += (hash << 15);
    return hash;
}

typedef struct {
		unsigned int counter;
		string word;
	} WordInfo;
typedef map <Hash32, WordInfo> WordList;

bool isStopWord(string word) {
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


int learn(string file) {
    sb_stemmer* stemmer_ru = sb_stemmer_new("russian","UTF_8");
    sb_stemmer* stemmer_en = sb_stemmer_new("english","UTF_8");

    UErrorCode status = U_ZERO_ERROR;
    RegexMatcher matcher("([a-zа-я])+", 0, status);
    RegexMatcher header("^----class ([\\d]+) -----$", 0, status);

    WordList words_all;
    ClassifierList classifier;
    ClassifierList::iterator words = classifier.end();

    while (!cin.eof()) {
        string buf;
        std::getline(cin, buf);
        UnicodeString ucs = UnicodeString::fromUTF8(buf.c_str());
		header.reset(ucs);
        if (header.find()) {
        	string str;
        	header.group(1,status).toUTF8String(str);
        	int cl = atoi(str.c_str());
        	if (classifier.find(cl) == classifier.end()) {
        		WordsStat stat;
        		classifier[cl] = stat;
        	}
			words = classifier.find(cl);
			continue;
        }

        assert(words != classifier.end());

        ucs.toLower();

        Hash32 hash;

        matcher.reset(ucs);
        while (matcher.find()) {
            string str, str0;
            UnicodeString uWord = matcher.group(status);
            if (uWord.length() < 2) continue;
            uWord.toUTF8String(str);

			if (isStopWord(str)) continue;

            const char *s = str.c_str();
            const unsigned int n = str.length();
            const char *stem = (char * ) sb_stemmer_stem(stemmer_ru, sb_stemmer_stem(stemmer_en, (sb_symbol *) s,n), n);
            hash = jenkins_one_at_a_time_hash(stem, strlen(stem));

			if (words_all.find(hash) == words_all.end()) {
				WordInfo data;
				data.counter = 1;
				data.word = string(stem, strlen(stem));

				words_all[hash]= data;
				words->second[hash] = 1;
			} else {
				words_all[hash].counter++;
				if (words->second.find(hash) == words->second.end()) {
					words->second[hash] = 1;
				} else {
					words->second[hash]++;
				}
			}

        }
    }

    sb_stemmer_delete(stemmer_ru);
    sb_stemmer_delete(stemmer_en);

	for(WordList::const_iterator item = words_all.begin(); item != words_all.end(); item++) {
		cout << item->first << " " << item->second.word << " " << item->second.counter << endl;
	}

	for(ClassifierList::const_iterator classificator = classifier.begin(); classificator!= classifier.end(); classificator++) {
		cout << "-----CLASSIFICATOR " << classificator->first << " --------" << endl;
		for(WordsStat::const_iterator stats = classificator->second.begin(); stats != classificator->second.end(); stats++)
			cout << stats->first << "\t" << stats->second << endl;
	}

	BayesClassifier bayes;
	bayes.setStats(classifier);

	ofstream ofs(file.c_str());
	bayes.saveToStream(ofs);
	ofs.close();

	return 0;
}

int classifier(string file) {

	BayesClassifier bayes;
	ifstream ifs(file.c_str());
	bayes.loadFromStream(ifs);
	ifs.close();

    sb_stemmer* stemmer_ru = sb_stemmer_new("russian","UTF_8");
    sb_stemmer* stemmer_en = sb_stemmer_new("english","UTF_8");

    UErrorCode status = U_ZERO_ERROR;
    RegexMatcher matcher("([a-zа-я])+", 0, status);

    WordsStat words;
    ClassifierList classifier;

    while (!cin.eof()) {

		stringstream text;
		char sym;

		do {
			sym = cin.get();
			text << sym;
		} while (sym != 0 && cin.good());

		while (!text.eof()) {
			string buf;

			getline(text, buf);
			UnicodeString ucs = UnicodeString::fromUTF8(buf);

			ucs.toLower();

			Hash32 hash;

			matcher.reset(ucs);
			while (matcher.find()) {
				string str, str0;
				UnicodeString uWord = matcher.group(status);
				if (uWord.length() < 2) continue;
				uWord.toUTF8String(str);

				if (isStopWord(str)) continue;

				const char *s = str.c_str();
				const unsigned int n = str.length();
				const char *stem = (char * ) sb_stemmer_stem(stemmer_ru, sb_stemmer_stem(stemmer_en, (sb_symbol *) s,n), n);
				hash = jenkins_one_at_a_time_hash(stem, strlen(stem));

				if (words.find(hash) == words.end()) {
					words[hash] = 1;
				} else {
					words[hash]++;
				}
			}
		}

		ClassifierP p;

		ClassifierP answer = bayes.classify(words);
		//float k = (std::max_element(answer.begin(), answer.end()))->second;

		for(ClassifierP::const_iterator i = answer.begin(); i != answer.end(); i ++) {
			float sum = 0;
			for(ClassifierP::const_iterator j = answer.begin(); j != answer.end(); j++ ) {
				sum += exp( j->second - i->second );
			}
			p[i->first] = 1/ sum;
		}


		for(ClassifierP::const_iterator item = answer.begin(); item != answer.end(); item++) {
			cout << item->first << ' ' << item->second << ' ' << p[item->first] << endl;
		}
		cout << endl;
		words.clear();
    }

    sb_stemmer_delete(stemmer_ru);
    sb_stemmer_delete(stemmer_en);

	return 0;

}

int main(int argc, char* argv[]) {

    if (!setlocale(LC_ALL, "ru_RU.utf8"))
        if (!setlocale(LC_ALL, "russian"))
            return 1;

	if (argc != 3) {
		cout <<
"Классификатор текста на основе классификатора Байеса (ver 0.01) \n"
"bayes [режим_работы] [имя_файла_конфигурации] \n"
"Режимы работы: \n"
" - L - обучение (после обучения данные будут записаны в файл конфигурации) \n"
"\t На stdin подается набор (много штук) -----CLASSIFICATOR {uint_классификатора} --------\\n + \n"
"\t текст для классификации. Где uint_классификатора -- класс к которому должен принадлежать текст во время классификации \n"
"\t После закрытия stdin классификатор формирует файл конфигурации. \n"
" - C - классификация. На stdin подаем классифицируемый текст с \\0 в конце. На stdout получаем численные оценки принадлежности c \\0 в конце. \n"
"\n\n Логунцов С.В. 2013 г. mailto: loguntsov@gmail.com \n";
		return 0;
	}

	string file(argv[2]);

	switch(argv[1][0]) {
		case 'L' :
			return learn(file);
		case 'C' :
			return classifier(file);
		break;
	}

    return 0;
}
