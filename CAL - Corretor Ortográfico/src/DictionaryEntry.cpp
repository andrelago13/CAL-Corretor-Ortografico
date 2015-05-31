/*
 * DictionaryEntry.cpp
 *
 *  Created on: May 17, 2015
 *      Author: Ricardo
 */

#include "DictionaryEntry.h"

DictionaryEntry::DictionaryEntry(std::string word,float count) {
	this->word = word;
	this->count = count;
	lastEditDistance = -1;
}
int DictionaryEntry::incCount(){
	return count++;
}

int DictionaryEntry::getCount() const {
	return count;
}

void DictionaryEntry::setCount(int count) {
	this->count = count;
}

int DictionaryEntry::getLastEditDistance() const {
	return lastEditDistance;
}
void DictionaryEntry::setLastEditDistance(int newDist) {
	lastEditDistance = newDist;
}

const std::string& DictionaryEntry::getWord() const {
	return word;
}
int DictionaryEntry::calculateEditDistance(const std::string& second_word){
	lastEditDistance = editDistance(word,second_word);
	return lastEditDistance;
}
bool DictionaryEntry::operator<(const DictionaryEntry& entry2) const{
	return word < entry2.word;
}
bool DictionaryEntry::operator==(const DictionaryEntry &entry) const
			{
	return word == entry.word;
			}
std::ostream& operator <<(std::ostream& os, DictionaryEntry dic){
	os << std::setw(WORD_SPACE) << dic.word << " " << "count: " << dic.count;
	return os;
}
