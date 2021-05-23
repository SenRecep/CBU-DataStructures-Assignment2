#include "Models.h"
int main()
{
    MultipleList *list = new MultipleList();//liste olusturulur
    list->Init();//liste kurulur
    list->Menu();//menu calistirilir
    delete list;//liste temizlenir
    return 0;//program sonlandirilir
}