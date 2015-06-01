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
	friend std::ostream& operator<<(std::ostream& os, CorrectedWord* cw);
	void addCorrection(DictionaryEntry * entry);
	~CorrectedWord();
	string correct() const;
};

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

	static CorrectedText* correct(Dictionary& dic, std::string filename);
	static CorrectedLine* correctLine(Dictionary& dic, const std::string& line, int linenum);
	static CorrectedWord* correctWord(Dictionary& dic, std::string& word, int wordnum);
	static BKTree fillBK(Dictionary *dic, std::string filename);
	static CorrectedText* correctBK(const BKTree &tree, Dictionary& dic, std::string filename);
	static CorrectedLine* correctLineBK(Dictionary& dic, const std::string& line, int linenum, const BKTree& tree);
	static CorrectedWord* correctWordBK(Dictionary& dic, std::string& word, int wordnum,  const BKTree& tree);
	static Trie fillTrie(Dictionary *dic);
	static CorrectedText* correctTrie(const Trie &tree, Dictionary& dic, std::string filename);
	static CorrectedLine* correctLineTrie(const Dictionary& dic, const std::string& line, int linenum, const Trie& tree);
	static CorrectedWord* correctWordTrie(const Dictionary& dic, std::string& word, int wordnum,  const Trie& tree);
	static bool correctorValidation(const std::string& w1, const std::string& w2);
	static void correctTextDynamic(const std::string& newFile, const std::string &oldFile, Dictionary &dic);
	static std::string correctLineDynamic(Dictionary& dic, const std::string& line, int linenum, Trie& tree, const std::string oldFile);
	static std::string correctWordDynamic(Dictionary& dic, std::string& word, int linenum, int wordnum,  Trie& tree, const std::string oldFile);
};


#endif /* SRC_CORRECTOR_H_ */
