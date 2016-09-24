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

Test::Test() {

	ptrPA = &Test::procA;
	ptrPB = &Test::procB;
	this->printLog("--- Class Test Created ---");
}

Test::~Test() {
	this->printLog("--- Class Test Destroyed ---");
}

void Test::EntryPoint(Test* obj, PTRP ptr,string name)
{
	(obj->*(PTRP)ptr)(name);
}


void Test::start()
{
	string nameA = "Proceso A";
	string nameB = "Proceso B";

	if(A == NULL)
	{
		A = new std::thread(Test::EntryPoint, this, ptrPA,nameA);
		B = new std::thread(Test::EntryPoint, this, ptrPB,nameB);
	}
}

void Test::stop()
{

}

void Test::printLog(string msg)
{
	mtx_l.lock();

	std::cout << msg << endl;

	mtx_l.unlock();

}

void Test::resumePA()
{
	printLog("Resuming Thread A.");
	cv_a.notify_one();
}

void Test::resumePB()
{
	printLog("Resuming Thread B.");
	cv_b.notify_one();
}


void Test::procA(string name)
{
	int i = 0;
	ostringstream o;
	o << "Thread A [" << name << "] is started and Wait for 20 seconds or a notify";
	printLog(o.str());
	std::unique_lock<std::mutex> lck_a(mtx_a);

	cv_a.wait_for(lck_a,chrono::seconds(20));

	printLog("Trehad A resumed.");

	for(i=0; i< 500; i++)
	{
		ostringstream c;
		cv_a.wait_for(lck_a,chrono::microseconds(10));
		c << "A(" << (i+1) << ")";
		printLog(c.str());
	}

	printLog("Thread A finishded, trying to resume Thread B...");
	cv_b.notify_one();
}

void Test::procB(string name)
{
	int i = 0;
	ostringstream o;
	o << "Thread B [" << name << "] is started and Wait for 20 seconds or a notify";
	printLog(o.str());
	std::unique_lock<std::mutex> lck_b(mtx_b);

	cv_b.wait_for(lck_b,chrono::seconds(20));


	printLog("Trehad B resumed.");

	for(i=0; i< 500; i++)
	{
		ostringstream c;
		cv_b.wait_for(lck_b,chrono::microseconds(10));
		c << "B(" << (i+1) << ")";
		printLog(c.str());
	}

	printLog("Thread B finishded, trying to resume Thread A...");
	cv_a.notify_one();
}
