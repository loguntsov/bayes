#include <math.h>
#include "BayesClassifier.h"
#include <fstream>
#include <iostream>

using namespace std;

BayesClassifier::BayesClassifier() {
	//ctor
}

BayesClassifier::~BayesClassifier() {
	//dtor
}

void BayesClassifier::setStats(ClassifierList list) {
	WordsStat all;
	for(ClassifierList::const_iterator item = list.begin(); item != list.end(); item++) {
		WordsStat::iterator it_all;
		for(WordsStat::const_iterator el = item->second.begin(); el != item->second.end(); el++) {
			it_all = all.find(el->first);
			if (it_all == all.end()) {
				all[el->first]=1;
			} else {
				it_all->second += el->second;
			}
		}
	}
	this->setStats(list, all);
}

void BayesClassifier::setStats(ClassifierList list, WordsStat all) {
	unsigned int total = 0;
	for(WordsStat::const_iterator item = all.begin(); item != all.end(); item++) {
		total+=item->second;
	}

	ClassifierListP categoryP;

	for(ClassifierList::const_iterator category = list.begin(); category != list.end(); category++) {
		for(WordsStat::const_iterator p = category->second.begin(); p != category->second.end(); p++) {
			categoryP[category->first][p->first] = log((float) p->second / all[p->first]);
		}
	}
	WordP allP;

	for(WordsStat::const_iterator item = all.begin(); item != all.end(); item++) {
		allP[item->first] = log((float) item->second / total);
	}

	this->categoryP = categoryP;
	this->allP = allP;
}

ClassifierP BayesClassifier::classify(WordsStat stat) {
	ClassifierP answer;
	for(ClassifierListP::const_iterator category = this->categoryP.begin(); category != this->categoryP.end(); category++) {
		float estimation = 0;
		for(WordsStat::const_iterator word = stat.begin(); word != stat.end(); word++) {
			if (word->second == 0) continue;
			ClassifierP::const_iterator it = category->second.find(word->first);
			if (it != category->second.end()) {
				estimation += it->second - this->allP[word->first];
			}
		}
		answer[category->first] = estimation;
	}
	return answer;
}

void BayesClassifier::saveToStream(ostream &stream) {
	stream << this->allP.size() << endl;
	for(WordP::const_iterator wordIt = this->allP.begin(); wordIt != this->allP.end(); wordIt++) {
		stream << wordIt->first << " " << wordIt->second << endl;
	}
	stream << "XXXXX" << endl;

	stream << this->categoryP.size() << endl;

	for(ClassifierListP::const_iterator categoryIt = this->categoryP.begin(); categoryIt != this->categoryP.end(); categoryIt++) {
		stream << categoryIt->first << " " << categoryIt->second.size() << endl;
		for(WordP::const_iterator wordIt = categoryIt->second.begin(); wordIt != categoryIt->second.end(); wordIt++) {
			stream << wordIt->first << " " << wordIt->second << endl;
		}
		stream << "XX" << categoryIt->first << "XX" << endl;
	}

}

void BayesClassifier::loadFromStream(istream &stream) {
	WordP::size_type allPSize = 0;
	stream >> allPSize;
	this->allP.clear();
	string str;
	for(unsigned int word = 0; word < allPSize; word++) {
		Hash32 hash = 0;
		float value;
		stream >> hash >> value;
		this->allP[hash] = value;
	}
	stream >> str;
	if (str.compare("XXXXX") != 0) {
		throw "Ожидалось XXXXXX";
	}

	ClassifierListP::size_type categoryPsize = 0;
	stream >> categoryPsize;

	for(unsigned int cnt_category = 0; cnt_category < categoryPsize; cnt_category++) {
		CategoryId id;
		WordP::size_type count;
		stream >> id >> count;
		WordP cat;
		for(unsigned int i = 0; i < count; i++) {
			Hash32 hash;
			float value;
			stream >> hash >> value;
			cat[hash] = value;
		}
		this->categoryP[id] = cat;
		stream >> str;
		if (str[0] != 'X') {
			throw "Ожидалось XX{id}XX";
		}
	}
}

