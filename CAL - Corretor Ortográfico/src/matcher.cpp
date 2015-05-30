/*
 * matcher.cpp
 *
 *  Created on: May 15, 2015
 *      Author: Ricardo
 */

#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>
#include <sstream>
#include <cmath>
#include <cctype>

#include "matcher.h"
using namespace std;

vector<int> computePrefix(std::string& toSearch) {
	vector<int> out(toSearch.length()+1);
	out[1] = 0;
	int k = 0;
	for(size_t q = 2; q < toSearch.length() +1; q++){
		while(k>0 && toSearch[k+1] != toSearch[q])
			k = out[k];
		if(toSearch[k+1] == toSearch[q])
			k++;
		out[q] = k;
	}
	return out;
}

int numStringMatching(string filename, string toSearch){
	ifstream fin(filename.c_str());
	vector<int> prefix = computePrefix(toSearch);
	int count = 0;
	while(!fin.eof()){
		string line;
		getline(fin, line);
		int pi = 0;
		for(int i = 0; i <= line.length(); i++){
			while(pi > 0 && toSearch[pi] != line[i])
				pi = prefix[pi-1];
			if(toSearch[pi] == line[i])
				pi++;
			if(pi == toSearch.size()){
				count++;
				pi =  prefix[pi-1];
			}
			/*if(toSearch[pi] == line[i]){
				pi++;
				if(pi == prefix.size()-1){
					count++;
					pi -= prefix[pi];
				}
			}
			else{
				pi -= prefix[pi];
			}*/
		}
	}
	return count;
}

#define min_(a,b) (a < b ? a :b)

int editDistance(string pattern, string text){
	vector<vector<int> > matrix;
	for(int i = 0; i < pattern.size()+1; i++){
		matrix.push_back(vector<int>(text.size()+1));
	}
	for(int j = 0; j < pattern.size() + 1; j++){
		matrix[j][0] = j;
	}
	for(int i = 0; i < text.size() + 1; i++){
		matrix[0][i] = i;
	}
	for(int j = 1; j < pattern.size() + 1; j++){
		for(int i = 1; i < text.size() + 1; i++){
			int num = 0;
			if(toupper(pattern[j-1]) != toupper(text[i-1])){
				num = min_( min_(matrix[j][i-1],matrix[j-1][i]), matrix[j-1][i-1]);
				matrix[j][i] = num+1;
			}
			else {
				matrix[j][i] = matrix[j-1][i-1];
			}
		}
	}
	return matrix[pattern.size()][text.size()];
}

float numApproximateStringMatching(string filename, string toSearch){
	ifstream fin(filename.c_str());

	int count = 0;
	int acc = 0;
	cout  << editDistance("mara", "paulo");
	while(!fin.eof()){
		string line;
		getline(fin, line);
		istringstream iss(line);
		string token;
		while(getline(iss, token, ' '))
		{
		    count++;
		    acc += editDistance(toSearch, token);
		}
	}
	cout << count <<  endl;
	return ((float) acc) / count;
}
