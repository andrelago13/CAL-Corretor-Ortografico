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

using namespace std;

class BKTreeNode{
	DictionaryEntry* entry;
	vector<BKTreeNode*> children;
public:
	BKTreeNode(DictionaryEntry* entry):entry(entry){};
	void insertEntry(DictionaryEntry* newEntry){
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
	void query(vector<DictionaryEntry*>& output, const string& word, int maxdist) const{
		//cerr << "query, ";
		int dist = editDistance(word, entry->getWord());
		if(dist <= maxdist){
			entry->setLastEditDistance(dist);
			output.push_back(entry);
		}
		/*cerr << word << ", " << entry->getWord() << this;
		for(int i = 0 ; i < output.size(); i++){
			cerr << output[i] <<", ";
		}
		cerr << endl;*/
		int beginning = max(0, dist - maxdist);
		int end = min( (int)children.size()-1,  dist + maxdist);
		for(int i = beginning; i <= end; i++){
			if(children[i] != NULL)
				children[i]->query(output, word, maxdist);
		}
	}
	~BKTreeNode(){
		for(size_t i = 0; i < children.size(); i++){
			if(children[i] != NULL){
				delete children[i];
			}
		}
	}
};

class BKTree{
	BKTreeNode* root;
public:
	BKTree():root(NULL){};
	~BKTree(){
		if(root != NULL)
			delete root;
	}
	vector<DictionaryEntry*> query(const string& word, int maxdist) const{
		vector<DictionaryEntry*> out;
		if(root != NULL)
			root->query(out, word, maxdist);
		return out;
	}
	void insertEntry(DictionaryEntry* newEntry){
		if(root == NULL)
			root = new BKTreeNode(newEntry);
		else root->insertEntry(newEntry);
	}
};


#endif /* SRC_BKTREE_H_ */
