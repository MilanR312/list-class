#include <iostream>
#include <ctype.h>
#include <cstring>

#define print(n,x) std::cout << n << " " << (x) << "\n"

template<typename A, typename T>
concept addAble = requires(T a, A b){
    std::is_convertible<A, T>::value;
    std::is_constructible<T>::value;
    a + b;
};

template<typename T>
class list{
    private:
    T * str = 0;
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
    #pragma region constructors
    list(){
        str = (T*) malloc(0);
    }
    template<size_t N>
    list(const T (&arr)[N]) : list(){
        resize(N);
        for (int i = 0; i < N; i++){
            this->str[i] = arr[i];
        }
    }
    /*template<list<typename T>>
    list(const list<T> &t){
        //todo add constructor from other list with templating
    }*/
    ~list(){
        free(str);
    }
    #pragma endregion
    
    #pragma region methods
    /// @brief add the value to the end of the list
    /// @param num value to add
    /// @return
    list & append(T num){
        resize(length + 1);
        str[length-1] = num;
        return *this;
    }
    /// @brief adds a list to the end of the list
    /// @param other other list to add, must be of same type
    /// @return 
    list & extend(const list & other){
        int len = this->length;
        resize(this->length + other.length); //also mutates length internally
        for (int i = len; i < this->length; i++){
            this->str[i] = other.str[i-len];
        }
        return *this;
    }
    /// @brief adds an element into the list
    /// @param index location to add the element
    /// @param ell value of the element
    /// @return 
    list & insert(const int index, T ell){
        resize(this->length+1);
        T val = ell;
        T temp = 0;
        for (int i = index; i < this->length; i++){
            temp = this->str[i];
            this->str[i] = val;
            val = temp;
        }
        return *this;
    }
    /// @brief removes a value from the list based on value
    /// @param val value to remove
    void remove(const T val){
        for (int i = 0; i < this->length; i++){
            if (val == this->str[i]){
                this->pop(i);
            }
        }
    }
    /// @brief removes a value from the list based on index
    /// @param index index of the value to remove
    /// @return value that was deleted
    T pop(const int index){
        T result = this->str[index];
        for (int i = index; i < this->length-1; i++){
            this->str[i] = this->str[i+1];
        }
        resize(this->length -1);
        return result;
    }
    /// @brief clears the array
    void clear(){
        resize(0);
    }
    /// @brief get the index of a value
    /// @param ell value to get the index from
    /// @return index of the element, if not found returns -1
    size_t index(T ell){
        for (int i = 0; i< this->length; i++){
            if (ell == this->str[i]){
                return i;
            }
        }
        return -1;
    }
    /// @brief count amount of times a value is found
    /// @param x value of the element
    /// @return  amount of times the element is in the list
    size_t count(T x){
        size_t cnt = 0;
        for (const T & ell: *this){
            if (x == ell){
                cnt++;
            }
        }
        return cnt;
    }

    /// @brief calls the given function on every element of the list and returns a new list
    /// @tparam L return type of the function
    /// @tparam M 
    /// @param func 
    /// @return 
    template<typename L, typename M> 
    requires std::is_convertible<L, T>::value && std::is_convertible<T, M>::value
    list<L> map(L (*func)(M)){
        list<L> result;
        for (const auto & ell: *this){
            result.append( (*func)(ell) );
        }
        /*result.length = this->length;
        result.resize(result.length);
        for (int i = 0; i < this->length; i++){
            result.getMemLoc()[i] = (*func)(this->str[i]);
        }*/
        return result;
    }
     
    /// @brief iterates over the list reducing it to a single value
    /// @tparam L resulting datatype
    /// @tparam M datatype of the current value
    /// @param func function to reduce the list
    /// @return returns the reduced value as datatype L
    template<typename L, typename M>
    requires std::is_convertible<T, M>::value && addAble<L, T>
    L reduce(L (*func)(T, M)){
        L result;
        for (int i = 0; i < this->length; i++){
            result += this->str[i];
        }
        return result;
    }

    #pragma endregion

    #pragma region operators
    list & operator = (const list & rhs){
        this->capacity = 0;
        this->length = rhs.length;
        resize(this->length);
        std::memcpy(this->str, rhs.getMemLoc(), rhs.length*sizeof(T));
        return *this;
    }
    list & operator +=(const list & other){
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
        //print("temp ",rhs);
        temp *= rhs;
        //print("temp ", temp);
        return temp;
    }
    bool operator ==(const list & rhs){
        if (this->length != rhs.length){
            return false;
        }
        for (int i = 0; i < this->length; i++){
            if (this->str[i] != rhs.getMemLoc()[i]){
                return false;
            }
        }
        return true;
    }
    bool operator !=(const list & rhs){
        return !(*this == rhs);
    }
    #pragma endregion
    
    #pragma region addons
    auto getMemLoc() const {return this->str;}
    auto begin() const {return str;}
    auto end() const {return str + length;}
    #pragma endregion
};

template <typename S>
std::ostream & operator<<(std::ostream &os, const list<S> & l){
    os << "length = " << l.length << "\n" << "elements = " << "\n";
    for (const auto ell: l){
        os << ell << "\n";
    }
    return os;
}

typedef list<uint16_t> lst;

float sum(uint16_t prev){
    return prev + 0.5;
}

int main(int argc, char *argv[]){
    lst a({5,2,3,4,5});
    list<float> b;
    b = a.map(sum);
    print("result", b);
    return 0;
}
