#include <cstdio>
#include <cstdlib>
#include <vector>
#include <algorithm>
#include <utility>
#include <iostream>
#include <ctime>
#include <random>


using namespace std;


const int minBlockSize = 128;
const int req = 7;

pair<int*, int> MergeAndCountSplitInvPair(int* Lb, int Lsize, int* Rb, int Rsize);
pair <int*, int> SortAndCountInvPair(int* begin, int* end);

int* MergeAndCountSplitInv(int* Lb, int Lsize, int* Rb, int Rsize);
int* SortAndCountInv(int* begin, int* end);

int* MergeAndCountSplitInvFor(int* Lb, int Lsize, int* Rb, int Rsize);
int* SortAndCountInvFor(int* begin, int* end);

int mergeAndCount(int* b, int* m, int* e, int* tmp);
int insertionSortAndCount(int* b, int* e);
int mergeSortAndCount(int* b, int* e);

void test(int*, int);

template <class T>
void insertionSort(T* b, T* e) {
	const int size = e - b;

	for (int i = 1; i < size; i++) {
		int pos = i;
		while (pos && b[pos] < b[pos - 1]) {
			std::swap(b[pos], b[pos--]);
		}
	}
}


template <class T>
void quickSort(T* b, T* e) {
	if (e - b < minBlockSize) {
		insertionSort<T>(b, e);
		return;
	}

	const int size = e - b;
	pair <T, int> arr[req];
	for (int i = 0; i < req; i++) {
		arr[i].second = rand() % size;
		arr[i].first = b[arr[i].second];
	}
	insertionSort< pair <T, int> >(arr, arr + req);

	swap(*(e - 1), b[arr[req >> 1].second]);
	T* wall = b;
	const T* pivot = e - 1;

	for (T* i = b; i != pivot; i++) {
		if (*i < *pivot) {
			swap(*i, *wall);
			++wall;
		}
	}

	swap(*(e - 1), *wall);
	quickSort(b, wall);
	quickSort(wall + 1, e);
}


int main(int argc, char* argv[]) {
	cout << "Enter Counter of Array: ";
	int n;
	cin >> n;
	random_device rd;
	mt19937 gen(rd());
	uniform_int_distribution<int> dist(INT_MIN + 1, INT_MAX - 1);
	int* A = new int[n];
	for (int i = 0; i < n; i++)
	{
		A[i] = dist(gen);
	}
	test(A, n);
	delete[] A;
	system("pause");
}


pair<int*, int> MergeAndCountSplitInvPair(int* Lb, int Lsize, int* Rb, int Rsize)
{
	int* L = new int[Lsize + 1]; // Local copy of Left Array
	int* R = new int[Rsize + 1]; // Local copy of Right Array

	memcpy(L, Lb, Lsize * sizeof(int));
	memcpy(R, Rb, Rsize * sizeof(int));

	L[Lsize] = INT_MAX;
	R[Rsize] = INT_MAX;

	int counter = 0; // counter of inversions
	int i = 0, j = 0;

	int* A = new int[Lsize + Rsize]; // Merged Array

	for (int k = 0; k < Lsize + Rsize; k++)
	{
		if (L[i] <= R[j])
		{
			A[k] = L[i];
			i++;
		}
		else
		{
			A[k] = R[j];
			j++;
			counter += (Lsize - i);
		}
	}

	delete[] L;
	delete[] R;

	return make_pair(A, counter);
}

// Sorting and find counter of Inversions
pair <int*, int> SortAndCountInvPair(int* begin, int* end)
{
	int counter = 0; // counter of inversions
	int size = end - begin; // size of array

	if (size == 1)
	{
		int* A = new int[1];
		memcpy(A, begin, sizeof(int));

		return make_pair(A, 0);
	}

	else
	{
		int size1 = (size / 2) + (size % 2); // size of Left Array
		int size2 = (size / 2); // size of Right Array

		int* Lb = new int[size1]; // Left Array
		int* Rb = new int[size2]; // Right Array

		memcpy(Lb, begin, size1 * sizeof(int));
		memcpy(Rb, begin + (size / 2) + (size % 2), size2 * sizeof(int));

		pair <int*, int> L; // Sorted Left Array and counter of inversions
		pair <int*, int> R; // Sorted Right Array and counter of inversions
		pair <int*, int> A; // Sorted Array and counter of inversions

		L = SortAndCountInvPair(Lb, Lb + size1); // Sorting of Left Array
		R = SortAndCountInvPair(Rb, Rb + size2); // Sorting of Right Array

		delete[] Lb;
		delete[] Rb;

		A = MergeAndCountSplitInvPair(L.first, size1, R.first, size2); // Merging of Left and Right Array
		A.second += L.second + R.second;

		delete[] L.first;
		delete[] R.first;

		return A;
	}
}


int mergeAndCount(int* b, int* m, int* e, int* tmp) {
	int counter = 0; // Count of inversion
	int* p1 = tmp; // Pointer for 1st array
	int* p2 = m; // Pointer for 2nd array
	int* cp = b; // Current pointet
	int* eTmp = tmp + (m - b); // End of tmp array

	memcpy(tmp, b, (m - b) * sizeof(int));
	while (p1 != eTmp && p2 != e) {
		if (*p1 <= *p2) {
			*cp = *p1;
			p1++;
		}
		else {
			*cp = *p2;
			counter += (eTmp - p1);
			p2++;
		}
		cp++;
	}

	while (p1 != eTmp) {
		*cp = *p1;
		cp++;
		p1++;
	}

	while (p2 != e) {
		*cp = *p2;
		cp++;
		p2++;
	}

	return counter;
}

// Insertion sort
int insertionSortAndCount(int* b, int* e) {
	int counter = 0;
	int size = e - b;

	for (int i = 1; i < size; i++) {
		int pos = i;
		while (pos && b[pos] < b[pos - 1]) {
			std::swap(b[pos], b[pos - 1]);
			pos--;
			counter++;
		}
	}

	return counter;
}

// Merge sort
int mergeSortAndCount(int* b, int* e) {
	int* tmp = new int[e - b]; // temporary array for merge
	int counter = 0;
	int size = e - b;

	int blockSize = 64;
	for (int i = 0; i < size; i += blockSize) {
		counter += insertionSortAndCount(b + i, b + min(i + blockSize, size));
	}

	while (blockSize < size) {
		for (int i = 0; i + blockSize < size; i += blockSize * 2) {
			counter += mergeAndCount(b + i, b + i + blockSize, b + min(i + 2 * blockSize, size), tmp);
		}
		blockSize <<= 1;
	}

	delete[] tmp;
	return counter;
}

// Function Merge Sorting
int* MergeAndCountSplitInv(int* Lb, int Lsize, int* Rb, int Rsize)
{
	int* L = new int[Lsize + 1]; // Local copy of Left Array
	int* R = new int[Rsize + 1]; // Local copy of Right Array

	memcpy(L, Lb, Lsize * sizeof(int));
	memcpy(R, Rb, Rsize * sizeof(int));

	L[Lsize] = INT_MAX;
	R[Rsize] = INT_MAX;

	int i = 0, j = 0;

	int* A = new int[Lsize + Rsize]; // Merged Array

	for (int k = 0; k < Lsize + Rsize; k++)
	{
		if (L[i] <= R[j])
		{
			A[k] = L[i];
			i++;
		}
		else
		{
			A[k] = R[j];
			j++;
		}
	}

	delete[] L;
	delete[] R;

	return A;
}

// Sorting and find counter of Inversions
int* SortAndCountInv(int* begin, int* end)
{
	int counter = 0; // counter of inversions
	int size = end - begin; // size of array

	if (size == 1)
	{
		int* A = new int[1];
		memcpy(A, begin, sizeof(int));

		return A;
	}

	else
	{
		int size1 = (size / 2) + (size % 2); // size of Left Array
		int size2 = (size / 2); // size of Right Array

		int* Lb = new int[size1]; // Left Array
		int* Rb = new int[size2]; // Right Array

		memcpy(Lb, begin, size1 * sizeof(int));
		memcpy(Rb, begin + (size / 2) + (size % 2), size2 * sizeof(int));

		int* L; // Sorted Left Array and counter of inversions
		int* R; // Sorted Right Array and counter of inversions
		int* A; // Sorted Array and counter of inversions

		L = SortAndCountInv(Lb, Lb + size1); // Sorting of Left Array
		R = SortAndCountInv(Rb, Rb + size2); // Sorting of Right Array

		delete[] Lb;
		delete[] Rb;

		A = MergeAndCountSplitInv(L, size1, R, size2); // Merging of Left and Right Array

		delete[] L;
		delete[] R;

		return A;
	}
}

int* MergeAndCountSplitInvFor(int* Lb, int Lsize, int* Rb, int Rsize)
{
	int* L = new int[Lsize + 1]; // Local copy of Left Array
	int* R = new int[Rsize + 1]; // Local copy of Right Array

	for (int i = 0; i < Lsize; i++)
	{
		L[i] = Lb[i];
	}

	for (int i = 0; i < Rsize; i++)
	{
		R[i] = Rb[i];
	}
	

	L[Lsize] = INT_MAX;
	R[Rsize] = INT_MAX;

	int i = 0, j = 0;

	int* A = new int[Lsize + Rsize]; // Merged Array

	for (int k = 0; k < Lsize + Rsize; k++)
	{
		if (L[i] <= R[j])
		{
			A[k] = L[i];
			i++;
		}
		else
		{
			A[k] = R[j];
			j++;
		}
	}

	delete[] L;
	delete[] R;

	return A;
}

// Sorting and find counter of Inversions
int* SortAndCountInvFor(int* begin, int* end)
{
	int counter = 0; // counter of inversions
	int size = end - begin; // size of array

	if (size == 1)
	{
		int* A = new int[1];
		memcpy(A, begin, sizeof(int));

		return A;
	}

	else
	{
		int size1 = (size / 2) + (size % 2); // size of Left Array
		int size2 = (size / 2); // size of Right Array

		int* Lb = new int[size1]; // Left Array
		int* Rb = new int[size2]; // Right Array

		for (int i = 0; i < size1; i++)
		{
			Lb[i] = begin[i];
		}

		for (int i = 0; i < size2; i++)
		{
			Rb[i] = begin[i + (size / 2) + (size % 2)];
		}

		int* L; // Sorted Left Array and counter of inversions
		int* R; // Sorted Right Array and counter of inversions
		int* A; // Sorted Array and counter of inversions

		L = SortAndCountInvFor(Lb, Lb + size1); // Sorting of Left Array
		R = SortAndCountInvFor(Rb, Rb + size2); // Sorting of Right Array

		delete[] Lb;
		delete[] Rb;

		A = MergeAndCountSplitInvFor(L, size1, R, size2); // Merging of Left and Right Array

		delete[] L;
		delete[] R;

		return A;
	}
}


void test(int* A, int n)
{
	int* B = new int[n];
	memcpy(B, A, n * sizeof(int));

	int* C = new int[n];

	pair<int*, int> D;

	time_t begin = clock();
	C = SortAndCountInv(B, B + n);
	time_t end = clock();

	time_t begin3 = clock();
	D = SortAndCountInvPair(B, B + n);
	time_t end3 = clock();

	time_t begin1 = clock();
	C = SortAndCountInvFor(B, B + n);
	time_t end1 = clock();

	time_t begin2 = clock();
	int count = mergeSortAndCount(B, B + n);
	time_t end2 = clock();

	memcpy(B, A, n * sizeof(int));

	time_t begin4 = clock();
	quickSort<int>(B, B + n);
	time_t end4 = clock();

	cout << "My Merge Sort with memcpy: " << end - begin << " ms" << endl;
	cout << "My Merge Sort with memcpy and pair: " << end3 - begin3 << " ms" << endl;
	cout << "My Merge Sort with for: " << end1 - begin1 << " ms" << endl;
	cout << "Sasha Merge Sort: " << end2 - begin2 << " ms" << endl;
	cout << "Sasha Quick Sort: " << end4 - begin4 << " ms" << endl;
	delete[] B;
	delete[] C;
}

