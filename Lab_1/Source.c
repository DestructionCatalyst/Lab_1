#include <stdio.h>
#include <string.h>
#include <locale.h>

struct Form
{
	void* buffer;
	int count;
	int element_size;
};

struct DynArray
{
	void** elem;
	int el_size;
	int capacity;
	int count;
};

//Установить элемент Form a с индексом index на значение value
void set_element(struct Form* a, int index, void* value) {
	void* p = (void*)((char*)a->buffer + a->element_size * index);
	memcpy(p, value, a->element_size);
}

void* get_element(struct Form* a, int index) {
	return (void*)(((char*)a->buffer)+index*(a->element_size));
}


//Cоздать форму из count элементов размером element_size и заполнить значением default_el
struct Form* Create(int count, int element_size, void* default_el)
{
	struct Form* a = malloc(sizeof(struct Form));
	a->count = count;
	a->element_size = element_size;
	a->buffer = malloc(count * element_size);
	for (int i = 0;i < count;i++) set_element(a, i, default_el);
	return a;
}


//Cоздать лин.форму из чисел values формата double количеством length
struct Form* CreateFloatLinearForm(int length, double* values) {
	int zero = 0;
	struct Form* form = Create(length, sizeof(double), &zero);
	for (int i = 0; i < form->count; i++) {
		set_element(form, i, values + i);
	}
	return form;
}

double* get(struct Form* a, int index) {
	return (double*)get_element(a, index);
}

//Вывести форму form
void PrintLinearForm(struct Form* form) {

	for (int i = 0; i < form->count; i++) {
		if (((double*)form->buffer)[i]) {
			if (i == 0)
				printf("%lg", *get(form,i));
			else printf("%lg*x%d", *get(form, i), i);
			if (i != form->count - 1)printf(" + ");
		}
	}

	printf("\n");
	
}

//Создать дин.массив с первым элементом first
struct DynArray* CreateDynArray(void* first) {
	struct DynArray* a = malloc(sizeof(struct DynArray));
	a->capacity = 4;
	a->el_size = sizeof(first);
	a->elem = malloc(a->capacity * a->el_size);
	a->elem[0] = first;
	a->count = 1;
	return a;
}

void* getArrayElement(struct DynArray* a, int index) {
	return (void*)(((char*)a->elem) + index * (a->el_size));
}

struct Form* getForm(struct DynArray* a, int index) {
	return (struct Form*)getArrayElement(a, index);
}

//Добавить в дин.массив a элемент new
void Add(struct DynArray* a, void* new) {
	if (a->el_size == sizeof(new)) {
		if (a->capacity <= a->count) {
			a->capacity = a->capacity * 2;
			a->elem = realloc(a->elem, a->capacity * a->el_size);
		}
		a->elem[a->count] = new;
		printf("%d. ", a->count);
		PrintLinearForm(a->elem[a->count]);
		a->count++;
	};
}

//Напечатать лин. формы из дин.массива a
void PrintForms(struct DynArray* a) {
	for (int i = 1; i < a->count; i++) {
		printf("%d. ", i);
		PrintLinearForm(getForm(a, i));
	}
}

//Сложить линейные формы a и b
struct Form* Sum(struct Form* a, struct Form* b) {
	struct Form* c;
	if (a->count < b->count) {
		c = a;
		a = b;
		b = c;
	}
	c = CreateFloatLinearForm(a->count, a->buffer);
	for (int i = 0; i < b->count; i++) {
		*get(c, i) += *get(b, i);
	}
	return c;
}

//Умножить линейную форму a на число num
struct Form* Multiply(struct Form* a, double num) {
	struct Form* c = CreateFloatLinearForm(a->count, a->buffer);
	for (int i = 0; i < a->count; i++) {
		*get(c, i) = *get(c, i) * num;
	}
	return c;
}

double Calculate(struct Form* a, double* args) {
	double rez = *get(a, 0);
	for (int i = 1; i < a->count; i++) {
		rez += *get(a, i) * args[i-1];
	}
	return rez;
}

int main() {
	char* locale = setlocale(LC_ALL, "");

	//int arg2 = 0;
	char* command = malloc(10*sizeof(char));

	double* sample = malloc(sizeof(double));
	sample[0] = 0;
	struct Form* a = CreateFloatLinearForm(1, sample);
	struct DynArray* arr = CreateDynArray(a);
	PrintForms(arr);

	while (1) {
		printf("Введите команду\n");
		scanf("%s", command);


		if (strcmp(command, "create") == 0) {
			int len = 0;
			scanf("%d", &len);
			len++;//Второй аргумент - длина формы
			double* values;
			values = (double*)malloc(len * sizeof(double));
			printf("Введите коэффициенты (через пробел)\n");
			for (int i = 0; i < len; i++) {
				scanf("%lf", values + i);
			}
			printf("\n");
			struct Form* a = CreateFloatLinearForm(len, values);
			Add(arr, a);
		}
		else if (strcmp(command, "print") == 0) {
			//2 аргумент - номер выводимого многочлена, если 0 - все сразу
			int n = 0;
			scanf("%d", n);
			if (n <= 0) PrintForms(arr);
			else PrintLinearForm(arr->elem[n]);
		}
		else if (strcmp(command, "sum") == 0) {
			int arg2 = 0,arg3 = 0;//Номера слагаемых
			scanf("%d %d", &arg2, &arg3);
			Add(arr, Sum(arr->elem[arg2], arr->elem[arg3]));
		}
		else if (strcmp(command, "multiply") == 0) {
			int arg2 = 0;
			double arg3 = 0;
			scanf("%d %lf", &arg2, &arg3);
			Add(arr, Multiply(arr->elem[arg2], arg3));
		}
		else if (strcmp(command, "calculate") == 0) {
			int n = 0;
			scanf("%d", &n);//Второй аргумент - номер многочлена
			double* values;
			struct Form* target = (struct Form*)arr->elem[n];
			int count = target->count - 1;
			values = (double*)malloc(count * sizeof(double));
			char c1;
			char* one = "ой:";
			char* many = "ых (через пробел):";
			char* selected;
			if (count == 1) {
				c1 = 'е';
				selected = one;
			}
			else {
				if (count < 5) c1 = 'я';
				else c1 = 'й';
				selected = many;
			}
			printf("Введите %d значени%c переменн%s\n", count,c1, selected);
			for (int i = 0; i < count; i++) {
				scanf("%lf", values + i);
			}
			printf("%lg\n", Calculate(target, values));
		}
		else if ((strcmp(command, "exit") && strcmp(command, "quit")) == 0) {
			return 0;
		}

	}
	return 0;
}