#include <string>
#include <iostream>

using namespace std;

string v;

double foo()
{
    return 1.1;
}

double g = foo();
int main()
{
    cout << g << endl;
    return 0;
}

/*
g++ -o test2 test2.cpp
*/