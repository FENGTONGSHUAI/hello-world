class C {

};

int main()
{
    C* c = new C();
    return 0;
}

/*
g++ -c test_new.cpp
objdump -dr test_new.o
c++filt _Znwm
*/