#include <iostream>
#include <fstream>
#include "Models.h"
using namespace std;

int main()
{
    MultipleList *list = new MultipleList();
    list->Init();
    list->Menu();
    delete list;
    return 0;
}