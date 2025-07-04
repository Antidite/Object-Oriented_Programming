#include <chrono>
#include <iostream>
#include <random>
#include <vector>
#include <chrono>
#include <utility>
#include "allocator_selector.h"

#define INT 1
#define FLOAT 2
#define DOUBLE 3
#define CLASS  4

class vecWrapper
{
public:
	vecWrapper() {
		m_pVec = NULL;
		m_type = INT;
	}
	virtual ~vecWrapper()	{
	}
public:
	void setPointer(int type, void *pVec) { m_type = type; m_pVec = pVec; }
	virtual void visit(int index) = 0;
	virtual int size() = 0;
	virtual void resize(int size) = 0;
	virtual bool checkElement(int index, void *value) = 0;
	virtual void setElement(int idex, void *value) = 0;
protected: 
	int m_type;
	void *m_pVec;
};

template<class T>
class vecWrapperT : public vecWrapper
{
public:
	vecWrapperT(int type, std::vector<T, MyAllocator<T> > *pVec)
	{ 
		m_type = type;
		m_pVec = pVec;
	}
	virtual ~vecWrapperT() {
		if (m_pVec)
			delete ((std::vector<T, MyAllocator<T> > *)m_pVec);
	}
public:
	virtual void visit(int index)
	{
		T temp = (*(std::vector<T, MyAllocator<T> > *)m_pVec)[index];
		(void)temp;
	}
	virtual int size()
	{
		return ((std::vector<T, MyAllocator<T> > *)m_pVec)->size();
	}
	virtual void resize(int size)
	{
		((std::vector<T, MyAllocator<T> > *)m_pVec)->resize(size);
	}
	virtual bool checkElement(int index, void *pValue)
	{
		T temp = (*(std::vector<T, MyAllocator<T> > *)m_pVec)[index];
		if (temp == (*((T *)pValue)))
			return true;
		else
			return false;
	}

	virtual void setElement(int index, void *value)
	{
		(*(std::vector<T, MyAllocator<T> > *)m_pVec)[index] = *((T *)value);
	}
};

class myObject
{
public:
	myObject() : m_X(0), m_Y(0) {}
	myObject(int t1, int t2) :m_X(t1), m_Y(t2) {}
	myObject(const myObject &rhs) { m_X = rhs.m_X; m_Y = rhs.m_Y;}
	~myObject() { /*std::cout << "my object destructor called" << std::endl;*/ }
	bool operator == (const myObject &rhs)
	{
		if ((rhs.m_X == m_X) && (rhs.m_Y == m_Y))
			return true;
		else
			return false;
	}
protected:
	int m_X;
	int m_Y;
};

#define TESTSIZE 10000
#define PICKSIZE 1000

int main()
{
	using Clock = std::chrono::high_resolution_clock;
    using us    = std::chrono::microseconds;
	auto t0 = Clock::now();
	vecWrapper **testVec;
	testVec = new vecWrapper*[TESTSIZE];

	int tIndex, tSize;
	//test allocator
	for (int i = 0; i < TESTSIZE - 4; i++)
	{
		tSize = (int)((float)rand()/(float)RAND_MAX * 100);
		vecWrapperT<int> *pNewVec = new vecWrapperT<int>(INT, new std::vector<int, MyAllocator<int>>(tSize));
		testVec[i] = (vecWrapper *)pNewVec;
	}

	for (int i = 0; i < 4; i++)
	{
		tSize = (int)((float)rand() / (float)RAND_MAX * 10000);
		vecWrapperT<myObject> *pNewVec = new vecWrapperT<myObject>(CLASS, new std::vector<myObject, MyAllocator<myObject>>(tSize));
		testVec[TESTSIZE - 4 + i] = (vecWrapper *)pNewVec;
	}
	auto t1 = Clock::now();
    std::cout << "Test Allocator: "
              << std::chrono::duration_cast<us>(t1 - t0).count()
              << " µs\n";
	//test resize
	for (int i = 0; i < PICKSIZE; i++)
	{
		tIndex = (int)((float)rand() / (float)RAND_MAX * (float)TESTSIZE);
		tSize = (int)((float)rand() / (float)RAND_MAX * (float)TESTSIZE);
		testVec[tIndex]->resize(tSize);
	}
	auto t2 = Clock::now();
    std::cout << "Test Resize: "
              << std::chrono::duration_cast<us>(t2 - t1).count()
              << " µs\n";

	//test assignment
	tIndex = (int)((float)rand() / (float)RAND_MAX * (TESTSIZE - 4 - 1));
	int tIntValue = 10;
	testVec[tIndex]->setElement(testVec[tIndex]->size()/2, &tIntValue);
	if (!testVec[tIndex]->checkElement(testVec[tIndex]->size()/ 2, &tIntValue))
		std::cout<< "incorrect assignment in vector %d\n" << tIndex << std::endl;

	tIndex = TESTSIZE - 4 + 3;
	myObject tObj(11,15);
	testVec[tIndex]->setElement(testVec[tIndex]->size()/2, &tObj);
	if (!testVec[tIndex]->checkElement(testVec[tIndex]->size()/2, &tObj))
		std::cout << "incorrect assignment in vector %d\n" << tIndex << std::endl;
	
	myObject tObj1(13, 20);
	if (!testVec[tIndex]->checkElement(testVec[tIndex]->size() / 2, &tObj1))
		std::cout << "incorrect assignment in vector " << tIndex << " for object (13,20)" << std::endl;

	for (int i = 0; i < TESTSIZE; i++)
		delete testVec[i];
	auto t3 = Clock::now();
    std::cout << "Test Assignment: "
              << std::chrono::duration_cast<us>(t3 - t2).count()
              << " µs" << "\n";
	delete []testVec;

	auto t4 = Clock::now();
    std::cout << AllocatorLabel <<"Totally: "
              << std::chrono::duration_cast<us>(t4 - t0).count()
              << " µs\n";
    return 0;
}