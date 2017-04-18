/*
* Test.cpp
*
*  Created on: Sep 19, 2016
*      Author: Jorge Omar Medra Torres
*		WebSite: www.jorgemedra.net
*/

#include "Test.h"
#include <iostream>
#include <sstream>

TestB::TestB() {
	this->printLog("--- Class Test B Created ---");
}

TestB::~TestB() {
	this->printLog("--- Class Test B Destroyed ---");
}

void TestB::start()
{
	string nameA = "Proceso A";
	string nameB = "Proceso B";

	if (A == NULL)
	{
		auto fTA = std::bind(&TestB::procA, this, placeholders::_1);
		auto fTB = std::bind(&TestB::procB, this, placeholders::_1);

		A = new std::thread(fTA, nameA);
		B = new std::thread(fTB, nameB);
	}
}

void TestB::stop()
{

}

void TestB::printLog(string msg)
{
	mtx_l.lock();

	std::cout << "[B]\t" << msg << endl;

	mtx_l.unlock();

}

void TestB::resumePA()
{
	printLog("Resuming Thread A.");
	cv_a.notify_one();
}

void TestB::resumePB()
{
	printLog("Resuming Thread B.");
	cv_b.notify_one();
}


void TestB::procA(string name)
{
	int i = 0;
	ostringstream o;
	o << "Thread A [" << name << "] is started and Wait for 20 seconds or a notify";
	printLog(o.str());
	std::unique_lock<std::mutex> lck_a(mtx_a);

	cv_a.wait_for(lck_a, chrono::seconds(20));

	printLog("Thread A resumed.");

	for (i = 0; i< 500; i++)
	{
		ostringstream c;
		cv_a.wait_for(lck_a, chrono::microseconds(10));
		c << "TA(" << (i + 1) << ")";
		printLog(c.str());
	}

	printLog("Thread A finishded, trying to resume Thread B...");
	cv_b.notify_one();
}

void TestB::procB(string name)
{
	int i = 0;
	ostringstream o;
	o << "Thread B [" << name << "] is started and Wait for 20 seconds or a notify";
	printLog(o.str());
	std::unique_lock<std::mutex> lck_b(mtx_b);

	cv_b.wait_for(lck_b, chrono::seconds(20));


	printLog("Trehad B resumed.");

	for (i = 0; i< 500; i++)
	{
		ostringstream c;
		cv_b.wait_for(lck_b, chrono::microseconds(10));
		c << "TB(" << (i + 1) << ")";
		printLog(c.str());
	}

	printLog("Thread B finishded, trying to resume Thread A...");
	cv_a.notify_one();
}
