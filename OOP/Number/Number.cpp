#include "number.h"
	Number::Number(int a) {
		int temp = a;
		capacity = 0;
		while (temp != 0) {
			temp /= base;
			capacity += 1;
		}
		if (capacity == 0) {
			capacity = 1;
		}
		data = new char[capacity];
		for (int i = 0; i < capacity; ++i) {
			data[i] = a % base;
			a /= base;
		}
		size = capacity;
	}

    Number::Number(){
        capacity = 1;
        data = new char[capacity];
        data[0] = 0;
        size = capacity;
    }

    Number::Number(const Number& n){
        size = n.size;
        capacity = n.capacity;
        data = new char[capacity];
        for (int i = 0; i < capacity; ++i)
            data[i] = n.data[i];
    }

    Number::Number(char* str){
        capacity = strlen(str) / 2 ;
        if (strlen(str) % 2 == 1)
            capacity++;
        size = capacity;
        data = new char[capacity];
        int j = strlen(str) - 1;
        for (int i = 0; i< capacity - 1; i++){
            data[i] = (str[j-1] - '0')*10 + (str[j]-'0');
            j -= 2;
        }
        if (strlen(str) % 2 == 1){
            data[capacity-1] = str[j] - '0'+0;
        }
        else
            data[capacity-1] = (str[j-1] - '0')*10 + (str[j]-'0');
    }

    bool  Number::comp(Number two){
        if (size > two.size)
            return true;
        if (size < two.size)
            return false;
        if (data[size - 1] > two.data[size-1])
            return true;
        return false;
    }

    Number& Number::operator=(const Number& right){
        size = right.size;
        capacity = right.capacity;
        data = new char[capacity];
        for (int i = 0; i < capacity; ++i)
            data[i] = right.data[i];
    }

    Number Number::operator+(const Number& right){
        Number y;
        size_t t = std::max(size, right.size);
        int ost = 0;
        y.capacity = t+1;
        y.size = t;
        y.data = new char[capacity];
        for(int i = 0; i < t; i++){
            int a = data[i] + right.data[i]+ost;
            ost = 0;
            if (a/100 >0)
                ost = a/100;
            y.data[i] = a%100;
        }
        if (ost > 0){
            y.data[t] = ost;
            y.size++;
        }
        return y;

    }

	Number::~Number() {
		delete [] data;
	}

	bool Number::isEven(){
	if (data[0] %2)
        return false;
    return true;
	}

	Number Number::mult_by_int(int x){
	Number a;
	a.capacity = capacity+1;
	int ost = 0;
	for (int i = 0; i < size; i++){
        (a.data)[i] = (int(data[i]) * x+ost) %100;
        ost = (int(data[i]) * x+ost) /100;
	}
	int i = size -1;
	while (ost>0){
        i++;
        a.data[i] = ost%100;
        ost /= 100;
	}
    a.size = i+1;
    a.capacity = i+1;
	return a;
	}

    void Number::divide_by_two(){
        int ost = 0;
        for(int i = size - 1; i >= 0; i--){
        int t = data[i];
        data[i] = data[i] /2 + ost*50;
        ost = t % 2;
        }
        if (data[size - 1] == 0)
            size--;
    }

	Number Number::operator*(Number right) const{
        Number a;
        for (int i = 0; i< size; i++){
            Number x = right.mult_by_int(int(data[i]));
            for(int j = 0; j < i/5;j++)
                x = x.mult_by_int(pow(100, 5));
            x = x.mult_by_int(pow(100, i%5));
            a = a + x;
        }
        return a;
	}


std::ostream& operator<<(std::ostream& stream, const Number& right) {
	bool fl = true;
	stream << (int)right.data[right.size - 1];
    for (size_t i = 0; i < right.size - 1; ++i){
        if (right.data[i] != 0)
            fl = false;
    }
    if (fl){
        return stream;
    }
	for (size_t i = 0; i < right.size - 1; ++i) {
		stream << std::setfill('0') << std::setw(2) << (int)right.data[right.size - 2 - i];
	}
	return stream;
}

Number fib[1000];
bool used[1000];


Number fibonacci(int n){
    if (used[n])
        return Number(fib[n]);
    fib[n] = fibonacci(n-1) + fibonacci(n-2);
    used[n] = 1;
    return fib[n];
}

Number factorial(int n){
    if (n == 1)
        return Number(1);
    Number a = factorial(n - 1).mult_by_int(n);
    return a;
}

std::pair<Number, Number> gradines(Number n, int st = 0, Number mx = 0){
    if (!n.comp(Number(1)))
        return std::make_pair(st, mx);
    if (n.comp(mx))
            mx = n;
    if (n.isEven()){
        n.divide_by_two();
        return gradines(n, st + 1, mx);}
    else
        return gradines(n.mult_by_int(3)+1, st + 1, mx);
}
