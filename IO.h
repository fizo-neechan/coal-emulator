#pragma once
#include <iostream>
#include <fstream>
#include <cstring>
#include <string>
#include <map>
#include <bitset>
#include <string>
#include <sstream>
using namespace std;

class IO
{
private:
    std::string readFilepath;
    std::string writeFilepath;
    std::map<std::string, std::string> ref;

public:
    IO(std::string r, std::string w)
    {
        this->readFilepath = r;
        this->writeFilepath = w;

        // addressing modes
        ref["DI"] = "00";
        ref["ID"] = "01";
        ref["IM"] = "10";
        ref["IN"] = "11";

        // mem ref instructions
        ref["ADD"] = "000000";
        ref["ADD"] = "000001";
        ref["LDA"] = "000010";
        ref["STA"] = "000011";
        ref["BUN"] = "000100";
        ref["BSA"] = "000101";
        ref["ISZ"] = "000110";
        ref["SIS"] = "000111";
        ref["SIG"] = "001000";
        ref["SIE"] = "001001";
        ref["OR"] = "001010";
        ref["XOR"] = "001011";
        ref["NOT"] = "001100";
        ref["SUB"] = "001101";
        ref["LSH"] = "001110";
        ref["RSH"] = "001111";
        ref["SWP"] = "010000";
        ref["XNOR"] = "010001";
        ref["LIR"] = "010010";

        // reg ref instructions
        ref["CLA"] = "11111101000000000001";
        ref["CLE"] = "11111101000000000010";
        ref["CMA"] = "11111101000000000100";
        ref["CME"] = "11111101000000001000";
        ref["CIR"] = "11111101000000010000";
        ref["CIL"] = "11111101000000100000";
        ref["INC"] = "11111101000001000000";
        ref["DEC"] = "11111101000010000000";
        ref["SPA"] = "11111101000100000000";
        ref["SNA"] = "11111101001000000000";
        ref["SZA"] = "11111101010000000000";
        ref["SZE"] = "11111101100000000000";
        ref["HLT"] = "11111110000000000001";
        ref["CLI"] = "11111110000000000010";

        // IO instructions
        ref["INP"] = "11111111000000000001";
        ref["OUT"] = "11111111000000000010";
        ref["SKI"] = "11111111000000000100"; 
        ref["SKO"] = "11111111000000001000";
        ref["ION"] = "11111111000000010000";
        ref["IOF"] = "11111111000000100000";
    }

    std::string decodeLine(std::string line)
    {
        std::string instruction = "";
        std::string word = "";

        stringstream ln(line);
        while (getline(ln, word, ' '))
        {
            if(line[0]>='0' && line[0]<='9'){
                int num = std::stoi(line);
                instruction+= std::bitset<20>(num).to_string();
                break;
            }

            instruction+=ref[word];
            if(word[0]>='0' && word[0]<='9')
            {
                int num = stoi(word);
                instruction+=bitset<12>(num).to_string();
                break; 
            }

        }

        return instruction;
    }
    void output(string *mem,unsigned int size)
    {
        ofstream Ofile;
        Ofile.open(writeFilepath);

        for(int i =0;i<size;i++)
        {
            Ofile<<mem[i]<<endl;
        }
        Ofile.close();
    }
    void readToMemory(std::string *mem, unsigned int size)
    {
        std::fstream in(this->readFilepath);
        int p_mem = 0;
        while (!in.eof())
        {
            std::string line;
            std::getline(in, line);

            if (line == "\n" || line == "")
                continue;
            mem[p_mem] = this->decodeLine(line);
            std::cout << "line " << p_mem << ": " << mem[p_mem] << std::endl;

            p_mem++;
        }

        system("pause");
    }

    ~IO()
    {
    }
};
