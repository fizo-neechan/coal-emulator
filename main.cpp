#include <iostream>
#include "Comp.h"

int main()
{

    string iFile;
    cout << "Enter input File: ";
    cin >> iFile;

    string oFile;
    cout << "Enter output File: ";
    cin >> oFile;
    Comp c(iFile,oFile);


    float clock;
    cout << "Enter clock speed: ";
    cin >> clock;
    c.run((1 / clock));
    c.output(oFile);

 


    return 0;
}