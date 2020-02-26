#pragma once
#ifndef DynamicArray
#define DynamicArray
#endif

//Динамический массив укзаателей, в данный момент содержащий count элементов размером el_size
struct DynArray
{
	void** elem;
	int el_size;
	int capacity;
	int count;
};

struct DynArray* CreateDynArray(void* first);

void* getArray(struct DynArray* a, int index);

int getCount(struct DynArray* a);

void Add(struct DynArray* a, void* new);
