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
	void f()
	{
		Corrector::correctBK(dic, oldFile);
	}
public:
	BKTreeBenchmark(const std::string &oldFile, Dictionary &dic);
	virtual ~BKTreeBenchmark();
};

#endif /* SRC_BKTREEBENCHMARK_H_ */
