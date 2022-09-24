#ifndef list_H
#define list_H

#include <cstring>
#include <iostream>
#include <ctype.h>


namespace py{

    template<typename A, typename B>
    concept addAble = requires(A a, B b){
        std::is_convertible<A, B>::value;
        std::is_constructible<B>::value;
        a + b;
    };
    template<typename A, typename B>
    concept conv = requires(){
        std::is_convertible<A,B>::value;
    };
    template<typename A, typename B>
    concept checkAble = requires(A a, B b){
        a == b;
        a != b;
    };

    template<typename T>
    class list{
        private:
        T * str;
        size_t capacity;

        void resize(size_t newSize){
            size_t newSizeToAllocate = 0;
            if (capacity < newSize){
                newSizeToAllocate = (newSize + newSize/5);
            } else if (newSize < capacity - newSize/5){
                newSizeToAllocate = newSize;
            }
            length = newSize;
            if (newSizeToAllocate != 0){
                str = (T*) realloc(str, newSizeToAllocate * sizeof(T));
            }
            capacity = newSizeToAllocate;
        };

        public:
        size_t length;
        #pragma region constructors
        list(): length(0), capacity(0), str(0){};
        template<size_t N>
        list(const T (&arr)[N]) : list(){
            this->resize(N);
            for (int i = 0; i < N; i++){
                this->str[i] = arr[i];
            }
        };
        ~list(){
            free(this->str);
        };
        #pragma endregion

        #pragma region methods
        /// @brief add the value to the end of the list
        /// @param num value to add
        /// @return
        template<typename A>
        requires conv<A, T>
        list & append(A num){
            resize(this->length + 1);
            this->str[length-1] = (T) num;
            return *this;
        };

        /// @brief adds a list to the end of the list   
        /// @param other other list to add, must be of same type
        /// @return 
        template<typename A>
        requires conv<A, T>
        list & extend(const list<A> & other){
            size_t len = this->length;
            resize(this->length + other.length);
            for (int i = len; i < this->length; i++){
                this->str[i] = (T) other.str[i-len];
            }
            return *this;
        };


        /// @brief adds an element into the list
        /// @param index location to add the element
        /// @param ell value of the element
        /// @return 
        template<typename A>
        requires conv<A, T>
        list & insert(const size_t index, A ell){
            resize(this->length+1);
            A val = ell;
            A temp = A();
            for (int i = index; i< this->length; i++){
                temp = this->str[i];
                this->str[i] = (T) val;
                val = temp;
            }
            return *this;
        };
       
        /// @brief removes a value from the list based on value
        /// @param val value to remove
        template<typename A>
        requires conv<A, T>
        void remove(const A val){
            for (int i = 0; i <this->length; i++){
                if (this->str[i] == (T) val){
                    this->pop(i);
                    return;
                }
            }
        };

        /// @brief removes a value from the list based on index
        /// @param index index of the value to remove
        /// @return value that was deleted
        T pop(const size_t index){
            T result = this->str[index];
            for (int i = index; i < this->length-1; i++){
                this->str[i] = this->str[i+1];
            }
            resize(this->length -1);
            return result;
        };

        /// @brief clears the array
        void clear(){
            resize(0);
        };

        /// @brief get the index of a value
        /// @param ell value to get the index from
        /// @return index of the element, if not found returns -1
        template<typename A>
        requires conv<A, T>
        size_t index(A ell){
            for (int i = 0; i< this->length; i++){
                if (this->str[i] == (T) ell){
                    return i;
                }
            }
            return -1;
        }

        /// @brief count amount of times a value is found
        /// @param x value of the element
        /// @return  amount of times the element is in the list
        template<typename A>
        requires conv<A, T>
        size_t count(A ell){
            size_t cnt = 0;
            for (const T & x: *this){
                if (x == (T) ell){
                    cnt++;
                }
            }
            return cnt;
        };
    
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
        requires conv<M,T> && addAble<L, T>
        L reduce(L (*func)(T, M)){
            L result;
            for (int i = 0; i < this->length; i++){
                result += this->str[i];
            }
            return result;
        }
    
        #pragma endregion

        #pragma region operators

        list & operator = (const list<T> & rhs){
            this->capacity = 0;
            this->length = rhs.length;
            resize(this->length);
            std::memcpy(this->str, rhs.getMemLoc(), rhs.length*sizeof(T));
            return *this;
        }
        
        template<typename A>
        requires conv<A, T>
        list & operator +=(const list<A> & other){
            for (auto ell: other){
                this->append((T) ell);
            }
            return *this;
        }

        template<typename A>
        requires conv<A, T>
        list & operator +(const list<A> & other){
            list<T> temp;
            temp += *this;
            temp += other;
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
            temp *= rhs;
            return temp;
        }

        template<typename A>
        requires checkAble<A, T>
        bool operator ==(const list<A> & rhs){
            if (this->length != rhs.length){
                return false;
            }
            for (int i = 0; i< this->length; i++){
                if (this->str[i] != (T) rhs.str[i]){
                    return false;
                }
            }
            return true;
        }

        template<typename A>
        requires checkAble<A, T>
        bool operator !=(const list<A> & rhs){
            return !(*this == rhs);
        }
        
        #pragma endregion

        #pragma region addons
        T * getMemLoc() const {return this->str;}
        T * begin() const {return str;}
        T * end() const {return str + length;}
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
}



#endif