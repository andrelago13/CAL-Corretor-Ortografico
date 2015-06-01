/*
 * Dictionary.h
 *
 *  Created on: May 17, 2015
 *      Author: Ricardo
 */

#ifndef DICTIONARY_H_
#define DICTIONARY_H_

#include <set>
#include <string>
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>
#include "DictionaryEntry.h"
#include "BKTree.h"
#include "Trie.h"
#include <unordered_set>

struct hstr {
	int operator() (const DictionaryEntry *entry) const {
		int v = 0;
		const string s1 = entry->getWord();
		for (unsigned int i = 0; i < s1.length(); i++)
			v = 37 * v + s1[i];
		return v;
	}
};

struct eqstr {
	bool operator() ( const DictionaryEntry *entry1, const DictionaryEntry *entry2) const {
		return *entry1 == *entry2;
	}
};

typedef std::unordered_set<DictionaryEntry *, hstr, eqstr> hash_table;

class Dictionary{
	friend class Corrector;
private:
	hash_table entries;
	void readProcessedDictionary(std::string filename);
	void readUnprocessedDictionary(std::string filename);
public:
	void writeDictionary(std::string filename);
	void countWholeWords(std::string& filename);
	void addEntry(const std::string& filename, const std::string& word);
	int wordCount(const std::string& filename, const std::string& word);
	DictionaryEntry* findWord(const std::string& word);
	Dictionary(std::string filename, bool isProcessed);

	class DictionaryException{
		std::string message;
	public:
		DictionaryException(std::string message){
			this->message = message;
		}
		const std::string& what() const{
			return message;
		}
	};

	friend std::ostream& operator <<(std::ostream& os, const Dictionary& dic);
	void debug();
	BKTree fillBKTree();
	Trie fillTrie();
	~Dictionary();
};

#endif /* DICTIONARY_H_ */
