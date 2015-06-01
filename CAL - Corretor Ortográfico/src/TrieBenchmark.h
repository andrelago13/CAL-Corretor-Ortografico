/*
 * TrieBenchmark.h
 *
 *  Created on: 01/06/2015
 *      Author: Gustavo
 */

#ifndef SRC_TRIEBENCHMARK_H_
#define SRC_TRIEBENCHMARK_H_

#include "Benchmark.h"
#include <string>
#include "Dictionary.h"
#include "Corrector.h"

class TrieFillBenchmark: public Benchmark {
private:
	Dictionary *dic;
	void f() const
	{
		Corrector::fillTrie(dic);
	}
public:
	TrieFillBenchmark(unsigned times, Dictionary *dic): Benchmark("Filling Trie", times), dic(dic) { }
	virtual ~TrieFillBenchmark() {};
};

class TrieCorrectBenchmark: public Benchmark {
private:
	const std::string &oldFile;
	Dictionary *dic;
	const Trie &tree;
	void f() const
	{
		Corrector::correctTrie(tree, *dic, oldFile);
	}
public:
	TrieCorrectBenchmark(unsigned times, const std::string &oldFile, Dictionary *dic, const Trie &tree): Benchmark("Correcting using Trie", times), oldFile(oldFile), dic(dic), tree(tree) { }
	virtual ~TrieCorrectBenchmark() {};
};

#endif /* SRC_TRIEBENCHMARK_H_ */
