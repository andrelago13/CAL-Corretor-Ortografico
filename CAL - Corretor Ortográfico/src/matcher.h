/*
 * matcher.h
 *
 *  Created on: May 15, 2015
 *      Author: Ricardo
 */

#ifndef SRC_MATCHER_H_
#define SRC_MATCHER_H_

#include <string>
#include <vector>

int numStringMatching(std::string filename, std::string toSearch);
float numApproximateStringMatching(std::string filename, std::string toSearch);
int editDistance(std::string pattern, std::string text);
std::vector<int> computePrefix(std::string& toSearch);

#endif /* SRC_MATCHER_H_ */
