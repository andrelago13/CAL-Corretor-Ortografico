/*
 * Trie.h
 *
 *  Created on: May 21, 2015
 *      Author: Ricardo
 */

#ifndef SRC_TRIE_H_
#define SRC_TRIE_H_

#include <vector>
#include <string>
#include <cstring>
#include <cctype>
#include <iostream>
#include "DictionaryEntry.h"

#define ALPHABET_SIZE (26*2+2)
#define min_(a,b) (a < b ? a :b)

using namespace std;

class TrieNode{
	TrieNode **children;
	DictionaryEntry* endpoint;
public:
	static unsigned long long id;
	TrieNode();
	void insertEntry(DictionaryEntry* entry, const string& word, int index);
	char unmap(size_t i) const;
	void query(vector<DictionaryEntry*>& output, const string& word, int maxdist, int index, vector<vector<int> >& matrix, string& current) const;
	~TrieNode();
	static int map(const unsigned char& c);
	void print(string& word);
};

class Trie{
	TrieNode* root;
public:
	Trie():root(new TrieNode()){};
	~Trie();
	vector<DictionaryEntry*> query(const string& word, int maxdist) const;
	void insertEntry(DictionaryEntry* entry, const string& word);
	void print();
};

#endif /* SRC_TRIE_H_ */
