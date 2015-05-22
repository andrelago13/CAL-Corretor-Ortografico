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
	std::vector<int> pi;
	int currentPiIndex = 0;
	int lastEditDistance;

public:
	DictionaryEntry(std::string word,std::vector<int> pi, int count = 0){
		this->word = word;
		this->count = count;
		this->pi = pi;
		lastEditDistance = -1;

	}
	DictionaryEntry(std::string word,float count = 0): DictionaryEntry(word, std::vector<int>(), count) {

	}
	int incCount(){
		count++;
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

	const std::vector<int>& getPi() const {
		return pi;
	}

	const std::string& getWord() const {
		return word;
	}
	int calculateEditDistance(const std::string& second_word){
		lastEditDistance = editDistance(word,second_word);
		return lastEditDistance;
	}
	void computePrefixFunction(){
		if(pi.size() == word.length())
			return;
		pi = computePrefix(word);
	}
	bool operator<(const DictionaryEntry& entry2) const{
		return word < entry2.word;
	}
	friend std::ostream& operator <<(std::ostream& os, DictionaryEntry dic){
		os << std::setw(WORD_SPACE) << dic.word << " " << "count: " << dic.count;
		os << " Prefixes: [ ";
		for(size_t i = 0; i < dic.pi.size(); i++){
			os << dic.pi[i] << " ";
		}
		os << "]";
		return os;
	}
	void resetPiIndex(){
		currentPiIndex = 0;
	}
	void processChar(const char& c){
		while(currentPiIndex > 0 && word[currentPiIndex] != c){
			//std::cerr << "loopin"  << currentPiIndex << ", " << word[currentPiIndex] << ", " << c << std::endl;
			currentPiIndex = pi[currentPiIndex];
		}
		if(word[currentPiIndex] == c)
			currentPiIndex++;
		if(currentPiIndex == word.size()){
			count++;
			currentPiIndex =  pi[currentPiIndex];
			std::cerr << "found " << word << ", count" << count << std::endl;
		}
	}
	struct EntryComp{
		bool operator()( DictionaryEntry * e1,  DictionaryEntry* e2){
			return *e1 < *e2;
		}
	};

};



#endif /* SRC_DICTIONARYENTRY_H_ */
