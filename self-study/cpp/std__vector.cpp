#include <iostream>

template<typename T>
class Vector {
private:
    int size;
    int capacity;
    T* data;

public:
    Vector(int capacity) : size(0), capacity(capacity), data(new T[capacity]) {}

    ~Vector() {
        delete[] data;
    }

    Vector(const Vector& rhs) : size(rhs->size), capacity(rhs->capacity), data(new T[rhs->capacity]){
        for(int i = 0; i < rhs.size(); ++i) this->data[i] = rhs->data[i];
    }
    //copy ctor
    Vector& operator=(const Vector& rhs){
        delete[] data;
        size = rhs.size;
        capacity = rhs.capacity;
        data = new T[capacity];
        for(int i = 0; i < rhs.size(); ++i) this->data[i] = rhs->data[i];
        return *this;
    }

    //move constructor
    Vector& operator=(Vector&& rhs){
        //can do std::swap for each thing instead. then we skip a delete[]
        delete[] data;
        this->data = rhs->data;
        this->capacity = rhs->capacity;
        this->size = rhs->size;
        return *this;
    }

    T& operator[](int index){
        return data[index];
    }

    void resize(){
        this->capacity = capacity * 2;
        T* newData = new T[capacity * 2];
        for(int i = 0; i < this->size; ++i) newData[i] = data[i];
        delete[] data;
        data = newData;
    }

    void push_back(const T& value){
        if(size == capacity){
            resize();
        }
        data[size++] = value;
    }

    int _size() {
        return this->size;
    }
};

int main(){
    Vector<int> vec(10);
    vec.push_back(1);
    vec.push_back(2);
    vec.push_back(3);
    for(int i = 0; i < vec._size(); ++i){
        std::cout << "val: " << vec[i] << " ";
    }
}