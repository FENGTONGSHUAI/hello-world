// testpp.cpp
#include "iostream"
#include "string"

using namespace std;

int main(int argc, char* argv[])
{
    string* msg = new string("Hello World");
    cout << *msg << endl;
    delete msg;

    return 0;
}

/*
20240728暂且算是看完了吧.
*/