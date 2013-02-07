#ifndef BAYESCLASSIFIER_H
#define BAYESCLASSIFIER_H

#include <stdint.h>
#include <map>
#include <string>
#include <ostream>
#include <istream>

typedef uint32_t Hash32;
typedef unsigned int CategoryId;
typedef std::map <Hash32, unsigned int> WordsStat;
typedef std::map <CategoryId, WordsStat> ClassifierList;
typedef std::map <CategoryId, float> ClassifierP;
typedef std::map <Hash32, float> WordP;
typedef std::map <CategoryId, WordP > ClassifierListP;
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
