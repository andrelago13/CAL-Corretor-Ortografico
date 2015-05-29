/*
 * main.cpp
 *
 *  Created on: May 17, 2015
 *      Author: Ricardo
 */

#include <iostream>
#include "Dictionary.h"
#include "Corrector.h"
#include "Trie.h"

using namespace std;

void useless_function()
{
	cout << "Reading dictionary" << endl;
	Dictionary dic("dictionary2.txt", true);
	cout << "Done Reading dictionary" << endl;
	string file = "example1.txt";
	string file2 = "errors.txt";
	//dic.countWholeWords(file);
	CorrectedText* corr = Corrector::correctTrie(dic, file2);
	//Trie tree =  dic.fillTrie();
	//tree.print();
	//dic.debug();
	//cout << dic << endl;
	cout << corr << endl;
	//dic.writeDictionary("dictionary2.txt");
	cout << "Number of nodes: " << TrieNode::id << endl;
	cout << "Size of each node: " << sizeof(TrieNode) << endl;
	//delete corr;
}

int main(){
	useless_function();

	return 0;
}
