#include <iostream>
#include <stdlib.h>

const int START_SIZE = 10;
const int UNDEFINED = -1;
const int POISON = 12324567890;
const int DAMAGED_STACK = -888;

typedef struct {
	int max_size;
	int size;
	int* element;
	int error;
} Stack;

bool is_damaged(Stack* st) {
	if (st->error == DAMAGED_STACK) {
		printf("Stack is damaged. You can't access it. Please use other stack\n");
		return true;
	}
	return false;
}

void create(Stack* st) {
	st->max_size = START_SIZE;
	st->size = 0;
	st->element = (int*)calloc(st->max_size, sizeof(int));
	st->error = 0;
}

void destruct(Stack* st) {
	for (int i = 0; i < st->max_size; ++i)
		st->element[i] = POISON;
	st->max_size = UNDEFINED;
	st->size = UNDEFINED;
	st->error = DAMAGED_STACK;
	//free(st->element);
}

int push(Stack* st, int value) {
	if (is_damaged(st))
		return st->error;
	if (st->size == st->max_size) {
		st->max_size *= 2;
		st->element = (int*)realloc(st->element, sizeof(int) * st->max_size);
	}
	st->element[st->size] = value;
	st->size++;
	return 0;
}

int pop(Stack* st) {
	if (is_damaged(st))
		return DAMAGED_STACK;
	if (st->size == 0) {
		printf("Error. Stack is empty. There is no elements.\n");
		st->error = DAMAGED_STACK;
		return DAMAGED_STACK;
	}
	int value = st->element[--st->size];
	if (st->max_size > START_SIZE && st->max_size / st->size >= 4) {
		st->max_size /= 2;
		st->element = (int*)realloc(st->element, sizeof(int) * st->max_size / 2);
	}
	return value;
}

void clear(Stack* st) {
	if (is_damaged(st))
		return;
	st->max_size = START_SIZE;
	st->size = 0;
	st->element = NULL;
	st->element = (int*)realloc(st->element, st->max_size * sizeof(char));
}

int size(Stack* st) {
	return st->size;
}

void test_1() {
	Stack st;
	create(&st);
	if (DAMAGED_STACK != pop(&st)) {
		printf("Mistake in test_1. You cannot pop from empty stack\n");
		return;
	}
	printf("Test_1 is ok\n");
	destruct(&st);
}

void test_2() {
	Stack st;
	create(&st);
	for (int i = 0; i < 15; ++i) {
		if (DAMAGED_STACK == push(&st, i)) {
			printf("Mistake in test_2. Wrong push\n");
			return;
		}
		if (size(&st) != i + 1) {
			printf("Mistake in test_2. Wrong size\n");
			return;
		}
	}
	for (int i = 0; i < 5; ++i) {
		if (pop(&st) != 14 - i) {
			printf("Mistake in test_2. Wrong pop\n");
		}
	}
	if (st.max_size != 20) {
		printf("Mistake in test_3. Wrong capacity (max size)");
		return;
	}
	printf("Test_2 is ok\n");
	destruct(&st);
}

void test_3() {
	Stack st;
	create(&st);
	for (int i = 0; i < 50; ++i) {
		push(&st, i * 50);
	}
	clear(&st);
	if (size(&st) != 0) {
		printf("Mistake in test_3. Size must equal zero after clear function/\n");
		return;
	}
	if (st.max_size != 10) {
		printf("Mistake in test_3. Max size (capacity) must equal zero after clear function/\n");
		return;
	}
	if (DAMAGED_STACK != pop(&st)) {
		printf("Mistake in test_3. Stack must be empty after clear function\n");
		return;
	}
	printf("Test_3 is ok\n");
	destruct(&st);
}

void unit_test() {
	test_1();
	test_2();
	test_3();
}


int main() {
	unit_test();
	return 0;
}