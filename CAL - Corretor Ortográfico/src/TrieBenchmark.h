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

class TrieBenchmark: public Benchmark {
private:
	const std::string &oldFile;
	Dictionary &dic;
	void f() const
	{
		Corrector::correctTrie(dic, oldFile);
	}
public:
	TrieBenchmark(unsigned times, const std::string &oldFile, Dictionary &dic): Benchmark("Trie", times), oldFile(oldFile), dic(dic) { }
	virtual ~TrieBenchmark() {};
};

#endif /* SRC_TRIEBENCHMARK_H_ */
