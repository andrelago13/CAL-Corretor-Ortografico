/*
 * main.cpp
 *
 *  Created on: May 17, 2015
 *      Author: Ricardo
 */

#include <iostream>
#include <dirent.h>
#include "Dictionary.h"
#include "Corrector.h"
#include "Trie.h"

#define DICTIONARY_DIR "dictionaries/"
#define DICTIONARY_EXT "txt"

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

string choose_dictionary(const string& dirName) {
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
		return "";
	}

	string dictionary = DICTIONARY_DIR;

	if(dictionaries.size() == 0) {
		cout << "No dictionary files were found." << endl;
		return "";
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

		string temp = dictionary;
		temp.insert(dictionary.find_last_of("."), "_p");
		cout << temp << endl;

		if(file_exists(temp)) {
			dictionary = temp;
			processed = true;
		}
		cout << "Chosen " << dictionary << endl;
	}

	return dictionary;
}

void useless_function()
{
	string dictionary = choose_dictionary(DICTIONARY_DIR);
	if(dictionary == "") return;

	system("pause");

	cout << "Reading dictionary" << endl;
	Dictionary dic(dictionary, true);
	cout << "Done Reading dictionary" << endl;
	string file = "example1.txt";
	string file2 = "errors.txt";
	//dic.countWholeWords(file);

	//CorrectedText* corr = Corrector::correctTrie(dic, file2);
	//	Trie tree =  dic.fillTrie();

	Corrector::correctTextDynamic("corrected_text.txt", file2, dic);

	//tree.print();
	//dic.debug();
	//cout << dic << endl;
	//cout << corr << endl;
	//dic.writeDictionary("dictionary2.txt");
	cout << "Number of nodes: " << TrieNode::id << endl;
	cout << "Size of each node: " << sizeof(TrieNode) << endl;
	//delete corr;
}

int main(){
	useless_function();

	return 0;
}
