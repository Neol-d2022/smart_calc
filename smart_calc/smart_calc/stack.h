class Stack { //Store charaters
private:
	int* stack;
	unsigned int cur;
	unsigned int max_size;
public:
	Stack() : stack(0), cur(0), max_size(0) {}
	Stack(unsigned int param) {
		stack = 0;
		cur = 0;
		max_size = 0;
		if (param > 1) {
			stack = new int[param];
			max_size = param;
		}
	}
	~Stack(){
		if (stack) delete[] stack;
	}
	int isValid() { return (stack) ? 1 : 0; }
	int isEmpty() { return (stack && cur == 0) ? 1 : 0; }
	int isFull() { return (stack && cur == max_size) ? 1 : 0; }
	int pop() {
		if (!cur) throw "Tried to pop an empty stack.";
		return stack[--cur];
	}
	void push(int param) {
		if (cur == max_size) throw "Tried to push a full stack.";
		stack[cur++] = param;
	}
	void clear() { cur = 0; }
};

class Stack_n { //Store reals
private:
	double* stack;
	unsigned int cur;
	unsigned int max_size;
public:
	Stack_n() : stack(0), cur(0), max_size(0) {}
	Stack_n(unsigned int param) {
		stack = 0;
		cur = 0;
		max_size = 0;
		if (param > 1) {
			stack = new double[param];
			max_size = param;
		}
	}
	~Stack_n(){
		if (stack) delete[] stack;
	}
	int isValid() { return (stack) ? 1 : 0; }
	int isEmpty() { return (stack && cur == 0) ? 1 : 0; }
	int isFull() { return (stack && cur == max_size) ? 1 : 0; }
	double pop() {
		if (!cur) throw "Tried to pop an empty stack.";
		return stack[--cur];
	}
	void push(double param) {
		if (cur == max_size) throw "Tried to push a full stack.";
		stack[cur++] = param;
	}
	void clear() { cur = 0; }
};