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

	void readProcessedDictionary(std::string filename) {
		std::ifstream fin(filename.c_str());
		if(!fin.is_open())
			throw DictionaryException("Invalid file name");
		std::string word;
		int num;
		//std::stringstream ss;
		while(!fin.eof()){
			//getline(fin, word);
			//std::stringstream ss(word);
			fin >> word;
			fin >> num;
		//	std::cerr << "read " << word << std::endl;
			//if(word.size() == 1 && !(word == "a" || word =="A"))
				//continue;
			bool found = false;
			for(size_t i = 0; i < word.size(); i++){
				if(word[i] != '-' && word[i] != '\'' && !isalpha(word[i])){
					found = true;
					break;
				}
			}
			if(found)
				continue;
			DictionaryEntry* entry = new DictionaryEntry(word, num);
			entries.insert(entry);
		}
		fin.close();
	}

	void readUnprocessedDictionary(std::string filename) {
		std::ifstream fin(filename.c_str());
		if(!fin.is_open())
			throw new DictionaryException("Invalid file name");
		while(!fin.eof()){
			std::string word;
			fin >> word;
			if(word.size() == 1 && !(word == "a" || word =="A"))
				continue;
			DictionaryEntry* entry = new DictionaryEntry(word);
			entries.insert(entry);
		}
		fin.close();
	}
public:
	void writeDictionary(std::string filename){
			std::ofstream fout(filename.c_str());
			if(!fout.is_open())
				throw new DictionaryException("Invalid file name");
			hash_table::iterator iti = entries.begin();
			hash_table::iterator ite = entries.end();
			for(; iti != ite; ++iti){
				fout << (*iti)->getWord() << " " << (*iti)->getCount() << std::endl;
			}
			fout.close();
		}
	void countWholeWords(std::string& filename) {
			std::ifstream fin(filename.c_str());
			if(!fin.is_open())
				throw new DictionaryException("Invalid file name");
			std::string word;
			while(!fin.eof()){
				fin >> word;
				DictionaryEntry* entry = new DictionaryEntry(word);
				hash_table::iterator iti;
				if((iti = entries.find(entry)) != entries.end()){
					(*iti)->incCount();
					std::cerr << "found " << word<< std::endl;
				}
				delete(entry);
			}
			fin.close();
		}
	DictionaryEntry* findWord(std::string& word) {
		hash_table::iterator iti;
		DictionaryEntry* entry = new DictionaryEntry(word);
		iti = entries.find(entry);
		delete entry;
		if(iti != entries.end()){
			return *iti;
		}
		std::cerr << word << std::endl;
		if(editDistance(word, "war") == 0){
			std::cerr << "oops" << std::endl;
		}
		return NULL;
	}
	Dictionary(std::string filename, bool isProcessed){
		if(isProcessed)
			readProcessedDictionary(filename);
		else readUnprocessedDictionary(filename);
	}
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
	friend std::ostream& operator <<(std::ostream& os, const Dictionary& dic){
		os << "number of entries in dictionary: " << dic.entries.size() <<"\n";
		hash_table::const_iterator iti = dic.entries.begin();
		hash_table::const_iterator ite = dic.entries.end();
		for(; iti != ite; ++iti){
			os << **iti << "\n";
		}
		os << "\n";
		return os;
	}
	void debug(){
		hash_table::iterator iti = entries.begin();
		hash_table::iterator ite = entries.end();
		int count = 0;
		for(; iti != ite; ++iti){
			count++;
		}
		std::cerr << count << ", " << entries.size() << std::endl;
	}
	BKTree fillBKTree(){
		BKTree out;
		hash_table::iterator iti = entries.begin();
		hash_table::iterator ite = entries.end();
		for(; iti != ite; ++iti){
			out.insertEntry(*iti);
		}
		return out;
	}
	Trie fillTrie(){
		Trie out;
		hash_table::iterator iti = entries.begin();
		hash_table::iterator ite = entries.end();
		int count = 0;
		for(; iti != ite; ++iti){
			count++;
			out.insertEntry(*iti, (*iti)->getWord());
		}
		cerr << count << endl;
		return out;
	}

	~Dictionary()
	{
		for (hash_table::const_iterator it = entries.begin(); it != entries.end(); ++it)
		{
			delete *it;
		}
	}
};




#endif /* DICTIONARY_H_ */
