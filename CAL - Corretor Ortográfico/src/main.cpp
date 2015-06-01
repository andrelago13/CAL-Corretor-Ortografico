/*
 * main.cpp
 *
 *  Created on: May 17, 2015
 *      Author: Ricardo
 */

#include <iostream>
#include <stdio.h>
#include "Dictionary.h"
#include "includes/dirent.h"
#include "Corrector.h"
#include "Trie.h"
#include "Benchmark.h"
#include "TrieCorrectBenchmark.h"
#include "BKTreeBenchmark.h"
#include <chrono>

#define DICTIONARY_DIR "dictionaries/"
#define DICTIONARY_EXT "txt"
#define TEXT_DIR "texts/"
#define TEXT_EXT "txt"

using namespace std;

bool file_exists(const string& filename) {
	ifstream f(filename.c_str());
	if (f.good()) {
		f.close();
		return true;
	} else {
		f.close();
		return false;
	}
}

Dictionary* choose_read_dictionary(const string& dirName) {
	DIR *dir;
	struct dirent *ent;
	vector<string> dictionaries;
	if ((dir = opendir (dirName.c_str())) != NULL) {

		while ((ent = readdir (dir)) != NULL) {

			string filename = ent->d_name;
			if(ent->d_type == DT_REG && (filename.substr(filename.find_last_of(".") + 1) == DICTIONARY_EXT) && (filename.find_first_of("_") == string::npos)) {
				dictionaries.push_back(filename);
			}
		}
		closedir (dir);
	} else {
		cerr << "Error reading dictionaries folder" << endl;
		free(dir);
		free(ent);
		return NULL;
	}

	string dictionary = DICTIONARY_DIR;

	if(dictionaries.size() == 0) {
		cout << "No dictionary files were found." << endl;
		free(dir);
		free(ent);
		return NULL;
	}

	bool processed = false;
	if(dictionaries.size() == 1) {
		cout << "Choosing dictionary " << dictionaries[0] << " (only one available)" << endl;
		dictionary += dictionaries[0];
		processed = (dictionary.find_last_of("_") != string::npos);
	} else {
		cout << "Please select a dictionary:" << endl;
		for(size_t i = 0; i < dictionaries.size(); i++) {
			cout << i+1 << " - " << dictionaries[i] << endl;
		}
		int option = UserInput::getInt(1, dictionaries.size());
		dictionary += dictionaries[option-1];
	}

	string temp = dictionary;
	temp.insert(dictionary.find_last_of("."), "_p");

	if(file_exists(temp)) {
		dictionary = temp;
		processed = true;
		cout << "Switched to " << dictionary << " because it is already processed." << endl;
	}

	cout << "Reading dictionary" << endl;
	Dictionary* dic = new Dictionary(dictionary, processed);
	cout << "Done Reading dictionary" << endl;

	if(!processed) {
		dic->writeDictionary(temp);
		cout << "Saved recently processed dictionary" << endl;
	}

	free(dir);
	free(ent);
	return dic;
}

string choose_file(const string& dirName) {
	DIR *dir;
	struct dirent *ent;
	vector<string> files;
	if ((dir = opendir (dirName.c_str())) != NULL) {

		while ((ent = readdir (dir)) != NULL) {

			string filename = ent->d_name;
			if(ent->d_type == DT_REG && (filename.substr(filename.find_last_of(".") + 1) == TEXT_EXT) && filename != Corrector::CORRECTED_TEXT) {
				files.push_back(filename);
			}
		}
		closedir (dir);
	} else {
		cerr << "Error reading text files folder" << endl;
		free(dir);
		free(ent);
		return "";
	}

	string file = TEXT_DIR;

	if(files.size() == 0) {
		cout << "No text files were found." << endl;
		free(dir);
		free(ent);
		return "";
	}

	if(files.size() == 1) {
		cout << "Do you wish to correct file \"" << files[0] << "\" (only one available)?" << endl;
		if(UserInput::getYesNo()) {
			file += files[0];
			free(dir);
			free(ent);
			return file;
		}
		else {
			free(dir);
			free(ent);
			return "";
		}
	}

	cout << "Please select a text file:" << endl;
	for(size_t i = 0; i < files.size(); i++) {
		cout << i+1 << " - " << files[i] << endl;
	}
	int option = UserInput::getInt(1, files.size());
	file += files[option-1];
	cout << "Chosen file \"" << file << "\"." << endl;
	free(dir);
	free(ent);
	return file;
}

void run()
{
	Dictionary* dic = choose_read_dictionary(DICTIONARY_DIR);
	if(dic == NULL) return;

	//string file = choose_file(TEXT_DIR);	//choose_file(TEXT_DIR);
	//if(file == "") return;

	//system("pause");

	//string file = "example1.txt";
	string file2 = "errors.txt";
	//dic.countWholeWords(file);

	//CorrectedText* corr = Corrector::correctTrie(dic, file2);
	//	Trie tree =  dic.fillTrie();

	//Corrector::correctTextDynamic(Corrector::CORRECTED_TEXT, file, *dic);

	//tree.print();
	//dic.debug();
	//cout << dic << endl;
	//cout << corr << endl;
	//dic.writeDictionary("dictionary2.txt");
	//cout << "Number of nodes: " << TrieNode::id << endl;
	//cout << "Size of each node: " << sizeof(TrieNode) << endl;
	//delete corr;

	vector<Benchmark *> benchmarks;
	//benchmarks.push_back(new TrieCorrectBenchmark(1, file2, dic));
	//benchmarks.push_back(new BKTreeFillBenchmark(1, file2, dic));
	//benchmarks.push_back(new BKTreeCorrectBenchmark(1, file2, dic, Corrector::fillBK(dic, file2)));

	for (size_t i = 0; i < benchmarks.size(); ++i)
	{
		cerr << benchmarks[i]->name << " took " << benchmarks[i]->run() / ((double)1000 * 1000 * 1000) << " seconds." << std::endl;
		cerr << "delete start" << endl;
		delete benchmarks[i];
		cerr << "delete end" << endl;
	}
	delete(dic);
	cout << "Terminating" << endl;
}

int main(){
	run();
	return 0;
}
