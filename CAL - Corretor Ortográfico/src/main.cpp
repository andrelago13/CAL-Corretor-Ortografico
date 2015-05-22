/*
 * main.cpp
 *
 *  Created on: May 17, 2015
 *      Author: Ricardo
 */

#include <iostream>
#include "Dictionary.h"
#include "Corrector.h"

using namespace std;

int main(){
	cout << "Reading dictionary" << endl;
	Dictionary dic("dictionary2.txt", true);
	cout << "Done Reading dictionary" << endl;
	string file = "example1.txt";
	string file2 = "errors.txt";
	//dic.countWholeWords(file);
	CorrectedText* corr = Corrector::correctTrie(dic, file2);
//	Trie tree =  dic.fillTrie();
	//tree.print();
	//dic.debug();
	//cout << dic << endl;
	cout << corr << endl;
	//dic.writeDictionary("dictionary2.txt");
	return 0;
}

