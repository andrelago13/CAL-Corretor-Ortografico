/*
 * Dictionary.cpp
 *
 *  Created on: May 17, 2015
 *      Author: Ricardo
 */

#include "Dictionary.h"

void Dictionary::readProcessedDictionary(std::string filename) {
	std::ifstream fin(filename.c_str());
	if(!fin.is_open())
		throw DictionaryException("Invalid file name");
	int num;
	string line, word;
	while(getline(fin, line)){
		//getline(fin, word);
		std::stringstream ss(line);
		ss >> word;
		ss >> num;
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

void Dictionary::readUnprocessedDictionary(std::string filename) {
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

void Dictionary::writeDictionary(std::string filename){
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
void Dictionary::countWholeWords(std::string& filename) {
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

void Dictionary::addEntry(const std::string& filename, const std::string& word) {
	DictionaryEntry* entry = new DictionaryEntry(word);
	entry->setCount(wordCount(filename, word));
	entries.insert(entry);
}

int Dictionary::wordCount(const std::string& filename, const std::string& word) {
	int count = 0;
	std::ifstream fin(filename.c_str());
	if(!fin.is_open()) {
		std::cerr << "Error opening original text file" << std::endl;
		throw new DictionaryException("Invalid file name");
	}
	std::string textWord = "";
	while(!fin.eof()){
		fin >> textWord;
		if(textWord == word)
			++count;
	}
	fin.close();
	return count;
}

DictionaryEntry* Dictionary::findWord(const std::string& word) const{
	hash_table::const_iterator iti;
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
Dictionary::Dictionary(std::string filename, bool isProcessed){
	if(isProcessed)
		readProcessedDictionary(filename);
	else readUnprocessedDictionary(filename);
}

std::ostream& operator <<(std::ostream& os, const Dictionary& dic){
	os << "number of entries in dictionary: " << dic.entries.size() <<"\n";
	hash_table::const_iterator iti = dic.entries.begin();
	hash_table::const_iterator ite = dic.entries.end();
	for(; iti != ite; ++iti){
		os << **iti << "\n";
	}
	os << "\n";
	return os;
}
void Dictionary::debug(){
	hash_table::iterator iti = entries.begin();
	hash_table::iterator ite = entries.end();
	int count = 0;
	for(; iti != ite; ++iti){
		count++;
	}
	std::cerr << count << ", " << entries.size() << std::endl;
}
BKTree Dictionary::fillBKTree(){
	BKTree out;
	hash_table::iterator iti = entries.begin();
	hash_table::iterator ite = entries.end();
	for(; iti != ite; ++iti){
		out.insertEntry(*iti);
	}
	return out;
}
Trie Dictionary::fillTrie(){
	Trie out;
	hash_table::iterator iti = entries.begin();
	hash_table::iterator ite = entries.end();
	int count = 0;
	for(; iti != ite; ++iti){
		count++;
		out.insertEntry(*iti, (*iti)->getWord());
	}
	return out;
}

Dictionary::~Dictionary()
{
	for (hash_table::const_iterator it = entries.begin(); it != entries.end(); ++it)
	{
		delete *it;
	}
}
