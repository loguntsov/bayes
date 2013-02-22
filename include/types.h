#ifndef TYPES_H
#define TYPES_H

#include <map>
#include <stdint.h>

typedef uint32_t Hash32;
typedef unsigned int CategoryId;
typedef std::map <Hash32, unsigned int> WordsStat;
struct ClassifierStat {
    WordsStat words;
    unsigned int docs_count;
};
typedef std::map <CategoryId, ClassifierStat> ClassifierList;
typedef std::map <CategoryId, float> ClassifierP;
typedef std::map <Hash32, float> WordP;
typedef std::map <CategoryId, WordP > ClassifierListP;


#endif // TYPES_H
