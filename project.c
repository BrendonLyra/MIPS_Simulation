//Brendon Lyra
//Bailey Waldorf
//Jonathon Irigoyen
//12/1/17/
#include "spimcore.h"


/* ALU */
/* 10 Points */
void ALU(unsigned A,unsigned B,char ALUControl,unsigned *ALUresult,char *Zero)
{
    if (ALUControl == 0)
    {
        //Z = A+B
        *ALUresult = A+B;
    }
    else if (ALUControl == 1)
    {
        //Z = A-B
        *ALUresult = A-B;
    }
    else if (ALUControl == 2)
    {
        //if A<B, Z = 1, otherwise Z = 0 (signed)
        *ALUresult = ((int) A < (int) B) ? 1 : 0;
    }
    else if (ALUControl == 3)
    {
        //if A<B then Z = 1, otherwise Z = 0 (unsigned)
        *ALUresult = (A<B)? 1 : 0;
    }
    else if (ALUControl == 4)
    {
        //Z = A and B
        *ALUresult = (A&B);
    }
    else if (ALUControl == 5)
    {
        //Z = A or B
        *ALUresult = (A|B);
    }
    else if (ALUControl == 6)
    {
        //z = to B left shift 16
        *ALUresult = B<<16;
    }
    else if (ALUControl == 7)
    {
        //Z = NOT A
        *ALUresult = (~A);

    }


    //make zero = 0
    *Zero = 0;
    //and if alu result is 0, make *Zero = 1
    if (*ALUresult == 0)
        *Zero = 1;


}

/* instruction fetch */
/* 10 Points */
int instruction_fetch(unsigned PC,unsigned *Mem,unsigned *instruction)
{
    //checking if word aligned
    if (PC%4==0) {
        //copying instructions from memory, and putting into instruction
        *instruction = Mem[PC>>2];
        return 0;
    }
    //halt program if PC is not word aligned
    return 1;
}


/* instruction partition */
/* 10 Points */
void instruction_partition(unsigned instruction, unsigned *op, unsigned *r1,unsigned *r2, unsigned *r3, unsigned *funct, unsigned *offset, unsigned *jsec)
{
    //bitmasking first 6 bits and shifting all the way to the right to find op
    *op = (instruction & 0b11111100000000000000000000000000) >> 26;
    //bitmasking 25-21 bits and shifting all the way to the right to find r1
    *r1 = (instruction & 0b00000011111000000000000000000000) >> 21;
    //bitmasking 20-16 bits and shifting all the way to the right to find r2
    *r2 = (instruction & 0b00000000000111110000000000000000) >> 16;
    //bitmasking 15-11 bits and shifting all the way to the right to find r3
    *r3 = (instruction & 0b00000000000000001111100000000000) >> 11;
    //bitmasking 5-0 bits to find funct
    *funct = (instruction & 0b00000000000000000000000000111111);
    //bitmasking first 15-0 bits to find offset
    *offset = (instruction & 0b00000000000000001111111111111111);
    //bitmasking first 25-0 bits to find jsec
    *jsec = (instruction & 0b00000011111111111111111111111111);

}



/* instruction decode */
/* 15 Points */
int instruction_decode(unsigned op,struct_controls *controls)
{

    //load word
    if (op == 0b00100011)
    {
        controls->ALUOp = 0;
        controls->ALUSrc = 1;
        controls->Branch = 0;
        controls->Jump = 0;
        controls->MemRead = 1;
        controls->MemtoReg = 1;
        controls->MemWrite = 0;
        controls->RegDst = 0;
        controls->RegWrite = 1;
        return 0;

    }
    //R-type
    else if (op == 0b00000000)
    {
        controls->ALUOp = 7;
        controls->ALUSrc = 0;
        controls->Branch = 0;
        controls->Jump = 0;
        controls->MemRead = 0;
        controls->MemtoReg = 0;
        controls->MemWrite = 0;
        controls->RegDst = 1;
        controls->RegWrite = 1;
        return 0;
    }
    //addi
    else if (op == 0b00001000)
    {
        controls->ALUOp = 0;
        controls->ALUSrc = 1;
        controls->Branch = 0;
        controls->Jump = 0;
        controls->MemRead = 0;
        controls->MemtoReg = 0;
        controls->MemWrite = 0;
        controls->RegDst = 0;
        controls->RegWrite = 1;
        return 0;
    }
    //save word
    else if (op == 0b00101011)
    {
        controls->ALUOp = 0;
        controls->ALUSrc = 1;
        controls->Branch = 0;
        controls->Jump = 0;
        controls->MemRead = 0;
        controls->MemtoReg = 2;
        controls->MemWrite = 1;
        controls->RegDst = 2;
        controls->RegWrite = 0;
        return 0;

    }
    //load unsigned immediate
    else if (op == 0b00001111)
    {
        controls->ALUOp = 6;
        controls->ALUSrc = 1;
        controls->Branch = 0;
        controls->Jump = 0;
        controls->MemRead = 0;
        controls->MemtoReg = 0;
        controls->MemWrite = 0;
        controls->RegDst = 0;
        controls->RegWrite = 1;
        return 0;

    }
    //jump
    else if (op == 0b00000010)
    {
        controls->ALUOp = 0;
        controls->ALUSrc = 0;
        controls->Branch = 0;
        controls->Jump = 1;
        controls->MemRead = 0;
        controls->MemtoReg = 0;
        controls->MemWrite = 0;
        controls->RegDst = 0;
        controls->RegWrite = 0;
        return 0;

    }
    //branch equal
    else if (op == 0b00000100)
    {
        controls->ALUOp = 1;
        controls->ALUSrc = 0;
        controls->Branch = 1;
        controls->Jump = 0;
        controls->MemRead = 0;
        controls->MemtoReg = 2;
        controls->MemWrite = 0;
        controls->RegDst = 2;
        controls->RegWrite = 0;
        return 0;
    }
    //set less than immediate
    else if (op == 0b00001010)
    {
        controls->ALUOp = 2;
        controls->ALUSrc = 1;
        controls->Branch = 0;
        controls->Jump = 0;
        controls->MemRead = 0;
        controls->MemtoReg = 0;
        controls->MemWrite = 0;
        controls->RegDst = 0;
        controls->RegWrite = 1;
        return 0;
    }
    //set less than unsigned immediate
    else if (op == 0b00001011)
    {
        controls->ALUOp = 3;
        controls->ALUSrc = 1;
        controls->Branch = 0;
        controls->Jump = 0;
        controls->MemRead = 0;
        controls->MemtoReg = 0;
        controls->MemWrite = 0;
        controls->RegDst = 0;
        controls->RegWrite = 1;
        return 0;

    }

    //no operation was found, set halt to 1.
    return 1;
}

/* Read Register */
/* 5 Points */
void read_register(unsigned r1,unsigned r2,unsigned *Reg,unsigned *data1,unsigned *data2)
{
    *data1 = Reg[r1];
    *data2 = Reg[r2];
}


/* Sign Extend */
/* 10 Points */
void sign_extend(unsigned offset,unsigned *extended_value)
{
    //bitmasking first bit to see if offset is negative
    if ((offset & 0b1000000000000000) == 0b1000000000000000)
    {
        //if negative, making first 16 bits all 1s
        *extended_value = offset | 0b11111111111111110000000000000000;
    }
    else
    {
        //if positive, make last 16 bits all 1s
        *extended_value = offset & 0b00000000000000001111111111111111;
    }

}

/* ALU operations */
/* 10 Points */
int ALU_operations(unsigned data1,unsigned data2,unsigned extended_value,unsigned funct,char ALUOp,char ALUSrc,unsigned *ALUresult,char *Zero)
{
    if (ALUSrc == 1)
        data2 = extended_value;

    if (ALUOp == 0b00000111)
    {
        //add
        if (funct == 0b00100000)
            ALUOp = 0;
        //subtract
        else if (funct == 0b00100010)
            ALUOp = 1;
        //set less than
        else if (funct == 0b00101010)
            ALUOp = 2;
        //set less than unsigned
        else if (funct == 0b00101011)
            ALUOp = 3;
        //and
        else if (funct == 0b00100100)
            ALUOp = 4;
        //or
        else if (funct == 0b00100101)
            ALUOp = 5;
        else
            //halt program
            return 1;
    }
    else if (ALUOp > 0b00000111){
        return 1;
     //   ALU(data1, data2, ALUOp, ALUresult, Zero);
    }


    ALU(data1, data2, ALUOp, ALUresult, Zero);


    return 0;
}

/* Read / Write Memory */
/* 10 Points */
int rw_memory(unsigned ALUresult,unsigned data2,char MemWrite,char MemRead,unsigned *memdata,unsigned *Mem)
{
    //read memory
    if (MemRead == 1)
    {
        if (ALUresult%4 == 0)
            *memdata = Mem[ALUresult>>2];
        else
            //if not word aligned, halt program.
            return 1;
    }

    //write memory
    else if (MemWrite == 1) {

        if (ALUresult %4 == 0 )
            Mem[ALUresult>>2] = data2;
        else
            //if not word aligned, halt
            return 1;
    }
    return 0;
}


/* Write Register */
/* 10 Points */
void write_register(unsigned r2,unsigned r3,unsigned memdata,unsigned ALUresult,char RegWrite,char RegDst,char MemtoReg,unsigned *Reg)
{
    //writing to registry
    if (RegWrite == 1){
        //if data coming from memory
        if (MemtoReg == 1)
            Reg[r2] = memdata;
        //data coming from register
        else if (MemtoReg == 0)
        {
            if (RegDst == 1)
                Reg[r3] = ALUresult;
        else
            Reg[r2] = ALUresult;
        }
    }
}

/* PC update */
/* 10 Points */
void PC_update(unsigned jsec,unsigned extended_value,char Branch,char Jump,char Zero,unsigned *PC)
{
    //increment program counter by 4
    *PC+=4;

    if (Zero == 1 && Branch == 1)
        *PC += (extended_value<<2);
    if (Jump == 1)
        *PC = (*PC & 0b11110000000000000000000000000000) | (jsec<<2);
}


