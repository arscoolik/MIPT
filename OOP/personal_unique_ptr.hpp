template <typename T>


class uniquePtr {
private:
	T* ptr = nullptr;
public:
	uniquePtr(): ptr(nullptr) {} //different types of constructors

	explicit uniquePtr(T* ptr) : ptr(ptr) {}

	uniquePtr(const uniquePtr& tmp) = delete;

	uniquePtr& operator=(const uniquePtr& tmp) = delete;

	uniquePtr(uniquePtr&& del) {
		this->ptr = del.ptr;
		del.ptr = nullptr;
	}

	void operator=(uniquePtr&& del) {//equal operator overloading
		if (ptr != nullptr)
			delete ptr;
		this->ptr = del.ptr;
		del.ptr = nullptr;
	}

	T* operator->() { //another operator for getting the value
		return ptr;
	}

	T& operator*() { //getting the pointer
		return *ptr;
	}

    explicit operator bool[] const { //operator for getting the pointer's value
        return ptr;
    }

	~uniquePtr() { //destructor
		if (ptr != nullptr)
			delete ptr;
	}
};
