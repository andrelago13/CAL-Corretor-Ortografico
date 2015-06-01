/*
 * BKTreeBenchmark.h
 *
 *  Created on: 01/06/2015
 *      Author: Gustavo
 */

#ifndef SRC_BKTREEBENCHMARK_H_
#define SRC_BKTREEBENCHMARK_H_

#include "Benchmark.h"

class BKTreeFillBenchmark: public Benchmark {
private:
	const std::string &oldFile;
	Dictionary *dic;
	void f() const
	{
		Corrector::fillBK(dic, oldFile);
	}
public:
	BKTreeFillBenchmark(unsigned times, const std::string &oldFile, Dictionary *dic): Benchmark("Filling BK Tree", times), oldFile(oldFile), dic(dic) { }
	virtual ~BKTreeFillBenchmark() {};
};

class BKTreeCorrectBenchmark: public Benchmark {
private:
	const std::string &oldFile;
	Dictionary *dic;
	const BKTree &tree;
	void f() const
	{
		Corrector::correctBK(tree, *dic, oldFile);
	}
public:
	BKTreeCorrectBenchmark(unsigned times, const std::string &oldFile, Dictionary *dic, const BKTree &tree): Benchmark("Correcting using BK Tree", times), oldFile(oldFile), dic(dic), tree(tree) { }
	virtual ~BKTreeCorrectBenchmark() {};
};

#endif /* SRC_BKTREEBENCHMARK_H_ */
