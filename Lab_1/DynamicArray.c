//#include "DynamicArray.h"

//������� ���.������ � ������ ��������� first
struct DynArray* CreateDynArray(void* first) {
	struct DynArray* a = malloc(sizeof(struct DynArray));
	a->capacity = 4;
	a->el_size = sizeof(first);
	a->elem = malloc(a->capacity * a->el_size);
	a->elem[0] = first;
	a->count = 1;
	return a;
}

//�������� ��������� � �������� Index �� ������������� ������� a
void* getArray(struct DynArray* a, int index) {
	return (void*)(((char*)a->elem) + index * (a->el_size));
}

int getCount(struct DynArray* a) {
	return a->count;
}
//�������� � ���.������ a ������� new
void Add(struct DynArray* a, void* new) {
	if (a->el_size == sizeof(new)) {
		if (a->capacity <= a->count) {
			a->capacity = a->capacity * 2;
			a->elem = realloc(a->elem, a->capacity * a->el_size);
		}
		a->elem[a->count] = new;
		a->count++;
	};
}

