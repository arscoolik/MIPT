#include "number.cpp"

int main() {
    for (int i = 0; i< 1000; i++)
        used[i] == 0;
    fib[1] = 1;
    fib[2] = 1;
    used[1] = 1;
    used[2] = 1;
	Number x = 2;
	Number a("4761963248413673697");
	//x.divide_by_two();
	auto t = gradines(a);
	std::cout << t.first << " " <<t.second;
	//std::cout << (Number("1000")).mult_by_int(999) << std::endl;
    //std::cout << " " << factorial(100) << std::endl;

}

