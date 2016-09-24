/*
 * Test_test.cpp
 *
 *  Created on: Sep 19, 2016
 *      Author: Jorge Omar Medra Torres
 *		WebSite: www.jorgemedra.net
 */

#include "Test.h"
#include <iostream>
#include <sstream>

int main(int argc, char* args[])
{
	char line[256];

	std::cout << "Press Enter to resume" << endl;

	Test *app = new Test();
	app->start();
	cin.getline(line, 256);

	if(line[0] == 'A' || line[0] == 'a')
		app->resumePA();
	else
		app->resumePB();

	std::cout << "Press Enter to finish" << endl;
	cin.getline(line, 256);

	app->stop();

	delete app;
	return 0;
}
