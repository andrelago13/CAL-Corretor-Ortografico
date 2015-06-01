/*
 * Corrector.h
 *
 *  Created on: May 17, 2015
 *      Author: Ricardo
 */

#ifndef SRC_CORRECTOR_H_
#define SRC_CORRECTOR_H_

#include <string>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <vector>
#include <iostream>
#include <cctype>
#include "Dictionary.h"
#include "DictionaryEntry.h"
#include "BKTree.h"
#include "Trie.h"
#include "UserInput.h"
#include <iostream>

class WordMatch{
	DictionaryEntry* entry;
	int distance;

public:
	struct MatchComp{
		bool operator()(WordMatch* e1, WordMatch* e2){
			if(e1->distance == e2->distance && e1->entry->getCount() == e2->entry->getCount())
				return e1->entry < e2->entry;
			if(e1->distance == e2->distance)
				return e1->entry->getCount() > e2->entry->getCount();
			else return e1->distance < e2->distance;
		}
	};
	WordMatch(DictionaryEntry* word): entry(word), distance(word->getLastEditDistance()){};
	std::string getWord()const{
		return entry->getWord();
	}
	int getDistance()const{
		return distance;
	}
	int getCount()const{
		return entry->getCount();
	}
};


class CorrectedWord{
public:
	static size_t MAX_SUGGESTIONS;
	unsigned wordNum;
	std::string word;
	std::set<WordMatch*, WordMatch::MatchComp> matches;
	CorrectedWord(std::string word,int wordNum ): wordNum(wordNum), word(word){};
	friend std::ostream& operator<<(std::ostream& os, CorrectedWord* cw){
		os << "Word Number " << cw->wordNum << ": " << cw->word << std::endl << "Suggestions:\n";
		std::set<WordMatch*, WordMatch::MatchComp>::iterator iti = cw->matches.begin();
		std::set<WordMatch*, WordMatch::MatchComp>::iterator ite = cw->matches.end();
		int numSug = 0;
		for(; iti != ite ; ++iti){
			numSug++;
			os <<  numSug << ": " << (*iti)->getWord() << ", Distance: " <<  (*iti)->getDistance()<< ", Count: " << (*iti)->getCount() << std::endl;
		}
		return os;
	}
	void addCorrection(DictionaryEntry * entry){
		WordMatch * temp = new WordMatch(entry);
		if(matches.size()  <  MAX_SUGGESTIONS)
			matches.insert(temp);
		else {
			std::set<WordMatch*, WordMatch::MatchComp>::iterator worst = --matches.end();
			if(WordMatch::MatchComp()(temp,*worst)){
				matches.erase(worst);
				matches.insert(temp);
			}else delete temp;
		}

	}
	~CorrectedWord(){
		std::set<WordMatch*, WordMatch::MatchComp>::iterator iti = matches.begin();
		std::set<WordMatch*, WordMatch::MatchComp>::iterator ite = matches.end();
		for(; iti != ite ; ++iti){
			delete *iti;
		}

	}
	string correct() const
	{
		if(matches.size() > 0)
		{
			cout << "Word Number " << wordNum << ": " << word << std::endl << "Suggestions:" << std::endl;
			cout << "0 - Add \"" << word << "\" to dictionary" << std::endl;
			std::set<WordMatch*, WordMatch::MatchComp>::iterator iti = matches.begin();
			std::set<WordMatch*, WordMatch::MatchComp>::iterator ite = matches.end();
			int numSug = 0;
			for(; iti != ite ; ++iti){
				cout <<  ++numSug << " - replace by \"" << (*iti)->getWord() << "\" (Distance: " << (*iti)->getDistance() << "; Count: " << (*iti)->getCount() << ")" << std::endl;
				//<< (*iti)->getWord() << ", Distance: " <<  (*iti)->getDistance()<< ", Count: " << (*iti)->getCount() << std::endl;
			}

			int option = UserInput::getInt(0, numSug);
			cout << "Option " << option << std::endl;

			if(option == 0)
				return word;

			iti = matches.begin();
			while(--option > 0)
				++iti;
			return (*(iti))->getWord();
		}

		cout << "Word Number " << wordNum << ": " << word << std::endl
				<< "No words were found similar to " << word << ". Would you like to replace it? (Y/N)" << std::endl;

		system("pause");
		return word;
	}
};
size_t CorrectedWord::MAX_SUGGESTIONS = 10;

class CorrectedLine{
public:
	int line;
	std::vector<CorrectedWord*> lineCorrections;
	~CorrectedLine(){
		for(size_t i  = 0; i < lineCorrections.size(); i++ ){
			delete lineCorrections[i];
		}
	}
	CorrectedLine(int line): line(line){};
	void addCorrection(CorrectedWord* correction){
		if(correction != NULL)
			lineCorrections.push_back(correction);
	}
	friend std::ostream& operator<<(std::ostream& os, CorrectedLine* cw){
		os << "Line Number " << cw->line << ": " << std::endl;
		std::vector<CorrectedWord*>::iterator iti = cw->lineCorrections.begin();
		std::vector<CorrectedWord*>::iterator ite = cw->lineCorrections.end();
		for(; iti != ite ;iti++){
			os  << *iti;
		}
		return os;
	}
	string correct(const string& original_line)
	{
		cout << "Line Number " << line << ": " << std::endl;
		string result = "";
		size_t word_no = 0;
		stringstream ss(original_line);
		string temp = "";

		for(size_t i = 0; i < lineCorrections.size(); i++)
		{
			while(++word_no < lineCorrections[i]->wordNum)
			{
				ss >> temp;
				result += temp + " ";
			}

			ss >> temp;
			temp = lineCorrections[i]->correct();
			result += temp + " ";
		}

		while(!ss.eof())
		{
			ss >> temp;
			result += temp + " ";
		}

		if(result[result.size()-1] == ' ')
			result.resize(result.size() - 1);
		return result;
	}
};

class CorrectedText{
public:
	std::vector<CorrectedLine*> textCorrections;
	void addCorrection(CorrectedLine* correction){
		if(correction != NULL)
			textCorrections.push_back(correction);
	}
	~CorrectedText(){
		for(size_t i  = 0; i < textCorrections.size(); i++ ){
			delete textCorrections[i];
		}
	}
	friend std::ostream& operator<<(std::ostream& os, CorrectedText* cw){
		os << "Text Corrections (" << cw->textCorrections.size() << "): " << std::endl;
		std::vector<CorrectedLine*>::iterator iti = cw->textCorrections.begin();
		std::vector<CorrectedLine*>::iterator ite = cw->textCorrections.end();
		int num = 0;
		for(; iti != ite ;iti++){
			std::cerr <<  cw->textCorrections.size() << ", "  << ++num<<std::endl;
			os  << *iti;
		}
		return os;
	}
};



class Corrector{
public:
	static const string CORRECTED_TEXT;
	//TODO make CorrectorException and DictionaryException inherit from base class
	class CorrectorException{
		std::string message;
	public:
		CorrectorException(std::string message){
			this->message = message;
		}
		const std::string& what() const{
			return message;
		}
	};

	static CorrectedText* correct(Dictionary& dic, std::string filename){
		std::ifstream fin(filename.c_str());
		if(!fin.is_open())
			throw new CorrectorException( "could not open " + filename);
		std::string line;
		CorrectedText* out = new CorrectedText();
		int linenum = 0;
		while(!fin.eof()){
			linenum++;
			getline(fin, line);
			out->addCorrection(correctLine(dic, line, linenum));
		}
		fin.close();
		return out;
	}

	static CorrectedLine* correctLine(Dictionary& dic, const std::string& line, int linenum){
		int wordCount = 0;
		std::istringstream iss(line);
		std::string token;
		CorrectedLine* out = NULL;
		int errorCount = 0;
		while(getline(iss, token, ' '))
		{
			unsigned int i;
			for(i = token.length(); i >= 0; i-- ){
				if(isalpha(token[i]))
					break;
			}
			token = token.substr(0,i+1);
			if(token.length() == 0 )
				continue;
			wordCount++;
			if(dic.findWord(token) == NULL){
				errorCount++;
				if(out == NULL)
					out = new CorrectedLine(linenum);
				out->addCorrection(correctWord(dic, token,wordCount));
			}
		}
		return out;
	}

	static CorrectedWord* correctWord(Dictionary& dic, std::string& word, int wordnum){
		CorrectedWord* out = new CorrectedWord(word, wordnum);
		hash_table::iterator iti = dic.entries.begin();
		hash_table::iterator ite = dic.entries.end();
		for(; iti != ite; ++iti){
			if(correctorValidation(word, (*iti)->getWord())){\
				(*iti)->calculateEditDistance(word);
				out->addCorrection(*iti);
			}
		}
		return out;
	}

	static BKTree fillBK(Dictionary *dic, std::string filename){
		return dic->fillBKTree();
	}
	static CorrectedText* correctBK(const BKTree &tree, Dictionary& dic, std::string filename){
		std::ifstream fin(filename.c_str());
		if(!fin.is_open())
			throw new CorrectorException( "could not open " + filename);
		std::string line;
		CorrectedText* out = new CorrectedText();
		int linenum = 0;
		while(!fin.eof()){
			linenum++;
			getline(fin, line);
			out->addCorrection(correctLineBK(dic, line, linenum, tree));
		}
		fin.close();
		return out;
	}
	static CorrectedLine* correctLineBK(Dictionary& dic, const std::string& line, int linenum, const BKTree& tree){
		int wordCount = 0;
		std::istringstream iss(line);
		std::string token;
		CorrectedLine* out = NULL;
		int errorCount = 0;
		while(getline(iss, token, ' '))
		{
			unsigned int i;
			for(i = token.length(); i >= 0; i-- ){
				if(isalpha(token[i]))
					break;
			}
			token = token.substr(0,i+1);
			if(token.length() == 0 )
				continue;
			wordCount++;
			if(dic.findWord(token) == NULL){
				errorCount++;
				if(out == NULL)
					out = new CorrectedLine(linenum);
				out->addCorrection(correctWordBK(dic, token,wordCount, tree));
			}
		}
		return out;
	}
	static CorrectedWord* correctWordBK(Dictionary& dic, std::string& word, int wordnum,  const BKTree& tree){
		CorrectedWord* out = new CorrectedWord(word, wordnum);
		std::vector<DictionaryEntry*> found = tree.query(word, 2);
		for(size_t i = 0 ; i < found.size(); i++){
			out->addCorrection(found[i]);
		}
		return out;
	}

	static Trie fillTrie(Dictionary *dic)
	{
		return dic->fillTrie();
	}

	static CorrectedText* correctTrie(const Trie &tree, Dictionary& dic, std::string filename){
		std::ifstream fin(filename.c_str());
		if(!fin.is_open())
		{
			throw new CorrectorException( "could not open " + filename);
		}
		std::string line;
		CorrectedText* out = new CorrectedText();
		int linenum = 0;
		while(!fin.eof()){
			linenum++;
			getline(fin, line);
			out->addCorrection(correctLineTrie(dic, line, linenum, tree));
		}
		fin.close();
		return out;
	}
	static CorrectedLine* correctLineTrie(const Dictionary& dic, const std::string& line, int linenum, const Trie& tree){
		int wordCount = 0;
		std::istringstream iss(line);
		std::string token;
		CorrectedLine* out = NULL;
		int errorCount = 0;
		while(getline(iss, token, ' '))
		{
			// TODO change way string is "run" to include commas
			unsigned int i;
			for(i = token.length(); i >= 0; i-- ){
				if(isalpha(token[i]))
					break;
			}
			token = token.substr(0,i+1);
			if(token.length() == 0 )
				continue;
			wordCount++;
			if(dic.findWord(token) == NULL){
				errorCount++;
				if(out == NULL)
					out = new CorrectedLine(linenum);
				out->addCorrection(correctWordTrie(dic, token,wordCount, tree));
			}
		}
		return out;
	}
	static CorrectedWord* correctWordTrie(const Dictionary& dic, std::string& word, int wordnum,  const Trie& tree){
		CorrectedWord* out = new CorrectedWord(word, wordnum);
		std::vector<DictionaryEntry*> found = tree.query(word, 2);
		for(size_t i = 0 ; i < found.size(); i++){
			DictionaryEntry* temp = found[i];
			if(temp != NULL)
				out->addCorrection(temp);
		}
		return out;
	}

	static bool correctorValidation(const std::string& w1, const std::string& w2){
		return true;
		if(abs((int)w1.length() - (int)w2.length()) < 5){
			return true;
		}
		return false;
	}

	static void correctTextDynamic(const std::string& newFile, const std::string &oldFile, Dictionary &dic)
	{
		Trie tree = dic.fillTrie();
		std::ifstream fin(oldFile.c_str());
		if(!fin.is_open())
			throw new CorrectorException( "could not open " + oldFile);
		std::ofstream fout(newFile.c_str());
		if(!fout.is_open())
			throw new CorrectorException( "could not open " + newFile);

		std::string line;
		int linenum = 0;

		while(!fin.eof()){
			linenum++;
			getline(fin, line);
			fout << correctLineDynamic(dic, line, linenum, tree, oldFile) << std::endl;
		}

		fin.close();
		fout.close();
	}

	static std::string correctLineDynamic(Dictionary& dic, const std::string& line, int linenum, Trie& tree, const std::string oldFile) {
		int wordCount = 0;
		std::istringstream iss(line);
		std::string token;
		int errorCount = 0;

		std::string result = "";

		while(getline(iss, token, ' '))
		{
			unsigned int i;
			for(i = token.length(); i >= 0; i-- ){
				if(isalpha(token[i]))
					break;
			}
			token = token.substr(0,i+1);
			if(token.length() == 0 )
				continue;
			wordCount++;
			if(dic.findWord(token) == NULL){
				errorCount++;
				result += correctWordDynamic(dic, token, linenum, wordCount, tree, oldFile) + " ";
			}
			else
				result += token + " ";
		}

		return result;
	}

	static std::string correctWordDynamic(Dictionary& dic, std::string& word, int linenum, int wordnum,  Trie& tree, const std::string oldFile) {

		string newWord = word;
		CorrectedWord* out = new CorrectedWord(word, wordnum);
		int maxDist = 2;
		std::vector<DictionaryEntry*> found = tree.query(word, maxDist);

		std::cout << "Line " << linenum << ", Word " << wordnum << ": " << word << std::endl << "Suggestions:" << std::endl;
		cout << "0 - Add \"" << word << "\" to dictionary" << std::endl;

		while(++maxDist <= 6 && found.size() <= 0)
		{
			std::vector<DictionaryEntry*> temp = tree.query(word, maxDist);
			found.insert(found.end(), temp.begin(), temp.end());
		}

		for(size_t i = 0 ; i < found.size(); i++){
			DictionaryEntry* temp = dic.findWord(found[i]->getWord());
			if(temp != NULL)
			{
				out->addCorrection(temp);
			}
		}

		if(!found.empty())
		{
			std::set<WordMatch*, WordMatch::MatchComp>::iterator iti = out->matches.begin();
			std::set<WordMatch*, WordMatch::MatchComp>::iterator ite = out->matches.end();

			int numSug = 0;
			for(; iti != ite ; ++iti){
				cout <<  ++numSug << " - replace by \"" << (*iti)->getWord() << "\" (Distance: " << (*iti)->getDistance() << "; Count: " << (*iti)->getCount() << ")" << std::endl;
			}

			int option = UserInput::getInt(0, numSug);
			cout << "Option " << option << std::endl;

			if(option == 0)
			{
				dic.addEntry(oldFile, word);
				return word;
			}

			iti = out->matches.begin();
			while(--option > 0)
				++iti;
			return (*(iti))->getWord();
		}
		else
		{
			cout << "No words were found similar to " << word << ". Would you like to add it to the dictionary?" << std::endl;

			if(UserInput::getYesNo()) {
				dic.addEntry(oldFile, word);
				return newWord;
			}

			return newWord;
		}
	}
};

const string Corrector::CORRECTED_TEXT = "corrected_text.txt";


#endif /* SRC_CORRECTOR_H_ */
