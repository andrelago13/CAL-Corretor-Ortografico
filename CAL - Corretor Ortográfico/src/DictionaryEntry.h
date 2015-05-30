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
	DictionaryEntry(std::string word,float count = 0) {
		this->word = word;
		this->count = count;
		lastEditDistance = -1;
	}
	int incCount(){
		return count++;
	}

	int getCount() const {
		return count;
	}

	void setCount(int count) {
		this->count = count;
	}

	int getLastEditDistance() const {
		return lastEditDistance;
	}
	void setLastEditDistance(int newDist) {
		lastEditDistance = newDist;
	}

	const std::string& getWord() const {
		return word;
	}
	int calculateEditDistance(const std::string& second_word){
		lastEditDistance = editDistance(word,second_word);
		return lastEditDistance;
	}
	bool operator<(const DictionaryEntry& entry2) const{
		return word < entry2.word;
	}
	bool operator==(const DictionaryEntry &entry) const
	{
		return word == entry.word;
	}
	friend std::ostream& operator <<(std::ostream& os, DictionaryEntry dic){
		os << std::setw(WORD_SPACE) << dic.word << " " << "count: " << dic.count;
		return os;
	}
	struct EntryComp{
		bool operator()( DictionaryEntry * e1,  DictionaryEntry* e2){
			return *e1 < *e2;
		}
	};

};



#endif /* SRC_DICTIONARYENTRY_H_ */
