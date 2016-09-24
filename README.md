# Using Thread C++11

## Requirements

**Linux:**

- Eclipse IDE for C/C++ Developers - Version: Neon Release (4.6.0)
- Compiler g++ 5.4.0

> **Tip**: You can create a Linux enviroment under windows using Cygwin and Eclipse IDE.

**Windows**

- Microsoft Visual Studio Community 2015
- Microsoft (R) C/C++ Optimizing Compiler Version 19.00.24210 for x86


# Context

This sample shows how to:

- Implement C++11 Thread.
- Create two threads over two methods into the same class.
- Use condition_variable to suspend each thread and resume these.
- use of unique_lock with condition_variable;
- Use Mutex to synchronize shared zone.
- Use Conditional Variable to suspend a thread and resume after some time.

# Theory

## The Test Class

The Test Class has two methods `procA` and `procA`. These methods will be executes by different thread, called `std::thread *A` and `std::thread *B`. These methods have the same input parameters (`string name`) and the same return type (`void`).

```c++
void procA(string name);
void procB(string name);


```

The methods `resumePA` and `resumePB` will be used to resume the execution of methods `procA` and `procB`.

```c++
void Test::resumePA()
{
	...
}

void Test::resumePB()
{
	...
}
```

There is a common method used by process `procA` and `procB`, this method is `printLog` which has the function to printLog  logs in synchronized way.

```c++
void Test::printLog(string msg)
{
	...
}
```

Each of both methods, `procA` and `procA`, has their own `condition_variable` and `mutex`.

```c++
std::mutex mtx_a;
std::condition_variable cv_a;

mutex mtx_b;
std::condition_variable cv_b;
```

The Class has a prototype of pointer that will be used to create two pointer to methods `procA` and `procB`:

```c++
typedef void (Test::*PTRP) (string);
PTRP ptrPA;
PTRP ptrPB;
```

At last, the Test Class has a static method which will be used by `thread` class as Callback method. Is important to say that is not possible to invoke a method class as Callback function because they (class methods) has a relative address and the `thread` class need a function that has a final address, as static address. This method will recive three parameters:

- A pointer to `Test` class.
- A `Test::*PTRP` pointer. The pointer will be used to execute one of these methods: `procA` or `procB`.
- A string variable with the name of the thread.

```c++
static void EntryPoint(Test*, PTRP, string);
```

> **Tip**: Eache thread that execute the method `EntryPoint` creates its own memory stack.

# The execution

## The Main

The main will create a Test class start the execution of both threads, using the method `app->start()`. Also, the main thread, wait for an keyboard input, letter **a** or any other; if the letter is **a** the main thread resume the execution of thread A or thread B if is another letter than **a**.

```c++
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
```

## The Test Constructor

The Test Constructor sets the `PTRP` pointer for each method, `procA` and `procB`.

```c++
Test::Test() {

	ptrPA = &Test::procA;
	ptrPB = &Test::procB;
	this->printLog("--- Class Test Created ---");
}
```

## The `start` method

The `start` method creates the threads **A** and **B** and sets the parameters for each one. Each `thread` will invoke the static method `Test::EntryPoint`. Each invocation creates its own memory stack and sends, as a parameter, the pointer class method which will be executed as part of the thread process.

```c++
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
```

## The `EntryPoint` method

The `EntryPoint` invoke the class methods by the pointer to `Test` class and `PTRP` pointer and set the string parameter `name`.

```c++
void Test::EntryPoint(Test* obj, PTRP ptr,string name)
{
	(obj->*(PTRP)ptr)(name);
}
```

## The methods `procA` and `procB`

The methods `procA` and `procB` are executed by each thread, according with the pointer recived into `Entity` method.

Each of them will bee waiting for 20 second until some input of key unlock the one of them. After the key input, one of these threads are unlocked and the process execution perform a loop of 500 interactions, for each interaction the process wait for 1 microseconds and resume the process, like a `wait()` of java code.

After the loop was executed, the process unlock the other process, that is still waiting 20 seconds for unlock.   

```c++
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
```

### To Make Waiting the process

Each method, `procA` or `procB`, starts waiting 20 seconds. To make the thread wait without usign `sleep`methos it is necesary to use the class `std::unique_lock`. The `unique_lock` is linked with its mutex object.

The `condition_variable` is used to susped the execution of thread until the time is over, set by `chrono::seconds(20)`, or the  `condition_variable` is resumed (unlocked).

> **Tip**: It is important to declare  `std::unique_lock` into the method where will be used because there are some compiles, as Visual C++, that does not allow its declaration into the header file.

```c++

void Test::procB(string name)
{
	int i = 0;
	ostringstream o;
	o << "Thread B [" << name << "] is started and Wait for 20 seconds or a notify";
	printLog(o.str());

	std::unique_lock<std::mutex> lck_b(mtx_b);
	cv_b.wait_for(lck_b,chrono::seconds(20));
  ...
}
```

For each interaction the process will be make a pause, 10 microsecond.

> **Tip**: This step avoids consume the process time.

```c++
void Test::procB(string name)
{
  ...
	printLog("Trehad B resumed.");

	for(i=0; i< 500; i++)
	{
		ostringstream c;
		cv_b.wait_for(lck_b,chrono::microseconds(10));
		c << "B(" << (i+1) << ")";
		printLog(c.str());
	}
  ...
}
```

At the end, the process will unlock the other process which is still waiting 20 seconds.

```c++
void Test::procB(string name)
{
  ...
  printLog("Thread B finishded, trying to resume Thread A...");
  cv_a.notify_one();
}
```

### The `resume` method

The resume method is used by main thread to unlock one of the theads which is waiting 20 seconds.

```c++
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
```

### Thre `printLog`

The print log is a method used by `procA` and `procB`. This method is synchronized with its own mutex `mtx_l` object. The mutex object avoid that the processes write at the same time their messages log, in case that both thread are unlocked at the same time.

```c++
void Test::printLog(string msg)
{
	mtx_l.lock();
	std::cout << msg << endl;
	mtx_l.unlock();

}
```
