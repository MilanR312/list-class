#include "list.h"
#include <cstring>
#include <iostream>
#include <ctype.h>

int main(){
    py::list<int> l;
    l.append(5);
    std::cout << l;
}