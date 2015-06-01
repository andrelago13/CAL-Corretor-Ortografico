/*
 * Benchmark.h
 *
 *  Created on: 01/06/2015
 *      Author: Gustavo
 */

#ifndef SRC_BENCHMARK_H_
#define SRC_BENCHMARK_H_

#include <chrono>

class Benchmark {
private:
	unsigned times;
protected:
	virtual void f() const = 0;
public:
	const string name;
	Benchmark(const string &name, unsigned times): name(name), times(times) { }
	virtual std::chrono::nanoseconds::rep run() const
	{
		auto begin = std::chrono::high_resolution_clock::now();
		for (size_t i = 0; i < times; ++i)
		{
			f();
		}
		auto end = std::chrono::high_resolution_clock::now();
		return std::chrono::duration_cast<std::chrono::nanoseconds>(end-begin).count() / times;
	}
	virtual ~Benchmark() {};
};

#endif /* SRC_BENCHMARK_H_ */
