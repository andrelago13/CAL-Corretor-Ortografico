/*
 * UserInput.h
 *
 *  Created on: 30/05/2015
 *      Author: André
 */

#ifndef SRC_USERINPUT_H_
#define SRC_USERINPUT_H_

#include <iostream>
#include <string>

class UserInput {
public:
	static int getInt(int min, int max) {
		std::cout << std::endl << "Please select an option between " << min << " and " << max << ": ";

		std::string temp = "";
		std::cin >> temp;
		int num = atoi(temp.c_str());

		while(num < min || num > max)
		{
			std::cout << std::endl << "Invalid option. Must be number between " << min << " and " << max << ": ";
			std::cin >> temp;
			num = atoi(temp.c_str());
		}

		return num;
	}

	static int getInt(int max) {return getInt(0, max);}

	static bool getYesNo() {
		std::cout << std::endl << "Please insert your option (Y/N): ";
		std::string temp = "";
		std::cin >> temp;

		while(temp.size() != 1 || (toupper(temp[0]) != 'Y' && toupper(temp[0]) != 'N'))
		{
			std::cout << "Invalid option, must be Y or N: ";
			std::cin >> temp;
		}

		return toupper(temp[0]) == 'Y';
	}
};




#endif /* SRC_USERINPUT_H_ */
