#include <stdio.h>
#include "thumbsim.hpp"
#define PC_REG 15
#define LR_REG 14
#define SP_REG 13
#define PC rf[PC_REG]
#define LR rf[LR_REG]
#define SP rf[SP_REG]

Stats stats;
Caches caches(0);

unsigned int signExtend16to32ui(short i) {
    return static_cast<unsigned int>(static_cast<int>(i));
}

unsigned int signExtend8to32ui(char i) {
    return static_cast<unsigned int>(static_cast<int>(i));
}

ASPR flags;

int BitCount(unsigned short registers) {
    int bitCount = 0;
    for (int i = 0; i < 8; i++) {
        if (registers & 0x01 << i) {
            bitCount++;
        }
    }
    return bitCount;
}

void setNegativeZero(unsigned int result) {
    flags.N = (result & 0x01 << 31) >> 31;
    flags.Z = result == 0;
}

void setCarryOverflow (int num1, int num2, OFType oftype) {
    switch (oftype) {
        case OF_ADD:
            if (((unsigned long long int)num1 + (unsigned long long int)num2) ==
                    ((unsigned int)num1 + (unsigned int)num2)) {
                flags.C = 0;
            }
            else {
                flags.C = 1;
            }
            if (((long long int)num1 + (long long int)num2) ==
                    ((int)num1 + (int)num2)) {
                flags.V = 0;
            }
            else {
                flags.V = 1;
            }
            break;
        case OF_SUB:
            if (num1 >= num2) {
                flags.C = 1;
            }
            else if (((unsigned long long int)num1 - (unsigned long long int)num2) ==
                    ((unsigned int)num1 - (unsigned int)num2)) {
                flags.C = 0;
            }
            else {
                flags.C = 1;
            }
            if (((num1==0) && (num2==0)) ||
                    (((long long int)num1 - (long long int)num2) ==
                     ((int)num1 - (int)num2))) {
                flags.V = 0;
            }
            else {
                flags.V = 1;
            }
            break;
        case OF_SHIFT:
            // C flag unaffected for shifts by zero
            if (num2 != 0) {
                if (((unsigned long long int)num1 << (unsigned long long int)num2) ==
                        ((unsigned int)num1 << (unsigned int)num2)) {
                    flags.C = 0;
                }
                else {
                    flags.C = 1;
                }
            }
            // Shift doesn't set overflow
            break;
        default:
            cerr << "Bad OverFlow Type encountered." << __LINE__ << __FILE__ << endl;
            exit(1);
    }
}

// You're given the code for evaluating BEQ,
// and you'll need to fill in the rest of these.
// See Page 99 of the armv6 manual
static int checkCondition(unsigned short cond) {
    switch(cond) {
        case EQ:
            if (flags.Z == 1) {
                return TRUE;
            }
            break;
        case NE:
            /* Not Equal */
            if (flags.Z == 0) {
                return TRUE;
            }
            break;
        case CS:
            /* Carry Set */
            if (flags.C == 1) {
                return TRUE;
            }
            break;
        case CC:
            /* Carry Clear */
            if (flags.C == 0) {
                return TRUE;
            }
            break;
        case MI:
            /* Minus, Negative */
            if (flags.N == 1) {
                return TRUE;
            }
            break;
        case PL:
            /* Plus, Positive, or Zero */
            if (flags.N == 0) {
                return TRUE;
            }
            break;
        case VS:
            /* Overflow */
            if (flags.V == 1) {
                return TRUE;
            }
            break;
        case VC:
            /* No Overflow */
            if (flags.V == 0) {
                return TRUE;
            }
            break;
        case HI:
            /* Unsigned Higher */
            if (flags.C == 1 && flags.Z == 0) {
                return TRUE;
            }
            break;
        case LS:
            /* Unsigned Lower or Same */
            if (flags.C == 0 || flags.Z == 1) {
                return TRUE;
            }
            break;
        case GE:
            /* Signed Greater Than or Equal */
            if (flags.N == flags.V) {
                return TRUE;
            }
            break;
        case LT:
            /* Signed Less Than */
            //cout <<"\tLT" << endl;
            if (flags.N != flags.V) {
                return TRUE;
            }
            break;
        case GT:
            /* Signed Greater Than */
            if (flags.Z == 0 && flags.N == flags.V) {
                return TRUE;
            }
            break;
        case LE:
            /* Signed Less Than or Equal */
            //cout <<"\tLE" << endl;
            //printf("\tZ: %d C: %d N: %d V: %d\n", flags.Z, flags.C, flags.N, flags.V);
            if (flags.Z == 1 || flags.N != flags.V) {
                //cout <<"\tTRUE" << endl;
                return TRUE;
            }
            //cout <<"\tFALSE" << endl;
            break;
        case AL:
            /* Always */
            return TRUE;
            break;
    }
    return FALSE;
}

void execute() {
    Data16 instr = imem[PC];
    Data16 instr2;
    Data32 temp(0);
    Thumb_Types itype;
    unsigned int pctarget = PC + 2;
    unsigned int addr;
    int i, n, offset;
    unsigned int list, mask;
    int num1, num2, result, bitCount;
    unsigned int bit;

    /* Convert instruction to correct type */
    BL_Type blupper(instr);
    ALU_Type alu(instr);
    SP_Type sp(instr);
    DP_Type dp(instr);
    LD_ST_Type ld_st(instr);
    MISC_Type misc(instr);
    COND_Type cond(instr);
    UNCOND_Type uncond(instr);
    LDM_Type ldm(instr);
    STM_Type stm(instr);
    LDRL_Type ldrl(instr);
    ADD_SP_Type addsp(instr);

    BL_Ops bl_ops;
    ALU_Ops add_ops;
    DP_Ops dp_ops;
    SP_Ops sp_ops;
    LD_ST_Ops ldst_ops;
    MISC_Ops misc_ops;

    rf.write(PC_REG, pctarget);

    itype = decode(ALL_Types(instr));
    switch(itype) {
        case ALU:
            add_ops = decode(alu);
            switch(add_ops) {
                case ALU_LSLI:
                    break;
                case ALU_LSRI:
                    break;
                case ALU_ASRI:
                    break;
                case ALU_ADDR:
                    //cout <<"\tALU_ADDR" << endl;
                    num1 = rf[alu.instr.addr.rn];
                    num2 = rf[alu.instr.addr.rm];
                    result = num1 + num2;
                    setCarryOverflow(num1, num2, OF_ADD);
                    setNegativeZero(result);
                    rf.write(alu.instr.addr.rd, result);
                    break;
                case ALU_SUBR:
                    //cout <<"\tALU_SUBR" << endl;
                    num1 = rf[alu.instr.subr.rn];
                    num2 = rf[alu.instr.subr.rm];
                    result = num1 - num2;
                    setCarryOverflow(num1, num2, OF_SUB);
                    setNegativeZero(result);
                    rf.write(alu.instr.subr.rd, result);
                    break;
                case ALU_ADD3I:
                    //cout <<"\tALU_ADD3I" << endl;
                    num1 = rf[alu.instr.add3i.rn];
                    num2 = alu.instr.add3i.imm;
                    result = num1 + num2;
                    setCarryOverflow(num1, num2, OF_ADD);
                    setNegativeZero(result);
                    rf.write(alu.instr.add3i.rd, result);
                    break;
                case ALU_SUB3I:
                    //cout <<"\tALU_SUB3I" << endl;
                    num1 = rf[alu.instr.sub3i.rn];
                    num2 = alu.instr.sub3i.imm;
                    result = num1 - num2;
                    setCarryOverflow(num1, num2, OF_SUB);
                    setNegativeZero(result);
                    rf.write(alu.instr.sub3i.rd, result);
                    break;
                case ALU_MOV:
                    //cout <<"\tALU_MOV" << endl;
                    result = alu.instr.mov.imm;
                    setNegativeZero(result);
                    flags.C = flags.C;
                    rf.write(alu.instr.mov.rdn, result);
                    break;
                case ALU_CMP:
                    num1 = rf[alu.instr.cmp.rdn];
                    num2 = alu.instr.cmp.imm;
                    //cout <<"\tALU_CMP " << num1 << " vs. " << num2 << endl;
                    result = num1 - num2;
                    setCarryOverflow(num1, num2, OF_SUB);
                    setNegativeZero(result);
                    //printf("\tZ: %d C: %d N: %d V: %d\n", flags.Z, flags.C, flags.N, flags.V);
                    break;
                case ALU_ADD8I:
                    //cout <<"\tALU_ADD8I" << endl;
                    num1 = rf[alu.instr.add8i.rdn];
                    num2 = alu.instr.add8i.imm;
                    result = num1 + num2;
                    setCarryOverflow(num1, num2, OF_ADD);
                    setNegativeZero(result);
                    rf.write(alu.instr.add8i.rdn, result);
                    break;
                case ALU_SUB8I:
                    //cout <<"\tALU_SUB8I" << endl;
                    num1 = rf[alu.instr.sub8i.rdn];
                    num2 = alu.instr.sub8i.imm;
                    result = num1 - num2;
                    setCarryOverflow(num1, num2, OF_SUB);
                    setNegativeZero(result);
                    rf.write(alu.instr.sub8i.rdn, result);
                    break;
                default:
                    cout << "FUCCCKKKKKK" << endl;
                    break;
            }
            break;
        case BL:
            //cout <<"\tBL" << endl;
            bl_ops = decode(blupper);
            if (bl_ops == BL_UPPER) {
                // PC has already been incremented above
                instr2 = imem[PC];
                BL_Type bllower(instr2);
                if (blupper.instr.bl_upper.s) {
                    addr = static_cast<unsigned int>(0xff<<24) | 
                        ((~(bllower.instr.bl_lower.j1 ^ blupper.instr.bl_upper.s))<<23) |
                        ((~(bllower.instr.bl_lower.j2 ^ blupper.instr.bl_upper.s))<<22) |
                        ((blupper.instr.bl_upper.imm10)<<12) |
                        ((bllower.instr.bl_lower.imm11)<<1);
                }
                else {
                    addr = ((blupper.instr.bl_upper.imm10)<<12) |
                        ((bllower.instr.bl_lower.imm11)<<1);
                }
                // return address is 4-bytes away from the start of the BL insn
                rf.write(LR_REG, PC + 2);
                // Target address is also computed from that point
                rf.write(PC_REG, PC + 2 + addr);

                stats.numRegReads += 1;
                stats.numRegWrites += 2; 
            }
            else {
                cerr << "Bad BL format." << endl;
                exit(1);
            }
            break;
        case DP:
            decode(dp);
            break;
        case SPECIAL:
            sp_ops = decode(sp);
            switch(sp_ops) {
                case SP_MOV:
                    //cout <<"\tSP_MOV" << endl;
                    result = rf[sp.instr.mov.rm];
                    setNegativeZero(result);
                    if (sp.instr.mov.d) {
                        rf.write(SP_REG, rf[sp.instr.mov.rm]);
                    }
                    else {
                        rf.write(sp.instr.mov.rd, rf[sp.instr.mov.rm]);
                    }
                    break;
            }
            break;
        case LD_ST:
            // You'll want to use these load and store models
            // to implement ldrb/strb, ldm/stm and push/pop
            ldst_ops = decode(ld_st);
            switch(ldst_ops) {
                case STRR:
                    //cout <<"\tSTRR" << endl;
                    addr = rf[ld_st.instr.ld_st_imm.rn] + ld_st.instr.ld_st_imm.imm * 4;
                    dmem.write(addr, rf[ld_st.instr.ld_st_imm.rt]);
                    //printf("\tD[%d]:%d\n", addr, dmem[addr]);
                    break;
                case LDRR:
                    //cout <<"\tLDRR" << endl;
                    addr = rf[ld_st.instr.ld_st_imm.rn] + ld_st.instr.ld_st_imm.imm * 4;
                    rf.write(ld_st.instr.ld_st_imm.rt, dmem[addr]);
                    //printf("\tR[%d]:%d\n", ld_st.instr.ld_st_imm.rt, rf[ld_st.instr.ld_st_imm.rt]);
                    break;
                default:
                    //cout <<"\tldst_ops: default" << endl;
                    break;
            }
            break;
        case MISC:
            misc_ops = decode(misc);
            switch(misc_ops) {
                case MISC_PUSH:
                    //cout <<"\tMISC_PUSH" << endl;
                    {
                        addr = SP - 4 * BitCount(misc.instr.push.reg_list);
                        for (int i = 0; i < 8; i++) {
                            if (misc.instr.push.reg_list & 0x01 << i) {
                                dmem.write(addr, rf[i]);
                                addr += 4;
                            }
                        }
                        if (misc.instr.push.m) {
                            dmem.write(addr, PC);
                            addr += 4;
                        }
                        rf.write(SP_REG, SP - 4 * BitCount(misc.instr.push.reg_list));
                    }
                    break;
                case MISC_POP:
                    //cout <<"\tMISC_POP" << endl;
                    {
                        addr = SP;
                        for (int i = 0; i < 8; i++) {
                            if (misc.instr.pop.reg_list & 0x01 << i) {
                                rf.write(i, dmem[addr]);
                                addr += 4;
                            }
                        }
                        if (misc.instr.pop.m) {
                            rf.write(PC_REG, 2*(int)((char)(dmem[addr]))+2);
                        }
                        rf.write(SP_REG, SP + 4 * BitCount(misc.instr.pop.reg_list));
                    }
                    break;
                case MISC_SUB:
                    //cout <<"\tMISC_SUB" << endl;
                    rf.write(SP_REG, SP - (misc.instr.sub.imm * 4));
                    break;
                case MISC_ADD:
                    //cout <<"\tMISC_ADD" << endl;
                    rf.write(SP_REG, SP + (misc.instr.add.imm * 4));
                    break;
            }
            break;
        case COND:
            decode(cond);
            //cout <<"\tCOND" << endl;
            // Once you've completed the checkCondition function,
            // this should work for all your conditional branches.
            if (checkCondition(cond.instr.b.cond)){
                rf.write(PC_REG, PC + 2 * signExtend8to32ui(cond.instr.b.imm) + 2);
            }
            break;
        case UNCOND:
            decode(uncond);
            //cout <<"\tUNCOND imm:" << uncond.instr.b.imm << endl;
            rf.write(PC_REG, (PC + 2*(int)((char)(uncond.instr.b.imm))+2));
            break;
        case LDM:
            decode(ldm);
            //cout <<"\tLDM" << endl;
            break;
        case STM:
            decode(stm);
            //cout <<"\tSTM" << endl;
            break;
        case LDRL:
            //cout <<"\tLDRL" << endl;
            decode(ldrl);
            // Need to check for alignment by 4
            if (PC & 2) {
                addr = PC + 2 + (ldrl.instr.ldrl.imm)*4;
            }
            else {
                addr = PC + (ldrl.instr.ldrl.imm)*4;
            }
            // Requires two consecutive imem locations pieced together
            temp = imem[addr] | (imem[addr+2]<<16);  // temp is a Data32
            rf.write(ldrl.instr.ldrl.rt, temp);

            // One write for updated reg
            stats.numRegWrites++;
            // One read of the PC
            stats.numRegReads++;
            // One mem read, even though it's imem, and there's two of them
            stats.numMemReads++;
            break;
        case ADD_SP:
            decode(addsp);
            //cout <<"\tADD_SP" << endl;
            rf.write(addsp.instr.add.rd, SP + (addsp.instr.add.imm*4));
            break;
        default:
            cout << "[ERROR] Unknown Instruction to be executed" << endl;
            cout << "itype: 0x" << hex << itype << endl;
            exit(1);
            break;
    }
}
