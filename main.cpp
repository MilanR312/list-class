#include <iostream>


template<const char sep = ' ', typename ...Args>
void print(Args && ...args){
    ((std::cout << args << sep) , ...) << "\n"; //expands to (Pack1 , (Pack2 , (Pack3 , Pack4)))
}

int main(){
    print<'t'>("hello", "world");
}
