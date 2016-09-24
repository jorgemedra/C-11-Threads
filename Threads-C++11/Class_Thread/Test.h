/*
 * Test.h
 *
 *  Created on: Sep 19, 2016
 *      Author: Jorge Omar Medra Torres
 *		WebSite: www.jorgemedra.net
 */

#ifndef TEST_H_
#define TEST_H_

#include <cstdio>
#include <cstdlib>
#include <unistd.h>

#include <condition_variable>
#include <thread>
#include <chrono>

using namespace std;


class Test {

	typedef void (Test::*PTRP) (string);

	PTRP ptrPA;
	PTRP ptrPB;

	mutex mtx_l;

	std::mutex mtx_a;
	std::condition_variable cv_a;

	mutex mtx_b;
	std::condition_variable cv_b;

	std::thread *A = NULL;
	std::thread *B = NULL;

public:
	static void EntryPoint(Test*, PTRP, string);

	Test();
	virtual ~Test();

	void start();
	void stop();

	void printLog(string msg);

	void resumePA();
	void resumePB();

	void procA(string name);
	void procB(string name);



};

#endif /* TEST_H_ */
