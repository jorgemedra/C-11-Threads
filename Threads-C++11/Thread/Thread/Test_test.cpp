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

	TestA *appA = new TestA();
	TestB *appB = new TestB();

	appA->start();
	appB->start();

	cin.getline(line, 256);

	if (line[0] == 'A' || line[0] == 'a')
	{
		appA->resumePA();
		appB->resumePA();
	}
	else
	{
		appA->resumePB();
		appB->resumePB();
	}

	//std::cout << "Press Enter to finish" << endl;
	//cin.getline(line, 256);
	system("Pause");

	appA->stop();
	appB->stop();

	delete appA;
	delete appB;

	system("Pause");

	return 0;
}
