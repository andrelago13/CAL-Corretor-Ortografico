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

class Dictionary{
	friend class Corrector;

private:
	std::set<DictionaryEntry*, DictionaryEntry::EntryComp> entries;

	void readProcessedDictionary(std::string filename) {
		std::ifstream fin(filename.c_str());
		if(!fin.is_open())
			throw new DictionaryException("Invalid file name");
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
			for(int i = 0; i < word.size(); i++){
				if(word[i] != '-' && word[i] != '\'' && !isalpha(word[i])){
					found = true;
					break;
				}
			}
			if(found)
				continue;
			DictionaryEntry* entry = new DictionaryEntry(word, num);
			entry->computePrefixFunction();
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
			entry->computePrefixFunction();
			entries.insert(entry);
		}
		fin.close();
	}
public:
	bool writeDictionary(std::string filename){
			std::ofstream fout(filename.c_str());
			if(!fout.is_open())
				throw new DictionaryException("Invalid file name");
			std::set<DictionaryEntry*>::iterator iti = entries.begin();
			std::set<DictionaryEntry*>::iterator ite = entries.end();
			for(; iti != ite; ++iti){
				fout << (*iti)->getWord() << " " << (*iti)->getCount() << std::endl;
			}
			fout.close();
		}
	void countWords(std::string& filename) {
		std::ifstream fin(filename.c_str());
		if(!fin.is_open())
			throw new DictionaryException("Invalid file name");
		std::set<DictionaryEntry*>::iterator iti = entries.begin();
		std::set<DictionaryEntry*>::iterator ite = entries.end();
		for(; iti != ite; ++iti){
			(*iti)->resetPiIndex();
		}
		while(!fin.eof()){
			std::string line;
			getline(fin, line);
			for(size_t i = 0; i < line.length(); i++){
				iti = entries.begin();
				for(; iti != ite; ++iti){
					(*iti)->processChar(line[i]);
					//std::cerr << "processed char" << std::endl;
				}
			}
		}
		fin.close();
	}
	void countWholeWords(std::string& filename) {
			std::ifstream fin(filename.c_str());
			if(!fin.is_open())
				throw new DictionaryException("Invalid file name");
			std::string word;
			while(!fin.eof()){
				fin >> word;
				DictionaryEntry* entry = new DictionaryEntry(word);
				std::set<DictionaryEntry*>::iterator iti;
				if((iti = entries.find(entry)) != entries.end()){
					(*iti)->incCount();
					std::cerr << "found " << word<< std::endl;
				}
				delete(entry);
			}
			fin.close();
		}
	DictionaryEntry* findWord(std::string& word) {
		std::set<DictionaryEntry*, DictionaryEntry::EntryComp>::iterator iti;
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
		std::set<DictionaryEntry*>::const_iterator iti = dic.entries.begin();
		std::set<DictionaryEntry*>::const_iterator ite = dic.entries.end();
		for(; iti != ite; ++iti){
			os << **iti << "\n";
		}
		os << "\n";
		return os;
	}
	void debug(){
		std::set<DictionaryEntry*, DictionaryEntry::EntryComp>::iterator iti = entries.begin();
		std::set<DictionaryEntry*, DictionaryEntry::EntryComp>::iterator ite = entries.end();
		int count = 0;
		for(; iti != ite; ++iti){
			count++;
		}
		std::cerr << count << ", " << entries.size() << std::endl;
	}
	BKTree fillBKTree(){
		BKTree out;
		std::set<DictionaryEntry*, DictionaryEntry::EntryComp>::iterator iti = entries.begin();
		std::set<DictionaryEntry*, DictionaryEntry::EntryComp>::iterator ite = entries.end();
		int count = 0;
		for(; iti != ite; ++iti){
			out.insertEntry(*iti);
		}
		return out;
	}
	Trie fillTrie(){
		Trie out;
		std::set<DictionaryEntry*, DictionaryEntry::EntryComp>::iterator iti = entries.begin();
		std::set<DictionaryEntry*, DictionaryEntry::EntryComp>::iterator ite = entries.end();
		int count = 0;
		for(; iti != ite; ++iti){
			count++;
			out.insertEntry((*iti)->getWord());
		}
		cerr << count << endl;
		return out;
	}
};




#endif /* DICTIONARY_H_ */
