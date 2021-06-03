#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#include <stdio.h>

#include <windows.h>

#define COUNT 10

void manual_alloc() {
  for (size_t i = 0; i < COUNT; ++i) {
    int * pInt = (int*)malloc(sizeof(int));
    //free(pInt);
  }
}

void manual_heap() {
  for (size_t i = 0; i < COUNT; ++i) {
    auto PFNArraySize = 512;
    ULONG_PTR * aPFNs = (ULONG_PTR *)HeapAlloc(GetProcessHeap(), 0, PFNArraySize);
  }
}

void manual_virtual() {
  LPVOID pBlock = VirtualAlloc(NULL, 1024 * 1024 * 1024, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
  int sz = 1024 * 1024 * 1024 / sizeof(int);
  int* pInt = (int*)pBlock;
  for (int i = 0; i < sz; ++i) {
    pInt[i] = i;
  }
  //pInt[1024] = 10;
  //pInt[1024 * 1024] = 200;
  //int k = 0;
}

void TestMemory() {
  int start_flag = 0;
  scanf_s("%d", &start_flag);

  manual_alloc();

  manual_heap();

  manual_virtual();

  int finish_flag = 0;
  scanf_s("%d", &finish_flag);

  _CrtDumpMemoryLeaks();
}
