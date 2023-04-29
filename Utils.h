#pragma once
#include "headers.h"

class Utils
{

public:
    Utils(/* args */);
    ~Utils();

    static int to_binary(int decimal){
        int binary = 0;
        int remainder;
        int product = 1;
        while (decimal != 0) {
            remainder = decimal % 2;
            binary = binary + (remainder * product);
            decimal = decimal / 2;
            product *= 10;
        }
    }

    static int to_decimal(int binary){
        int decimal = 0;
        std::string binary_str = std::to_string(binary);
        for(int i = 0; i < binary_str.length(); i++){
            if(binary_str[i] == '1'){
                decimal += 2^i;
            }
        }
    }

    
};

Utils::Utils(/* args */)
{
}

Utils::~Utils()
{
}
