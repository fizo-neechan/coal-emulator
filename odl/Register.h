#include "headers.h"


class Register {
    private:

        unsigned int size;
        int decimal;
        int binary;
        std::string hex;

    public:
        Register(){

        }

        Register(int size){
            this->size = size;
        }

        Register(int size, int decimal){
            this->size = size;
            this->decimal = decimal;
            this->binary = Utils.to_binary(decimal);
            this->hex = Utils.to_hex(decimal);
        }

        void set_val_decimal(int decimal){
            this->decimal = decimal;
            this->binary = Utils.to_binary(decimal);
            this->hex = Utils.to_hex(decimal);
        }   
        
        void set_val_binary(int binary){
            this->binary = binary;
            this->decimal = Utils.to_decimal(binary);
            this->hex = Utils.binary_to_hex(binary);
        }

        void set_val_hex(std::string hex){
            this->hex = hex;
            this->decimal = Utils.hex_to_decimal(hex);
            this->binary = Utils.hex_to_binary(hex);
        }

        int get_decimal(){
            return this->decimal;
        }

        int get_binary(){
            return this->binary;
        }

        std::string get_hex(){
            return this->hex;
        }

        char operator [] (int x){
            return std::to_string(this->binary)[x];
        }

        std::string operator () (int x, int y){
            std::string out;
            for(int i = x; i < y; i++)
                out += std::to_string(this->binary)[i];
            return out;
        }

};