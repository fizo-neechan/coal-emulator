#pragma once
#include <iostream>
#include <fstream>
#include <cstring>
#include <string>
#include <map>
#include <bitset>

class IO
{
private:
    std::string readFilepath;
    std::string writeFilepath;
    std::map<std::string, std::string> ref;
public:
    IO(std::string r, std::string w){
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
        ref["XNR"] = "010001";

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

    }


    std::string decodeLine(std::string line){
        std::string instruction = "";
        std::string word = "";

        bool isNum = line[0]>='0' && line[0]<='9' ? 1:0;
        int last_space = 0;
        for(int i = 0; i < line.length(); i++){
            if(line[i] == ' ' || i == line.length()-1){
                if(line[i] == ' ')
                    last_space = i;


                if(!isNum){
                    instruction += ref[word];

                    if(
                        word == "CLA" || word == "CLE" || word == "CMA" || word == "CME" ||
                        word == "CIR" || word == "CIL" || word == "INC" || word == "DEC" ||
                        word == "SPA" || word == "SNA" || word == "SZA" || word == "SZE" ||
                        word == "HLT" || word == "CLV"
                    ) break;

                    else if(
                        word == "ADD" || word == "ADD" || word == "LDA" || word == "STA" ||
                        word == "BUN" || word == "BSA" || word == "ISZ" || word == "SIS" ||
                        word == "SIG" || word == "SIE" || word == "OR" || word == "XOR" ||
                        word == "NOT" || word == "SUB" || word == "LSH" || word == "RSH" ||
                        word == "SWP" || word == "XNR"
                    ) isNum = 1;

                    word = "";
                } else {
                    if(last_space == 0){
                        word=line;
                    } else {
                        for(int j = last_space+1; j < line.length(); j++){
                            word += line[j];
                        }
                    }
                    if(last_space != 0)
                        instruction += std::bitset<12>(std::stoi(word)).to_string();
                    else
                        instruction += std::bitset<20>(std::stoi(word)).to_string();
                    break;
                }
            } else{
                word += line[i];
            }
        }

        return instruction;
    }

    void readToMemory(std::string * mem, unsigned int size){
        std::fstream in(this->readFilepath);
        int p_mem = 0;
        while(!in.eof()){
            std::string line;
            std::getline(in, line);

            if(line == "\n" || line == "")
                continue;
            mem[p_mem] = this->decodeLine(line);
            std::cout << "line " << p_mem << ": " << mem[p_mem] << std::endl;

            p_mem++;
        }
    }

    ~IO(){

    }
};
