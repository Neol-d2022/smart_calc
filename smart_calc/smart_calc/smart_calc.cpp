// smart_calc.cpp : 定義主控台應用程式的進入點。
//

#include "stdafx.h"
#include <cstdio>
#include <cstdlib>
#include "stack.h"

#define MAX_STACK_SIZE	64

int isN(char c);
int isO(char c);
int isClosureO(char c);
void nextChr(char *str, unsigned char *i, unsigned char l);
char p(char c);
void errMsg(char *b, unsigned char c, char e);
void errFull();
double _calc(double num1, double num2, char optr);
double calc(char *buf, unsigned char *i, unsigned char l, int *t);

int main(int argc, char** argv) {
	char buf[256];
	fgets(buf, sizeof(buf), stdin);
	unsigned char c = 0;
	unsigned char l = 0;
	while (buf[l] != '\0') l++;
	l--;
	int t = 0;
	double r = calc(buf, &c, l, &t);
	if (t){
		printf("\"()\"s are not in pairs.\n");
		return 1;
	}
	printf("%f\n", r);
	return 0;
}

inline int isN(char c){
	if (c >= '0' && c <= '9') return 1;
	if (c == '.') return 1;
	return 0;
}

inline int isO(char c){
	if (c == '+') return 1;
	if (c == '-') return 1;
	if (c == '*') return 1;
	if (c == '/') return 1;
	return 0;
}

inline int isClosureO(char c){
	if (c == ')') return 1;
	if (c == '(') return 1;
	return 0;
}

inline void nextChr(char *str, unsigned char *i, unsigned char l){
	while (str[(*i)] == ' ' && (*i) < l) (*i)++;
}

inline void nextSpc(char *str, unsigned char *i, unsigned char l){
	while (str[(*i)] != ' ' && (*i) < l) (*i)++;
}

inline char p(char c){
	if (c == '+' || c == '-') return 1;
	if (c == '*' || c == '/') return 2;
	return -1;
}

inline void errMsg(char *b, unsigned char c, char e){
	char out[256];
	for (unsigned char i = 0; i <= c; i++)
		out[i] = b[i];
	if ((int)c + 1 < 256) out[c + 1] = '\0';
	printf("%s<- expect %s\n", out, (e) ? "an operator" : "a number");
	return;
}

inline void errFull(){
	printf("[ERROR] Too many commands.\n");
}

inline double _calc(double num1, double num2, char optr){
	if (optr == '+') return (num1 + num2);
	if (optr == '-') return (num1 - num2);
	if (optr == '*') return (num1 * num2);
	if (optr == '/') return (num1 / num2);
	return 0;
}

double calc(char *buf, unsigned char *i, unsigned char l, int *t){
	Stack_n n(MAX_STACK_SIZE); //number
	Stack o(MAX_STACK_SIZE); //operator
	//Stack s(MAX_STACK_SIZE); //closure
	char expected = 0;
	char last_ch = 0x0;
	char last_p = 0;
	unsigned char c = *i;
	for (; c < l; c++){
		nextChr(buf, &c, l);
		if (buf[c] == '\0' || buf[c] == 0xa) break;
		if (isN(buf[c])){
			if (expected == 0) expected = 1;
			else{
				errMsg(buf, c, expected);
				exit(1);
			}
			if (n.isFull()) {
				errFull();
				exit(1);
			}
			n.push(atof(buf + c));
			while (isN(buf[c])) c++;
			c--;
		}
		else if (isO(buf[c])){
			if (expected == 1) expected = 0;
			else{
				errMsg(buf, c, expected);
				exit(1);
			}
			if (o.isFull()) {
				errFull();
				exit(1);
			}
			if (!o.isEmpty()){
				if (last_p > p(buf[c])) {
					double n1, n2;
					char optr;
					unsigned char lc = 0;
					Stack to(MAX_STACK_SIZE);
					while (!o.isEmpty()){
						optr = (char)o.pop();
						if (p(optr) != last_p)if (p(optr) != last_p){
							o.push(optr);
							break;
						}
						to.push((char)optr);
						lc++;
					}
					Stack_n tn(MAX_STACK_SIZE);
					for (unsigned char i = 0; i <= lc; i++)
						tn.push(n.pop());
					while (!to.isEmpty()){
						n1 = tn.pop();
						n2 = tn.pop();
						tn.push(_calc(n1, n2, (char)to.pop()));
					}
					n.push(tn.pop());
				}
			}
			o.push((int)buf[c]);
			last_p = p(buf[c]);
		}
		else if (buf[c] == '('){
			if (expected == 0) expected = 1;
			else{
				errMsg(buf, c, expected);
				exit(1);
			}
			/*if(s.isFull()) {
			errFull();
			return 1;
			}*/
			c++;
			int t = 0;
			n.push(calc(buf, &c, l, &t));
			if (!t){
				printf("\"()\"s are not in pairs.\n");
				exit(1);
			}
		}
		else if (buf[c] == ')'){
			if (expected == 1);
			else{
				errMsg(buf, c, expected);
				exit(1);
			}
			double n1, n2;
			char optr;
			unsigned char lc;
			Stack to(MAX_STACK_SIZE);
			Stack_n tn(MAX_STACK_SIZE);
			while (last_p != 0){
				to.clear();
				tn.clear();
				lc = 0;
				while (!o.isEmpty()){
					optr = (char)o.pop();
					if (p(optr) != last_p){
						o.push((char)optr);
						break;
					}
					to.push((char)optr);
					lc++;
				}
				if (lc == 0) {
					last_p--;
					continue;
				}
				for (unsigned char i = 0; i <= lc; i++)
					tn.push(n.pop());
				while (!to.isEmpty()){
					n1 = tn.pop();
					n2 = tn.pop();
					tn.push(_calc(n1, n2, (char)to.pop()));
				}
				n.push(tn.pop());
				last_p--;
			}
			*i = (++c);
			*t = 1;
			return n.pop();
		}
		else{
			errMsg(buf, c, expected);
			exit(1);
		}
	}
	if (expected == 1);
	else{
		errMsg(buf, c, expected);
		exit(1);
	}
	double n1, n2;
	char optr;
	unsigned char lc;
	Stack to(MAX_STACK_SIZE);
	Stack_n tn(MAX_STACK_SIZE);
	while (last_p != 0){
		to.clear();
		tn.clear();
		lc = 0;
		while (!o.isEmpty()){
			optr = (char)o.pop();
			if (p(optr) != last_p){
				o.push((char)optr);
				break;
			}
			to.push((char)optr);
			lc++;
		}
		if (lc == 0) {
			last_p--;
			continue;
		}
		for (unsigned char i = 0; i <= lc; i++)
			tn.push(n.pop());
		while (!to.isEmpty()){
			n1 = tn.pop();
			n2 = tn.pop();
			tn.push(_calc(n1, n2, (char)to.pop()));
		}
		n.push(tn.pop());
		last_p--;
	}
	*i = c;
	return n.pop();
}