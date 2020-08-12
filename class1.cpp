#include<iostream>
using namespace std;
class Fred;
class Barney{
public:
void method();
private:
Fred* x;
};
class Fred{
public:
void yabbaDabbaDo();
private:
Barney* y;
};
