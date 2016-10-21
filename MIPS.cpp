#include<iostream>
#include<string>
#include<vector>
#include<bitset>
#include<fstream>
#include<sstream>
using namespace std;
#define ADDU 1
#define SUBU 3
#define AND 4
#define OR  5
#define NOR 7
#define MemSize 65536 // memory size, in reality, the memory size should be 2^32, but for this lab, for the space resaon, we keep it as this large number, but the memory is still 32-bit addressable.


class RF
{
public:
    bitset<32> ReadData1, ReadData2;
    RF()
    {
        Registers.resize(32);
        Registers[0] = bitset<32> (0);
    }
    
    void ReadWrite(bitset<5> RdReg1, bitset<5> RdReg2, bitset<5> WrtReg, bitset<32> WrtData, bitset<1> WrtEnable)
    {
        // implement the funciton by you.
        if (WrtEnable.to_string() == "1")
        {
            Registers[WrtReg.to_ulong()]= WrtData;
        }
        
        ReadData1= Registers[RdReg1.to_ulong()];
        ReadData2= Registers[RdReg2.to_ulong()];
    }
    
    void OutputRF()
    {
        ofstream rfout;
        rfout.open("/Users/jesse/Documents/C++Application/MIPS/MIPS/MIPS/RFresult.txt",std::ios_base::app);
        if (rfout.is_open())
        {
            rfout<<"A state of RF:"<<endl;
            for (int j = 0; j<32; j++)
            {
                rfout << Registers[j]<<endl;
            }
            
        }
        else cout<<"Unable to open file";
        rfout.close();
        
    }
private:
    vector<bitset<32> > Registers;
    
};

class ALU
{
public:
    bitset<32> ALUresult;
    bitset<32> ALUOperation (bitset<3> ALUOP, bitset<32> oprand1, bitset<32> oprand2)
    {
        // implement the ALU operations by you.
        //R-type addu 21--00100001
        if (ALUOP.to_string() == "001")
        {
            ALUresult= oprand1.to_ulong() + oprand2.to_ulong();
            cout << "ALUresult is: " << ALUresult << endl;
        }
        
        //R-type subu 23--00100011
        if (ALUOP.to_string() == "011")
        {
            ALUresult= oprand1.to_ulong() - oprand2.to_ulong();
        }
        
        //R-Type and 24--00100100
        if (ALUOP.to_string() == "100")
        {
            ALUresult= oprand1 & oprand2;
        }
        
        //R-type or 25--00100101
        if (ALUOP.to_string() == "101")
        {
            ALUresult= oprand1 | oprand2;
        }
        
        //R-type nor 27--00100111
        if (ALUOP.to_string() == "111")
        {
            ALUresult= ~(oprand1 | oprand2);
        }
        
        return ALUresult;
    }
};

class INSMem
{
public:
    bitset<32> Instruction;
    INSMem()
    {       IMem.resize(MemSize);
        ifstream imem;
        string line;
        int i=0;
        imem.open("/Users/jesse/Documents/C++Application/MIPS/MIPS/MIPS/imem.txt");
        if (imem.is_open())
        {
            while (getline(imem,line))
            {
                //IMem[i] = bitset<8>(line);
                IMem[i]= stoul(line, NULL, 2);
                //cout << IMem[i] << endl;
                i++;
            }
            
        }
        else cout<<"Unable to open file";
        imem.close();
        
    }
    
    bitset<32> ReadMemory (bitset<32> ReadAddress)
    {
        // implement by you. (Read the byte at the ReadAddress and the following three byte).
        
        string instruction_pre= "";
        instruction_pre += IMem[ReadAddress.to_ulong()].to_string();
        instruction_pre += IMem[ReadAddress.to_ulong()+1].to_string();
        instruction_pre += IMem[ReadAddress.to_ulong()+2].to_string();
        instruction_pre += IMem[ReadAddress.to_ulong()+3].to_string();
        
        Instruction= bitset<32>(instruction_pre);
        
        return Instruction;
    }
    
private:
    vector<bitset<8> > IMem;
    
};

class DataMem
{
public:
    bitset<32> readdata;
    DataMem()
    {
        DMem.resize(MemSize);
        ifstream dmem;
        string line;
        int i=0;
        dmem.open("/Users/jesse/Documents/C++Application/MIPS/MIPS/MIPS/dmem.txt");
        if (dmem.is_open())
        {
            while (getline(dmem,line))
            {
                //DMem[i] = bitset<8>(line);
                DMem[i]= stoul(line, NULL, 2);
                //cout << DMem[i] << endl;
                i++;
            }
        }
        else cout<<"Unable to open file";
        dmem.close();
        
    }
    bitset<32> MemoryAccess (bitset<32> Address, bitset<32> WriteData, bitset<1> readmem, bitset<1> writemem)
    {
        
        // implement by you.
        //read memory to register--lw
        if (readmem.to_string() == "1" && writemem.to_string() == "0")
        {
            string readdata_pre= "";
            readdata_pre += DMem[Address.to_ulong()].to_string();
            readdata_pre += DMem[Address.to_ulong()+1].to_string();
            readdata_pre += DMem[Address.to_ulong()+2].to_string();
            readdata_pre += DMem[Address.to_ulong()+3].to_string();
            readdata= bitset<32>(readdata_pre);
        }
        
        //write value in the register to memory
        if (writemem.to_string() == "1")
        {
            string str= WriteData.to_string();
            string str1= str.substr(0, 8);
            string str2= str.substr(8, 8);
            string str3= str.substr(16, 8);
            string str4= str.substr(24, 8);
            DMem[Address.to_ulong()]= bitset<8>(str1);
            DMem[Address.to_ulong()+1]= bitset<8>(str2);
            DMem[Address.to_ulong()+2]= bitset<8>(str3);
            DMem[Address.to_ulong()+3]= bitset<8>(str4);
        }
        
        return readdata;
    }
    
    void OutputDataMem()
    {
        ofstream dmemout;
        dmemout.open("/Users/jesse/Documents/C++Application/MIPS/MIPS/MIPS/dmemresult.txt");
        if (dmemout.is_open())
        {
            for (int j = 0; j< 1000; j++)
            {
                dmemout << DMem[j]<<endl;
            }
            
        }
        else cout<<"Unable to open file";
        dmemout.close();
        
    }
    
private:
    vector<bitset<8> > DMem;
    
};



int main()
{
    RF myRF;
    ALU myALU;
    INSMem myInsMem;
    DataMem myDataMem;
    
    bitset<32> PC;
    PC.reset();
    cout << "pc now is: " << PC << endl;
    bitset<32> Instruction;
    cout << "Instruction is: " << Instruction << endl;
    bitset<32> tmp;
    
    //a automatic variable for instruction
    string instruction= "";
    
    while (1)
    {
        Instruction= myInsMem.ReadMemory(PC);
        instruction= Instruction.to_string();
        cout << "instruction is: " << instruction << endl;
        // If current insturciton is "11111111111111111111111111111111", then break;
        
        if (instruction == "11111111111111111111111111111111") break;
        // decode(Read RF)
        //R-type
        if (instruction.substr(0, 6) == "000000")
        {
            
            myRF.ReadWrite(bitset<5>(instruction.substr(6, 5)), bitset<5>(instruction.substr(11, 5)), bitset<5>(instruction.substr(16, 5)), Instruction, bitset<1>(0));
            cout << "readdata1: " << myRF.ReadData1 << "readdata2: " << myRF.ReadData2 << endl;
            tmp= myALU.ALUOperation(bitset<3>(instruction.substr(29, 3)), myRF.ReadData1, myRF.ReadData2);
            cout << "tmp is: " << tmp << endl;
            myRF.ReadWrite(bitset<5>(instruction.substr(6, 5)), bitset<5>(instruction.substr(11, 5)), bitset<5>(instruction.substr(16, 5)), tmp, bitset<1>(1));
            
            cout << "r-type " << PC << endl;
            PC = PC.to_ulong() + 4;
            cout << PC << endl;
            
        }
        //I-type 09 addiu
        else if (instruction.substr(0, 6) == "001001")
        {
            tmp.reset();
            myRF.ReadWrite(bitset<5>(instruction.substr(6, 5)), bitset<5>(instruction.substr(6, 5)), bitset<5>(instruction.substr(11, 5)), Instruction, bitset<1>(0));
            
            string imm= "";
            if (instruction.at(16) == '1')
            {
                imm= "1111111111111111" + instruction.substr(16, 16);
            }
            else
            {
                imm= "0000000000000000" + instruction.substr(16, 16);
            }
            
            tmp= myALU.ALUOperation(bitset<3>(instruction.substr(3, 3)), myRF.ReadData1, bitset<32>(imm));
            myRF.ReadWrite(bitset<5>(instruction.substr(6, 5)), bitset<5>(instruction.substr(6, 5)), bitset<5>(instruction.substr(11, 5)), tmp, bitset<1>(1));
            
            cout << "addiu " << PC << endl;
            PC= PC.to_ulong() + 4;
            cout << PC << endl;
        }
        //I-type 04 beq
        else if (instruction.substr(0, 6) == "000100")
        {
            tmp.reset();
            myRF.ReadWrite(bitset<5>(instruction.substr(6, 5)), bitset<5>(instruction.substr(11, 5)), bitset<5>(instruction.substr(11, 5)), Instruction, bitset<1>(0));
            string imm= "0000000000000000" + instruction.substr(16, 16);
            if (myRF.ReadData1 == myRF.ReadData2)
            {
                PC= PC.to_ulong() + 4 + bitset<32>(imm).to_ulong();
            }
            else
            {
                PC= PC.to_ulong() + 4;
            }
            
            cout << "beq " << PC << endl;
            
        }
        //I-type 23--lw
        else if (instruction.substr(0, 6) == "100011")
        {
            tmp.reset();
            myRF.ReadWrite(bitset<5>(instruction.substr(6, 5)), bitset<5>(instruction.substr(11, 5)), bitset<5>(instruction.substr(11, 5)), Instruction, bitset<1>(0));
            
            string imm= "";
            if (instruction.at(16) == '1')
            {
                imm= "1111111111111111" + instruction.substr(16, 16);
            }
            else
            {
                imm= "0000000000000000" + instruction.substr(16, 16);
            }
            
            tmp= myRF.ReadData1.to_ulong() + bitset<32>(imm).to_ulong();
            tmp= myDataMem.MemoryAccess(tmp, tmp, bitset<1>(1), bitset<1>(0));
            myRF.ReadWrite(bitset<5>(instruction.substr(6, 5)), bitset<5>(instruction.substr(11, 5)), bitset<5>(instruction.substr(11, 5)), tmp, bitset<1>(1));
            cout << "lw " << PC << endl;
            PC = PC.to_ulong() + 4;
            cout << PC << endl;
        }
        //I-type 2B--sw
        else if (instruction.substr(0, 6) == "101011")
        {
            tmp.reset();
            myRF.ReadWrite(bitset<5>(instruction.substr(6, 5)), bitset<5>(instruction.substr(11, 5)), bitset<5>(instruction.substr(11, 5)), Instruction, bitset<1>(0));
            
            string imm= "";
            if (instruction.at(16) == '1')
            {
                imm= "1111111111111111" + instruction.substr(16, 16);
            }
            else
            {
                imm= "0000000000000000" + instruction.substr(16, 16);
            }
            
            tmp= myRF.ReadData1.to_ulong() + bitset<32>(imm).to_ulong();
            myDataMem.MemoryAccess(tmp, myRF.ReadData2, bitset<1>(0), bitset<1>(1));
            
            cout << "sw " << PC << endl;
            PC = PC.to_ulong() + 4;
            cout << PC << endl;
        }
        //J-type 02--j
        else if (instruction.substr(0, 6) == "000010")
        {
            string imm= instruction.substr(6, 26) + "00";
            PC = PC.to_ulong() + 4;
            imm= PC.to_string().substr(0, 4) + imm;
            PC= bitset<32>(imm);
            cout << PC << endl;
        }
        
        // Execute
        
        // Read/Write Mem
        
        // Write back to RF
        
        myRF.OutputRF(); // dump RF;
    }
    myDataMem.OutputDataMem(); // dump data mem
    
    return 0;
    
}
