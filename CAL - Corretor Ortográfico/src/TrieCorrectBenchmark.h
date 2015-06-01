/*
 * TrieBenchmark.h
 *
 *  Created on: 01/06/2015
 *      Author: Gustavo
 */

#ifndef SRC_TRIECORRECTBENCHMARK_H_
#define SRC_TRIECORRECTBENCHMARK_H_

#include "Benchmark.h"
#include <string>
#include "Dictionary.h"
#include "Corrector.h"

class TrieCorrectBenchmark: public Benchmark {
private:
	const std::string &oldFile;
	Dictionary &dic;
	void f() const
	{
		Corrector::correctTrie(dic, oldFile);
	}
public:
	TrieCorrectBenchmark(unsigned times, const std::string &oldFile, Dictionary &dic): Benchmark("Trie", times), oldFile(oldFile), dic(dic) { }
	virtual ~TrieCorrectBenchmark() {};
};

#endif /* SRC_TRIECORRECTBENCHMARK_H_ */
