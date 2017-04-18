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

TestA::TestA() {

	ptrPA = &TestA::procA;
	ptrPB = &TestA::procB;
	this->printLog("--- Class Test Created ---");
}

TestA::~TestA() {
	this->printLog("--- Class Test Destroyed ---");
}

void TestA::EntryPoint(TestA* obj, PTRP ptr,string name)
{
	(obj->*(PTRP)ptr)(name);
}


void TestA::start()
{
	string nameA = "Proceso A";
	string nameB = "Proceso B";

	if(A == NULL)
	{
		A = new std::thread(TestA::EntryPoint, this, ptrPA,nameA);
		B = new std::thread(TestA::EntryPoint, this, ptrPB,nameB);
	}
}

void TestA::stop()
{

}

void TestA::printLog(string msg)
{
	mtx_l.lock();

	std::cout << "[A]\t" << msg << endl;

	mtx_l.unlock();

}

void TestA::resumePA()
{
	printLog("Resuming Thread A.");
	cv_a.notify_one();
}

void TestA::resumePB()
{
	printLog("Resuming Thread B.");
	cv_b.notify_one();
}


void TestA::procA(string name)
{
	int i = 0;
	ostringstream o;
	o << "Thread A [" << name << "] is started and Wait for 20 seconds or a notify";
	printLog(o.str());
	std::unique_lock<std::mutex> lck_a(mtx_a);

	cv_a.wait_for(lck_a,chrono::seconds(20));

	printLog("Thread A resumed.");

	for(i=0; i< 500; i++)
	{
		ostringstream c;
		cv_a.wait_for(lck_a,chrono::microseconds(10));
		c << "TA(" << (i+1) << ")";
		printLog(c.str());
	}

	printLog("Thread A finishded, trying to resume Thread B...");
	cv_b.notify_one();
}

void TestA::procB(string name)
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
		c << "TB(" << (i+1) << ")";
		printLog(c.str());
	}

	printLog("Thread B finishded, trying to resume Thread A...");
	cv_a.notify_one();
}
