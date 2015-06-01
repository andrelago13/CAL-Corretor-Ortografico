/*
 * Corrector.cpp
 *
 *  Created on: June 1, 2015
 *      Author: Gustavo
 */

#ifndef SRC_BKTREE_CPP_
#define SRC_BKTREE_CPP_

#include "BKTree.h"

void BKTreeNode::insertEntry(DictionaryEntry* newEntry){
	unsigned dist = editDistance(entry->getWord(), newEntry->getWord());
	if(dist >= children.size()){
		int oldsize = children.size();
		children.resize(dist+1);
		for(size_t i = oldsize; i < children.size(); i++){
			children[i] = NULL;
		}
	}
	if(children[dist] == NULL){
		children[dist] = new BKTreeNode(newEntry);
	}
	else children[dist]->insertEntry(newEntry);
}

void BKTreeNode::query(vector<DictionaryEntry*>& output, const string& word, int maxdist) const{
	if (entry == NULL)
		exit(1);
	int dist = editDistance(word, entry->getWord());
	if(dist <= maxdist){
		entry->setLastEditDistance(dist);
		output.push_back(entry);
	}
	int beginning = max(0, dist - maxdist);
	int end = min( (int)children.size()-1,  dist + maxdist);
	for(int i = beginning; i <= end; i++){
		if(children[i] != NULL)
		{
			children[i]->query(output, word, maxdist);
		}
	}
}

BKTreeNode::~BKTreeNode(){
	for(size_t i = 0; i < children.size(); i++){
		if(children[i] != NULL){
			delete children[i];
		}
	}
}

BKTree::~BKTree(){
	if(root != NULL)
		delete root;
}

vector<DictionaryEntry*> BKTree::query(const string& word, int maxdist) const{
	vector<DictionaryEntry*> out;
	if(root != NULL)
		root->query(out, word, maxdist);
	return out;
}

void BKTree::insertEntry(DictionaryEntry* newEntry){
	if(root == NULL)
		root = new BKTreeNode(newEntry);
	else root->insertEntry(newEntry);
}

#endif /* SRC_BKTREE_CPP_ */
