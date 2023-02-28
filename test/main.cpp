#include<iostream>
#include<cstring>
using namespace std;


int main() {
    const char* value = "1234";
    value = "2345";
    char* name = "123";
    strcpy(name, value);




    return 0;
}