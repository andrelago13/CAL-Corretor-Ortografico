/*
 * BKTree.h
 *
 *  Created on: May 20, 2015
 *      Author: Ricardo
 */

#ifndef SRC_BKTREE_H_
#define SRC_BKTREE_H_

#include "matcher.h"
#include <cstdlib>
#include <vector>
#include <string>
#include "DictionaryEntry.h"

using namespace std;

class BKTreeNode{
	DictionaryEntry* entry;
	vector<BKTreeNode*> children;
public:
	BKTreeNode(DictionaryEntry* entry):entry(entry) {}
	void insertEntry(DictionaryEntry* newEntry);
	void query(vector<DictionaryEntry*>& output, const string& word, int maxdist) const;
	~BKTreeNode();
};

class BKTree{
	BKTreeNode* root;
public:
	BKTree():root(NULL){};
	~BKTree();
	vector<DictionaryEntry*> query(const string& word, int maxdist) const;
	void insertEntry(DictionaryEntry* newEntry);
};


#endif /* SRC_BKTREE_H_ */
