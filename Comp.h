#pragma once
#include <iostream>
#include <bitset>
#include "IO.h"
#include "Register.h"

class Comp {
    private:
        // registers
        std::string AC;
        std::string IR;
        std::string PC;
        std::string AR;
        std::string IN;
        std::string OUT;
        std::string DR;
        std::string index;

        // flipflops
        bool I, E, S, IEN, IED, FGI, FGO;
        bool i0,i1;
        

        int D;
        int SC;

        // memory
        std::string *RAM;

    public:
    
        Comp(){

            IO io("/home/fizo/Documents/coal-emulator/console/sample.txt",
                    "");

            this->AC = std::string(20, '0');
            this->IR = std::string(20, '0');
            this->PC = std::string(12, '0');
            this->AR = std::string(12, '0');
            this->IN = std::string(20, '0');
            this->OUT = std::string(20, '0');
            this->DR = std::string(20, '0');
            this->index = std::string(20, '0');

            this->RAM = new std::string[4096]{std::string(20,'0')};
            this->SC = 0;

            io.readToMemory(this->RAM, 4096);
        }

        int binaryToDecimal(std::string binary){
            // int dec = 0;
            // for(int i = 0; i < binary.length(); i++){
            //     if(binary[i]=='1')
            //         dec += (2^(binary.length()-i-1));
            // }

            int dec;
            dec = std::bitset<20>(binary).to_ulong();

            return dec;
        }

        std::string decimalToBinary(int decimal, int size){
            if(size == 12){
                return std::bitset<12>(decimal).to_string();
            } else {
                return std::bitset<20>(decimal).to_string();
            }
        }

        std::string notGate(std::string binary){
            std::string out = "";
            for(int i = binary.length()-1; i >= 0; i--){
                out += binary[i] == '1' ? 0:1;
            }
            
            return out;
        }

        void t0(){
            this->AR = this->PC;

            this->SC += 1;
        }

        void t1(){
            this->IR = this->RAM[binaryToDecimal(this->AR)];
            this->PC = this->decimalToBinary(this->binaryToDecimal(this->PC) + 1, 12);

            this->SC += 1;
        }

        void t2(){
            this->i1 = IR[0] == '1' ? 1 : 0;
            this->i0 = IR[1] == '1' ? 1 : 0;

            this->D = binaryToDecimal(this->IR.substr(2, 6));

            this->AR = this->IR.substr(8, 12);

            this->SC += 1;
        }

        void t3(){
            if(this->i1 == 0){
                if(this->i0 == 0){
                    // 00 direct
                    // nothing
                } else {
                    // 01 indirect
                    this->AR = this->RAM[this->binaryToDecimal(this->AR)];
                }
            } else {
                if(this->i0 == 0){
                    // 10 immediate
                    this->DR = std::string(20, '0');
                    for(int i = this->AR.length() - 1; i >= 0; i++){
                        this->DR[i] = this->AR[i];
                    }
                } else {
                    // 11 indexed
                    this->AR = this->RAM[this->binaryToDecimal(this->AR)];
                }
            }

            this->SC += 1;
        }

        void t4(){
            if(this->i0 == 1 && this->i1 == 1){
                // 11 indexed
                this->AR = decimalToBinary(binaryToDecimal(this->AR) + binaryToDecimal(this->index), 12);
            } else{
                // nothing!
            }

            this->SC += 1;
        }

        void t5(){
            // bool x = this->i1 && this->i0;
            if(!(this->i1 && this->i0)){
                switch(this->D){
                    case 3:
                        this->RAM[binaryToDecimal(this->AR)] = this->AC;
                        this->SC = 0;
                        return;
                        break;
                    case 4:
                        this->PC = this->AR;
                        this->SC = 0;
                        return;
                        break;
                    case 5:
                        this->RAM[binaryToDecimal(this->AR)] = this->PC;
                        break;
                    case 0:
                    case 1:
                    case 2:
                    case 6:
                    case 7:
                    case 8:
                    case 9:
                    case 10:
                    case 11:
                    case 12:
                    case 13:
                    case 16:
                    case 17:
                        this->DR = this->RAM[binaryToDecimal(this->AR)];
                        break;
                    case 14:
                    case 15:
                        this->DR = this->AR;
                        break;
                    default:
                        std::cout << "ERROR UNDEFINED INSTRUCTION T5 :: D=" << this->D << std::endl;
                        break;
                }
            } else {
                    // do nothing
            }

            this->SC += 1;
        }

        std::string lsh(std::string binary){
            std::string out = binary;

            for(int i = 0; i < out.length()-1; i++){
                out[i] = out[i+1];
            }

            out[out.length()-1] = '0';
            return out;
        }

        std::string rsh(std::string binary){
            std::string out = binary;

            for(int i = out.length()-1; i < 0; i++){
                out[i] = out[i-1];
            }

            out[0] = '0';
            return out;
        }

        void t6(){
            switch(this->D){
                case 0:
                    this->AC = decimalToBinary(binaryToDecimal(this->AC) & binaryToDecimal(this->DR), 20);
                    this->SC = 0;
                    return;
                    break;
                case 1:
                    this->AC = decimalToBinary(binaryToDecimal(this->AC) + binaryToDecimal(this->DR), 20);
                    this->SC = 0;
                    return;
                    break;
                case 2:
                    this->AC = this->DR;
                    this->SC = 0;
                    return;
                    break;
                case 5:
                    this->PC = this->AR;
                    this->SC = 0;
                    return;
                    break;
                case 6:
                    this->DR = decimalToBinary(binaryToDecimal(this->DR) + 1, 20);
                    break;
                case 7:
                    if(binaryToDecimal(this->AC) < binaryToDecimal(this->DR)){
                        this->PC = decimalToBinary(binaryToDecimal(this->PC) + 1, 12);
                        this->SC = 0;
                    } else { /* nothing */ }
                    return;
                    break;
                case 8:
                    if(binaryToDecimal(this->AC) > binaryToDecimal(this->DR)){
                        this->PC = decimalToBinary(binaryToDecimal(this->PC) + 1, 12);
                        this->SC = 0;
                    } else { /* nothing */ }
                    return;
                    break;
                case 9:
                    if(binaryToDecimal(this->AC) == binaryToDecimal(this->DR)){
                        this->PC = decimalToBinary(binaryToDecimal(this->PC) + 1, 12);
                        this->SC = 0;
                    } else { /* nothing */ }
                    return;
                    break;
                case 10:
                    this->AC = decimalToBinary(binaryToDecimal(this->AC) | binaryToDecimal(this->DR), 20);
                    this->SC = 0;
                    return;
                    break;
                case 11:
                    this->AC = decimalToBinary(binaryToDecimal(this->AC) ^ binaryToDecimal(this->DR), 20);
                    this->SC = 0;
                    return;
                    break;
                case 12:
                    this->DR = notGate(this->DR);
                    this->SC = 0;
                    return;
                    break;
                case 13:
                    this->AC = decimalToBinary(binaryToDecimal(this->AC) - binaryToDecimal(this->DR), 20);
                    this->SC = 0;
                    return;
                    break;
                case 14:
                    this->AC = lsh(this->AC);
                    break;
                case 15:
                    this->AC = rsh(this->AC);
                    break;
                case 16:
                    this->RAM[binaryToDecimal(this->AR)] = this->AC;
                    break;
                case 17:
                    this->AC = decimalToBinary(binaryToDecimal(this->AC) ^ binaryToDecimal(this->DR), 20);
                    break;
                default:
                    std::cout << "ERROR UNDEFINED INSTRUCTION T6 :: D=" << this->D << std::endl;
                    break;
            }

            this->SC += 1;
        }


        void t7(){
            switch(this->D){
                case 6:
                    if(binaryToDecimal(this->DR) == 0){
                        this->RAM[binaryToDecimal(this->AR)] = this->DR;
                    } else {
                        this->PC = decimalToBinary(binaryToDecimal(this->PC) + 1, 12);
                    }


                    this->SC = 0;
                    return;
                    break;
                case 14:
                case 15:
                    if(binaryToDecimal(this->DR) == 0){
                        this->SC = 0;
                    } else {
                        this->SC = 6;
                    }
                    break;
                case 16:
                    this->AC = this->DR;
                    this->SC = 0;
                    return;
                    break;
                case 17:
                    this->AC = notGate(this->AC);
                    this->SC = 0;
                default:
                    std::cout << "ERROR UNDEFINED INSTRUCTION T7 :: D=" << this->D << std::endl;
                    break;
            }
        }

        void regRefT3(){
            int b = B(this->IR);
            if(this->D == 62){
                switch(b){
                    case 12:
                        this->AC = std::string(20, '0');
                        break;
                    case 11:
                        this->E = !this->E;
                        break;
                    case 10:
                        this->E = AC[0] == '1' ? 1:0;
                        this->AC = rsh(this->AC);
                        AC[19] = this->E == 1 ? '1':'0';
                        break;
                    case 9:
                        this->E = AC[19] == '1' ? 1:0;
                        this->AC = lsh(this->AC);
                        AC[0] = this->E == 1 ? '1':'0';
                        break;
                    case 8:
                        this->AC = decimalToBinary(binaryToDecimal(this->AC)+1, 20);
                        break;
                    case 7:
                        this->AC = decimalToBinary(binaryToDecimal(this->AC)-1, 20);
                        break;
                    case 6:
                        if(this->AC[19] == '0'){
                            this->PC = decimalToBinary(binaryToDecimal(this->PC)+1, 12);
                        }
                        break;
                    case 5:
                        if(this->AC[19] == '1'){
                            this->PC = decimalToBinary(binaryToDecimal(this->PC)+1, 12);
                        }
                        break;
                    case 4:
                        if(this->AC == "0000000000000000000"){
                            this->PC = decimalToBinary(binaryToDecimal(this->PC)+1, 12);
                        }
                        break;
                    case 3:
                        if(this->E == 0){
                            this->PC = decimalToBinary(binaryToDecimal(this->PC)+1, 12);
                        }
                        break;
                    case 2:
                        this->S = 0;
                        break;
                    case 1:
                        this->index = std::string(12, '0');
                        break;
                    default:
                        std::cout << "BRO WTF regT3 :: D =" << this->D << " && B = " << b << std::endl;
                }
            } else if(this->D == 63){
                switch(b){
                    case 2:
                        this->E = 0;
                        break;
                    case 1:
                        this->AC = notGate(this->AC);
                        break;
                    default:
                        std::cout << "UNDEFINED regT3 :: D =" << this->D << " && B = " << b << std::endl;
                }
            } else {
                std::cout << "UNDEFINED regT3 :: D =" << this->D << std::endl;
            }

            this->SC = 0;
        }

        void ioT3(){
            int b = B(this->IR);
            if(this->D == 64){
                switch(b){
                    case 1:
                        this->AC = this->IN;
                        this->FGI = 0;
                        break;
                    case 2:
                        this->OUT = this->AC;
                        this->FGO = 0;
                        break;
                    case 3:
                        if(this->FGI){
                            this->PC = decimalToBinary(binaryToDecimal(this->PC)+1, 12);
                        }
                        break;
                    case 4:
                        if(this->FGO){
                            this->PC = decimalToBinary(binaryToDecimal(this->PC)+1, 12);
                        }
                        break;
                    case 5:
                        this->IEN = 1;
                        break;
                    case 6:
                        this->IEN = 0;
                        break;
                    default:
                        std::cout << "UNDEFINED ioT3 :: D =" << this->D << " && B = " << b << std::endl;
                        break;
                }
            } else {
                std::cout << "UNDEFINED ioT3 :: D =" << this->D << std::endl;
            }
        }

        void print(){
            std::cout << std::endl;
            std::cout << "SC = " << this->SC << std::endl;
            std::cout << "IR = " << this->IR <<  std::endl;
            std::cout << "AR = " << this->AR <<  std::endl;
            std::cout << "PC = " << this->PC <<  std::endl;
            std::cout << "AC = " << this->AC <<  std::endl;
            std::cout << "DR = " << this->DR <<  std::endl;
            std::cout << "OUT = " << this->OUT <<  std::endl;
            std::cout << "index = " << this->index <<  std::endl;

            std::cout << "E = " << this->E <<  std::endl;
            std::cout << "S = " << this->S <<  std::endl;
            std::cout << "IEN = " << this->IEN <<  std::endl;
            std::cout << "IED = " << this->IED <<  std::endl;
            std::cout << "I = " << this->I <<  std::endl;
            std::cout << "D = " << this->D <<  std::endl;

        }

        int B(std::string binary){
            for(int i = binary.length()-1; i >= 0; i--){
                if(binary[i] == '1'){
                    return binary.length() - i + 1;
                }
            }

            std::cout << "BRO WTF NO B :: int B" << std::endl;
            return 0;
        }

        void run(){
            // hlt fe001
            int pc = 0;
            while(this->RAM[pc] != "11111110000000000001"){

                switch (this->SC){
                    case 0:
                        t0();
                        break;
                    case 1:
                        t1();
                        break;
                    case 2:
                        t2();
                        break;
                    case 3:
                        if(this->i1 && this->i0 && (this->D == 62 || this->D == 63)){
                            regRefT3();
                        } else if(this->i1 && this->i0 && this->D == 64) {
                            ioT3();
                        } else {
                            t3();
                        }
                        break;
                    case 4:
                        t4();
                        break;
                    case 5:
                        t5();
                        break;
                    case 6:
                        t6();
                        break;
                    case 7:
                        t7();
                        break;
                    default:
                        std::cout << "UNCAUGHT :: SC=" << this->SC << std::endl;
                        break;
                }

                this->print();
            }
        }
        

        ~Comp(){

        }
};

// this->D = 