#pragma once
#include <iostream>
#include <bitset>



struct Register
{
    std::string data;
    int bits;

    Register(){ 
        // none
    }

    Register(int bits, char data){
        this->bits = bits;
        this->data = std::string(bits, data);
    }

    ~Register(){

    }
};

