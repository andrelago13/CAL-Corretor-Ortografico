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
		entry->getCount();
	}
};


class CorrectedWord{
public:
	static size_t MAX_SUGGESTIONS;
	int wordNum;
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
		os << "Text Corrections: " << std::endl;
		std::vector<CorrectedLine*>::iterator iti = cw->textCorrections.begin();
		std::vector<CorrectedLine*>::iterator ite = cw->textCorrections.end();
		int num = 0;
		for(; iti != ite ;iti++){
			std::cerr <<  cw->textCorrections.size() << ", "  << ++num<<std::endl;
			os  << *iti;
		}
		std::cerr <<"ok "<<std::endl;
		return os;
	}
};



class Corrector{
public:
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
			//std::cerr << "correcting text" << std::endl;
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
			//std::cerr << "done correcting text" << std::endl;
			return out;
	}
	static CorrectedLine* correctLine(Dictionary& dic, const std::string& line, int linenum){
		//std::cerr << "correcting Line" << std::endl;
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
		//std::cerr << "done correcting Line" << std::endl;
		return out;
	}
	static CorrectedWord* correctWord(Dictionary& dic, std::string& word, int wordnum){
		//std::cerr << "correcting word" << std::endl;
		CorrectedWord* out = new CorrectedWord(word, wordnum);
		std::set<DictionaryEntry*, DictionaryEntry::EntryComp>::iterator iti = dic.entries.begin();
		std::set<DictionaryEntry*, DictionaryEntry::EntryComp>::iterator ite = dic.entries.end();
		for(; iti != ite; ++iti){
			if(correctorValidation(word, (*iti)->getWord())){
				(*iti)->calculateEditDistance(word);
				out->addCorrection(*iti);
			}
		}
		//std::cerr << "done correcting word" << std::endl;
		return out;
	}
	//TODO Strategy
	static CorrectedText* correctBK(Dictionary& dic, std::string filename){
				std::cerr << "filling tree" << std::endl;
				BKTree tree = dic.fillBKTree();
				std::cerr << "done filling tree" << std::endl;
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
				//std::cerr << "done correcting text" << std::endl;
				return out;
		}
		static CorrectedLine* correctLineBK(Dictionary& dic, const std::string& line, int linenum, const BKTree& tree){
			//std::cerr << "correcting Line" << std::endl;
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
			//std::cerr << "done correcting Line" << std::endl;
			return out;
		}
		static CorrectedWord* correctWordBK(Dictionary& dic, std::string& word, int wordnum,  const BKTree& tree){
			//std::cerr << "correcting word" << std::endl;
			CorrectedWord* out = new CorrectedWord(word, wordnum);
			std::vector<DictionaryEntry*> found = tree.query(word, 2);
			for(size_t i = 0 ; i < found.size(); i++){
				out->addCorrection(found[i]);
			}
			//std::cerr << "done correcting word" << std::endl;
			return out;
		}

		static CorrectedText* correctTrie(Dictionary& dic, std::string filename){
			std::cerr << "filling tree" << std::endl;
			Trie tree = dic.fillTrie();
			std::cerr << "done filling tree" << std::endl;
			std::ifstream fin(filename.c_str());
			if(!fin.is_open())
				throw new CorrectorException( "could not open " + filename);
			std::string line;
			CorrectedText* out = new CorrectedText();
			int linenum = 0;
			while(!fin.eof()){
				linenum++;
				getline(fin, line);
				out->addCorrection(correctLineTrie(dic, line, linenum, tree));
				cout << out << endl;
			}
			fin.close();
			std::cerr << "done correcting text" << std::endl;
			return out;
		}
		static CorrectedLine* correctLineTrie(Dictionary& dic, const std::string& line, int linenum, const Trie& tree){
			//std::cerr << "correcting Line" << std::endl;
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
					out->addCorrection(correctWordTrie(dic, token,wordCount, tree));
				}
			}
			//std::cerr << "done correcting Line" << std::endl;
			return out;
		}
		static CorrectedWord* correctWordTrie(Dictionary& dic, std::string& word, int wordnum,  const Trie& tree){
			//std::cerr << "correcting word" << std::endl;
			CorrectedWord* out = new CorrectedWord(word, wordnum);
			std::vector<string> found = tree.query(word, 2);
			for(size_t i = 0 ; i < found.size(); i++){
				DictionaryEntry* temp = dic.findWord(found[i]);
					if(temp != NULL)
						out->addCorrection(temp);
			}
			//std::cerr << "done correcting word" << std::endl;
			return out;
		}
	static bool correctorValidation(const std::string& w1, const std::string& w2){
		return true;
		if(abs((int)w1.length() - (int)w2.length()) < 5){
			return true;
		}
		return false;
	}

};



#endif /* SRC_CORRECTOR_H_ */
