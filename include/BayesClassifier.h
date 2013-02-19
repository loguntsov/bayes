#ifndef BAYESCLASSIFIER_H
#define BAYESCLASSIFIER_H

#include <string>
#include <ostream>
#include <istream>

#include "types.h"

/*
	Классификатор Байеса
*/
class BayesClassifier {
	public:
		BayesClassifier();
		/*
			Инициализировать классификатор частотным списком
		*/
		void setStats(ClassifierList list);
		void setStats(ClassifierList list, WordsStat all);
		// Выполнить классификацию
		ClassifierP classify(WordsStat stat);
		void saveToStream(std::ostream &stream);
		void loadFromStream(std::istream &stream);
		virtual ~BayesClassifier();
	protected:
		WordP allP;
		ClassifierListP categoryP;
		ClassifierP pNoWordFound;
	private:
};

#endif // BAYESCLASSIFIER_H
