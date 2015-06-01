/*
 * BKTreeBenchmark.h
 *
 *  Created on: 01/06/2015
 *      Author: Gustavo
 */

#ifndef SRC_BKTREEBENCHMARK_H_
#define SRC_BKTREEBENCHMARK_H_

#include "Benchmark.h"

class BKTreeBenchmark: public Benchmark {
private:
	const std::string &oldFile;
	Dictionary &dic;
	void f() const
	{
		Corrector::correctBK(dic, oldFile);
	}
public:
	BKTreeBenchmark(unsigned times, const std::string &oldFile, Dictionary &dic): Benchmark("BK Tree", times), oldFile(oldFile), dic(dic) { }
	virtual ~BKTreeBenchmark() {};
};

#endif /* SRC_BKTREEBENCHMARK_H_ */
