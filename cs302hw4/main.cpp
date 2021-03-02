/**********************
*
*   Name: Garett Pascual-Folster, 2001691416, Assignment 4
*   Description: Use a set of threads to sort a given array
*   Input: 2 ints (listSize: size of the list; numThreads: number of threads;) Assuming each
*			integer input is a power of 2
*   Output: A sorted list
*
**********************/

#include <iostream>
#include <thread>
#include <string>
#include <vector>
#include <random>

using namespace std;

vector<int> _list;

void insertionSort(int left, int right);

void mergeLists(int leftLeft, int leftRight, int rightLeft, int rightRight);

int main() {
	// get inputs for listSize and numThreads
	int listSize, numThreads;
	string input;

	do { // get listSize
		cout << "list size = ";
		cin >> input;
		try {
			listSize = stoi(input);
		} catch (exception &error) {
			cout << "not a number" << endl;
			listSize = -1;
		}
	} while (listSize < 0);

	int maxThreads = thread::hardware_concurrency();
	do { // get numThreads
		cout << "(Max Threads = " << maxThreads << ") Number of threads = ";
		cin >> input;
		try {
			numThreads = stoi(input);
		} catch (exception &error) {
			cout << "not a number" << endl;
			numThreads = -1;
		}
	} while (numThreads <= 0 || numThreads > maxThreads);



	// initialize the vector _list
	srand((unsigned)time(NULL)); // randomize seed
	for (int i = 0; i < listSize; i++) {
		int num = rand() % listSize + 1;
		_list.push_back(num);
		cout << num << endl;
	}



	// create threads on each sublist using insertionSort function
	vector<thread> threadPool;
	int size = listSize / numThreads, left;
	for (left = 0; left <= listSize - size; left += size)
		threadPool.push_back(thread(insertionSort, left, left+size-1));
	
	// clear threadPool
	for (unsigned int i = 0; i < threadPool.size(); i++)
		threadPool[i].join();
	threadPool.clear();



	// merge adjacent pairs of sublists using mergeLists function
	for (left = 0; left <= listSize - (size * 2); left += size * 2)
		threadPool.push_back(thread(mergeLists, left, left + size - 1, left + size, left + (size * 2) - 1));

	// clear threadPool
	for (unsigned int i = 0; i < threadPool.size(); i++)
		threadPool[i].join();
	threadPool.clear();



	cout << endl << endl;
	for (int i = 0; i < listSize; i++)
		cout << _list[i] << endl;

	return 0;
}


void insertionSort(int left, int right) {
	int i, key, j;

	for (i = left; i <= right; i++) {

		key = _list[i];
		j = i - 1;

		// move ints that are > j one position forward
		while (j >= left && _list[j] > key) {
			_list[j + 1] = _list[j];
			j = j - 1;
		}
		_list[j + 1] = key;

	}
}

void mergeLists(int leftLeft, int leftRight, int rightLeft, int rightRight) {
	vector<int> sortedList;
	int beg = leftLeft, end = rightRight;

	// compare the two sublist leftmost values until one sublist is done
	while (leftLeft <= leftRight && rightLeft <= rightRight) {
		if (_list[leftLeft] < _list[rightLeft]) {
			sortedList.push_back(_list[leftLeft]);
			leftLeft++;
		}
		else {
			sortedList.push_back(_list[rightLeft]);
			rightLeft++;
		}
	}

	// insert the rest of the remaining sublists if possible
	while (leftLeft <= leftRight) {
		sortedList.push_back(_list[leftLeft]);
		leftLeft++;
	}
	while (rightLeft <= rightRight) {
		sortedList.push_back(_list[rightLeft]);
		rightLeft++;
	}

	// replace the new sorted list into _list
	for (int i = 0; beg <= end; i++) {
		_list[beg] = sortedList[i];
		beg++;
	}
}