#include <iostream>
#include <ctype.h>
#include <cstring>

#define print(n,x) std::cout << n << " " << x << "\n"
template<typename T>
class list{
    private:
    T * str;
    int capacity = 0;
    //hidden functions
    int resize(int newSize){
        int newSizeToAllocate = 0;
        if (newSize > capacity){
            newSizeToAllocate = (newSize + newSize/5); //alocates a bit more to accomodate growth
        }
        else if (newSize < capacity - newSize/5){
            newSizeToAllocate = newSize;
        }
        length = newSize;
        if (newSizeToAllocate != 0){
            str = (T*) realloc(str, newSizeToAllocate * sizeof(T));
        }
        capacity = newSizeToAllocate;
        return 1;
    }
    public:
    int length = 0;
    //constructors
    list(){
        str = (T*) malloc(0);
    }
    list(const list<T> &t){
        
    }
    ~list(){
        print("memory before", this->str);
        free(str);
        print("freed memory", "succesfully");
    }
    //functions
    list & append(T num){
        resize(length + 1);
        str[length-1] = num;
        return *this;
    }
    void clear(){
        resize(0);

    }
    //operators
    list & operator = (list & rhs){
        this->capacity = rhs.capacity;
        this->length = rhs.length;
        resize(this->capacity);
        print("test here", *this);
        print("rhs is ", rhs);
        print("source loc = ", rhs.getMemLoc());
        std::memcpy(this->str, rhs.getMemLoc(), rhs.capacity*sizeof(T));
        print("after copy", *this);
        return *this;
    }
    list & operator = (list && rhs){
        return this->operator=(rhs);
    }
    list & operator +=(list & other){
        for (auto ell: other){
            this->append(ell);
        }
        return *this;
    }
    list operator +(const list & rhs){
        list<T> temp;
        temp += *this;
        temp += rhs;
        std::cout << temp;
        return temp;
    }
    list & operator *=(const int & rhs){
        int original_length = this->length;
        resize(length*rhs);
        for (int i = 1; i < rhs; i++){
            std::memcpy(str+(i*original_length), str, (original_length*sizeof(T)));
        }
        return *this;
    }
    list operator *(const int & rhs){
        list<T> temp;
        temp = *this;
        std::cout << "here" << "\n";
        std::cout << temp;
        print("temp ",rhs);
        temp *= rhs;
        print("temp ", temp);
        return temp;
    }
    template <typename S>
    friend std::ostream & operator<<(std::ostream &os, list<S> & l){
        os << "length = " << l.length << "\n" << "elements = " << "\n";
        for (auto ell: l){
            os << ell << "\n";
        }
        os << "memoryloc " << l.str << "\n";
        return os;
    }
    //addons
    auto getMemLoc(){return this->str;}
    auto begin(){return str;}
    auto end(){return str + length;}
};

int main(){
    list<uint16_t> l;
    list<uint16_t> t;
    l.append(5).append(6);
    l.append(7).append(10);
    std::cout << l;
    t = (l*2);
    print("t", t);
    return 0;
}
