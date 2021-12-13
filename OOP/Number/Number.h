#include <iostream>
#include <iomanip>
#include <vector>
#include <algorithm>
#include <cstring>
#include <string>
#include <math.h>

struct Number {
private:
	static const int base = 100;
	size_t size;
	size_t capacity;
	char* data;

public:
	Number(int a);
    Number();
    Number(const Number& n);
    Number(char* str);
    bool comp(Number two);
    Number& operator=(const Number& right);
    Number operator+(const Number& right);
	~Number();
	friend std::ostream& operator<<(std::ostream& stream, const Number& right);
	bool isEven();
	Number mult_by_int(int x);
    void divide_by_two();
	Number operator*(Number right) const;
};
