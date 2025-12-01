#include "pch.h"
#include "framework.h"
#include <iostream>
#include <Windows.h>
#pragma warning(disable: 4996)

extern "C" {
	int __stdcall getmin(int* arr, int size) {
		int min = 10000000;
		for (int i = 0; i < size; i++) {
			if (arr[i] < min) {
				min = arr[i];
			}
		}
		return min;
	}

	int __stdcall getmax(int* arr, int size) {
		int max = 0;
		for (int i = 0; i < size; i++) {
			if (arr[i] > max) {
				max = arr[i];
			}
		}
		return max;
	}

}
