#include <stdio.h>
#include <string.h>
#include <locale.h>
#define FormType(a) a->isFloat?*double:*int

struct Array
{
	void* buffer;
	int count;
	int element_size;
	int isFloat;
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
	int zero = 0;
	struct Array* form = Create(length, sizeof(int), &zero);
	for (int i = 0; i < form->count; i++) {
		set_element(form, i, values + i);
	}
	form->isFloat = 0;
	return form;
}

struct Array* CreateFloatLinearForm(int length, double* values) {
	int zero = 0;
	struct Array* form = Create(length, sizeof(double), &zero);
	for (int i = 0; i < form->count; i++) {
		set_element(form, i, values + i);
	}
	form->isFloat = 1;
	return form;
}

void PrintLinearForm(struct Array* form) {


	if (form->isFloat) {
		for (int i = 0; i < form->count; i++) {
			if (((double*)form->buffer)[i]) {
				if (i == 0)
					printf("%lg", ((double*)form->buffer)[i]);
				else printf("%lg*x%d", ((double*)form->buffer)[i], i);
				if (i != form->count - 1)printf(" + ");
			}
		}
	}
	else {
		for (int i = 0; i < form->count; i++) {
			if (((int*)form->buffer)[i]) {
				
				if (i == 0)
					printf("%d", ((int*)form->buffer)[i]);
				else printf("%d*x%d", ((int*)form->buffer)[i], i);
				if (i != form->count - 1)printf(" + ");
			}
		}
	}
	
}

void MultiplyBy(struct Array* form, double number) {
	
}

int main() {
	char* locale = setlocale(LC_ALL, "");

	int len = 0;
	char* command = malloc(10*sizeof(char));

	printf("Введите команду\n");
	scanf("%s %d", command, &len);

	len++;

	
	if (strcmp(command, "IntForm") == 0) {
		int* values;
		values = (int*)malloc(len * sizeof(int));
		printf("Введите коэффициенты (через пробел)\n");
		for (int i = 0; i < len; i++) {
			scanf("%d", values + i);
		}
		printf("\n");
		struct Array* a = CreateIntegerLinearForm(len, values);
		PrintLinearForm(a);
	}

	if (strcmp(command, "FloatForm") == 0) {
		double* values;
		values = (double*)malloc(len * sizeof(double));
		printf("Введите коэффициенты (через пробел)\n");
		for (int i = 0; i < len; i++) {
			scanf("%lf", values + i);
		}
		printf("\n");
		struct Array* a = CreateFloatLinearForm(len, values);
		PrintLinearForm(a);
	}
	return 0;
}