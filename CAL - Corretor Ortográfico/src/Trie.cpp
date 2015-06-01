#include "Trie.h"

unsigned long long TrieNode::id = 0;

TrieNode::TrieNode(): endpoint(NULL) {
	children = NULL;
	++id;
}

void TrieNode::insertEntry(DictionaryEntry* entry, const string& word, int index){
	if(index == word.length()){
		endpoint = entry;
		return;
	}
	int mapIndex = map(word[index]);
	if (children == NULL)
	{
		children = new TrieNode*[ALPHABET_SIZE];
		//memset(children, (int)NULL, ALPHABET_SIZE * sizeof(TrieNode *));
		for (size_t i = 0; i < ALPHABET_SIZE; ++i)
		{
			children[i] = NULL;
		}
	}
	if (children[mapIndex] == NULL)
	{
		children[mapIndex] = new TrieNode();
	}
	children[mapIndex]->insertEntry(entry, word, index+1);
}

char TrieNode::unmap(size_t i) const{
	if(i == ALPHABET_SIZE-2)
		return '-';
	if(i == ALPHABET_SIZE-1)
		return '\'';
	if(i < 26)
		return i + 'A';
	else return i-26 + 'a';
}

void TrieNode::query(vector<DictionaryEntry*>& output, const string& word, int maxdist, int index, vector<vector<int> >& matrix, string& current) const{
	if(index != -1){
		matrix.push_back(vector<int>(word.size()+1));
		matrix[index+1][0] = index+1;
		for(size_t i = 1; i <= word.size(); i++){
			int num = 0;
			if(current[index] != word[i-1]){
				if (current[index] == word[i - 2] && current[index - 1] == word[i - 1]) // Letters swapped
				{
					matrix[index + 1][i] = matrix[index][i - 1];
				}
				else
				{
					num = min_( min_(matrix[index+1][i-1],matrix[index][i]), matrix[index][i-1]);
					matrix[index+1][i] = num+1;
				}
			}
			else {
				matrix[index+1][i] = matrix[index][i-1];
			}
		}
		if(matrix[index+1][index+1] > maxdist)//check
		{
			return;
		}
		else if(endpoint && matrix[index+1][word.length()] <= maxdist){
			endpoint->setLastEditDistance(matrix[index+1][word.length()]);
			output.push_back(endpoint);
		}
	}
	if (children == NULL)
	{
		return;
	}
	for(size_t i = 0; i < ALPHABET_SIZE; i++){
		if(children[i] != NULL){
			current += unmap(i);
			children[i]->query(output, word,maxdist, index +1, matrix, current);
			current.resize(current.size()-1);
			matrix.resize(matrix.size()-1);
		}
	}
}

TrieNode::~TrieNode(){
	if (children == NULL) return;
	for(size_t i = 0; i < ALPHABET_SIZE; i++){
		if(children[i] != NULL){
			delete children[i];
		}
	}
	delete children;
	children = NULL;
}

int TrieNode::map(const unsigned char& c){
	if(c == '\'')
		return ALPHABET_SIZE - 1;
	else if(c == '-')
		return  ALPHABET_SIZE-2;

	if(islower(c))
		return c-'a' +26;
	else if (isupper(c))
		return c-'A';
	else
	{
		cerr << "Unknown character found (ASCII code " << (unsigned)c << ")." << endl;
		exit(1);
	}
}

void TrieNode::print(string& word){
	cerr << word << "\n";
	if(children != NULL)
		for(size_t i = 0; i < ALPHABET_SIZE; i++){
			if(children[i] != NULL){
				word += unmap(i);
				children[i]->print(word);
				word.resize(word.size()-1);
			}
		}
}

Trie::~Trie(){
	if(root != NULL)
		delete root;
}

vector<DictionaryEntry*> Trie::query(const string& word, int maxdist) const{
	vector<DictionaryEntry*> out;
	vector<vector<int> > matrix;
	matrix.push_back(vector<int>(word.size()+1));
	for(size_t i = 0; i <= word.size(); i++){
		matrix[0][i] = i;
	}
	if(root != NULL){
		string in1 = "";
		root->query(out, word, maxdist, -1, matrix, in1);
	}
	return out;
}

void Trie::insertEntry(DictionaryEntry* entry, const string& word){
	root->insertEntry(entry, word, 0);
}

void Trie::print(){
	std::cerr << "\n";
	for(char c = 'A'; c <= 'Z'; c++){
		cerr << root->map(c)<<", ";
	}
	for(char c = 'a'; c <= 'z'; c++){
		cerr << root->map(c)<<", ";
	}
	cerr << root->map('-') << ", ";
	cerr << root->map('\'') << ", ";
	cerr << endl;
	for(int i = 0; i < ALPHABET_SIZE; i++){
		cerr << root->unmap(i)<<", ";
	}
	string word;
	root->print(word);
}
