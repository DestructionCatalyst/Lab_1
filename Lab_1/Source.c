#include <stdio.h>
#include <locale.h>

struct Array
{
	void* buffer;
	int count;
	int element_size;
};

void set_element(struct Array* a, int index, void* value) {
	void* p = (void*)((char*)a->buffer + a->element_size * index);
	memcpy(p, value, a->element_size);
}

struct Array* Create(int count, int element_size, void* default_el)
{
	struct Array* a = malloc(sizeof(struct Array));
	a->count = count;
	a->element_size = element_size;
	a->buffer = malloc(count * element_size);
	for (int i = 0;i < count;i++) set_element(a, i, default_el);
	return a;
}



struct Array* CreateInt(int count) {
	int zero = 0;
	return Create(count, sizeof(int), &zero);
}

struct Array* CreateIntegerLinearForm(int length, int* values) {
	int t = 0;
	struct Array* form = Create(length, sizeof(int), &t);
	for (int i = 0; i < form->count; i++) {
		set_element(form, i, values+i);
	}
	return form;
}

void PrintLinearForm(struct Array* form) {
	for (int i = 0; i < form->count; i++) {
		if (((int*)form->buffer)[i]) {
			if (i == 0)
				printf("%d", ((int*)form->buffer)[i]);
			else printf("%d*x%d", ((int*)form->buffer)[i], i);
			if (i != form->count - 1)printf(" + ");
		}
	}
	
}

int main() {
	char* locale = setlocale(LC_ALL, "");

	int len = 0;
	int* values;
	printf("¬ведите количество переменных\n");
	do {
		scanf("%d", &len);
	} while (len < 1);
	len++;
	values = (int*)malloc(len * sizeof(int));
	printf("¬ведите коэффициенты (через пробел)\n");
	for (int i = 0; i < len; i++) {
		scanf("%d", values+i);
	}
	printf("\n");
	struct Array* a = CreateIntegerLinearForm(len, values);
	PrintLinearForm(a);
	return 0;
}