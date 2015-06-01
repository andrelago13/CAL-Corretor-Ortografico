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
#include "Benchmark.h"
#include "TrieCorrectBenchmark.h"
#include "BKTreeBenchmark.h"
#include <chrono>

using namespace std;

void run()
{
	cout << "Reading dictionary" << endl;
	Dictionary dic("dictionary2.txt", true);
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

	vector<Benchmark *> benchmarks;
	benchmarks.push_back(new TrieCorrectBenchmark(1, file2, dic));
	benchmarks.push_back(new BKTreeFillBenchmark(1, file2, dic));
	benchmarks.push_back(new BKTreeCorrectBenchmark(1, file2, dic, Corrector::fillBK(dic, file2)));
	for (size_t i = 0; i < benchmarks.size(); ++i)
	{
		cerr << benchmarks[i]->name << " took " << benchmarks[i]->run() / ((double)1000 * 1000 * 1000) << " seconds." << std::endl;
		cerr << "delete start" << endl;
		//delete benchmarks[i];
		cerr << "delete end" << endl;
	}
}

int main(){
	run();
	return 0;
}
