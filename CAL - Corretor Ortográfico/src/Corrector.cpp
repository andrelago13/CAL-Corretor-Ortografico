/*
 * Corrector.cpp
 *
 *  Created on: May 17, 2015
 *      Author: Ricardo
 */

#ifndef SRC_CORRECTOR_CPP_
#define SRC_CORRECTOR_CPP_

#include "Corrector.h"

const string Corrector::CORRECTED_TEXT = "corrected_text.txt";
size_t CorrectedWord::MAX_SUGGESTIONS = 10;

std::ostream& operator<<(std::ostream& os, CorrectedWord* cw){
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

void CorrectedWord::addCorrection(DictionaryEntry * entry){
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

CorrectedWord::~CorrectedWord(){
	std::set<WordMatch*, WordMatch::MatchComp>::iterator iti = matches.begin();
	std::set<WordMatch*, WordMatch::MatchComp>::iterator ite = matches.end();
	for(; iti != ite ; ++iti){
		delete *iti;
	}

}

string CorrectedWord::correct() const
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

CorrectedText* Corrector::correct(Dictionary& dic, std::string filename){
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

CorrectedLine* Corrector::correctLine(Dictionary& dic, const std::string& line, int linenum){
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

CorrectedWord* Corrector::correctWord(Dictionary& dic, std::string& word, int wordnum){
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

BKTree Corrector::fillBK(Dictionary *dic, std::string filename){
	return dic->fillBKTree();
}

CorrectedText* Corrector::correctBK(const BKTree &tree, Dictionary& dic, std::string filename){
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

CorrectedLine* Corrector::correctLineBK(Dictionary& dic, const std::string& line, int linenum, const BKTree& tree){
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

CorrectedWord* Corrector::correctWordBK(Dictionary& dic, std::string& word, int wordnum,  const BKTree& tree){
	CorrectedWord* out = new CorrectedWord(word, wordnum);
	std::vector<DictionaryEntry*> found = tree.query(word, 2);
	for(size_t i = 0 ; i < found.size(); i++){
		out->addCorrection(found[i]);
	}
	return out;
}

Trie Corrector::fillTrie(Dictionary *dic)
{
	return dic->fillTrie();
}

CorrectedText* Corrector::correctTrie(const Trie &tree, Dictionary& dic, std::string filename){
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

CorrectedLine* Corrector::correctLineTrie(const Dictionary& dic, const std::string& line, int linenum, const Trie& tree){
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

CorrectedWord* Corrector::correctWordTrie(const Dictionary& dic, std::string& word, int wordnum,  const Trie& tree){
	CorrectedWord* out = new CorrectedWord(word, wordnum);
	std::vector<DictionaryEntry*> found = tree.query(word, 2);
	for(size_t i = 0 ; i < found.size(); i++){
		DictionaryEntry* temp = found[i];
		if(temp != NULL)
			out->addCorrection(temp);
	}
	return out;
}

bool Corrector::correctorValidation(const std::string& w1, const std::string& w2){
	return true;
	if(abs((int)w1.length() - (int)w2.length()) < 5){
		return true;
	}
	return false;
}

void Corrector::correctTextDynamic(const std::string& newFile, const std::string &oldFile, Dictionary &dic)
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

std::string Corrector::correctLineDynamic(Dictionary& dic, const std::string& line, int linenum, Trie& tree, const std::string oldFile) {
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

std::string Corrector::correctWordDynamic(Dictionary& dic, std::string& word, int linenum, int wordnum,  Trie& tree, const std::string oldFile) {

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

#endif /* SRC_CORRECTOR_CPP_ */
