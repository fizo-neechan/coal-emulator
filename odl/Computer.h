#include "headers.h"

class Computer {
private:
    
    // registers
    Register AC(20, '0');
    Register IR(20, '0');
    Register PC(12, );
    Register AR;
    Register IN;
    Register OUT;
    Register DR;

    // flipflops
    bool I, E, S, IEN, IED;
    bool i0,i1;

    int D;

    // memory
    Register *RAM;


    void T0(){
        PC = AR;
    }

    void T1(){
        IR = RAM[AR.get_decimal()];
        PC.set_val_decimal(PC.get_decimal() + 1);
    }

    void T2() {
        i0 = IR[19] == 1 ? 1 : 0;
        i1 = IR[18] == 1 ? 1 : 0;
        D = Utils.to_decimal(IR[12,17]);
        AR = IR[0,11];
    }

    void T3() {
        if(this->i0 == 0){
            if(this->i1 == 0){
                // direct
                // nothing
            } else {
                // indirect
                AR = RAM[AR.get_decimal()];
            }
        } else {
            if(this->i1 == 0){
                // immediate
                DR = AR;
            } else {
                AR = RAM[AR.get_decimal()];
            }
        }
    }

    void T4(){
        if(this->i0 && this->i1){
            AR = AR + IR;
        }
    }

    



public:
    Computer(/* args */);
    ~Computer();
};

Computer::Computer(/* args */)
{
    AC.set_val_decimal(0);
    IR.set_val_decimal(0);
    PC.set_val_decimal(0);
    AR.set_val_decimal(0);
    IR.set_val_decimal(0);
    OUT.set_val_decimal(0);

    I = 0;
    E = 0;
    S = 0;
    IEN = 0;
    IED = 0;

    RAM = new Register(20);
}

Computer::~Computer()
{
    
}
