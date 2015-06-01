/*
 * DictionaryEntry.h
 *
 *  Created on: May 17, 2015
 *      Author: Ricardo
 */

#ifndef SRC_DICTIONARYENTRY_H_
#define SRC_DICTIONARYENTRY_H_

#include <string>
#include <vector>
#include <algorithm>
#include <iostream>
#include <iomanip>

#include "matcher.h"

#define WORD_SPACE 15

class DictionaryEntry{
private:
	std::string word;
	int count;
	int lastEditDistance;
public:
	DictionaryEntry(std::string word,float count = 0);
	int incCount();
	int getCount() const;
	void setCount(int count);
	int getLastEditDistance() const;
	void setLastEditDistance(int newDist);
	const std::string& getWord() const;
	int calculateEditDistance(const std::string& second_word);
	bool operator<(const DictionaryEntry& entry2) const;
	bool operator==(const DictionaryEntry &entry) const;
	friend std::ostream& operator <<(std::ostream& os, DictionaryEntry dic);
	struct EntryComp{
		bool operator()( DictionaryEntry * e1,  DictionaryEntry* e2) const {
			return *e1 < *e2;
		}
	};
};



#endif /* SRC_DICTIONARYENTRY_H_ */
