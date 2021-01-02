/* ********************************************
 *	COSE222 Lab #3
 *
 *	Module: top design of the single-cycle CPU (single_cycle_cpu.sv)
 *  - Top design of the single-cycle CPU
 *
 *  Author: Gunjae Koo (gunjaekoo@korea.ac.kr)
 *
 * ********************************************
 */

`timescale 1ns/1ps
`define FF 1    // Flip-flop delay for just better waveform view

module single_cycle_cpu
#(  parameter IMEM_DEPTH = 1024,    // imem depth (default: 1024 entries = 4 KB)
              IMEM_ADDR_WIDTH = 10,
              REG_WIDTH = 64,
              DMEM_DEPTH = 1024,    // dmem depth (default: 1024 entries = 8 KB)
              DMEM_ADDR_WIDTH = 10 )
(
    input           clk,            // System clock
    input           reset_b         // Asychronous negative reset
);

    // Wires for datapath elements
    logic   [IMEM_ADDR_WIDTH-1:0]   imem_addr;
    logic   [31:0]  inst;   // instructions = an output of ???? --> imem

    logic   [4:0]   rs1, rs2, rd;
    logic   [REG_WIDTH-1:0] rd_din;
    logic           reg_write;
    logic   [REG_WIDTH-1:0] rs1_dout, rs2_dout;

    logic   [REG_WIDTH-1:0] alu_in1, alu_in2;
    logic   [3:0]   alu_control;
    logic   [REG_WIDTH-1:0] alu_result;
    logic           alu_zero;

    logic   [DMEM_ADDR_WIDTH-1:0]    dmem_addr;
    logic   [63:0]  dmem_din, dmem_dout;
    logic           mem_read, mem_write;

    // -------------------------------------------------------------------
    /* Main control unit:
     * Main control unit generates """"control signals!!!""""" for datapath elements
     * The control signals are determined by decoding instructions
     * Generating control signals using opcode = inst[6:0]
     */
    logic   [6:0]   opcode;
    logic           branch, alu_src, mem_to_reg;
    logic   [1:0]   alu_op;
    //logic         mem_read, mem_write, reg_write; // declared above
    // COMPLETE THE MAIN CONTROL UNIT HERE
    always_comb begin
        if(opcode == 7'b0000011) begin //ld instruction
            alu_src = 1; mem_to_reg = 1; reg_write = 1; mem_read = 1;
            mem_write = 0; branch = 0; alu_op = 2'b00;
        end
        else if(opcode == 7'b0100011)begin //sd instruction
            alu_src = 1; mem_to_reg = 0; reg_write = 0; mem_read = 0;
            //mem_to_reg is "X"(don't care)
            mem_write = 1; branch = 0; alu_op = 2'b00;
        end
        else if(opcode == 7'b0110011) begin //R-format
            alu_src = 0; mem_to_reg = 0; reg_write = 1; mem_read = 0;
            mem_write = 0; branch = 0; alu_op = 2'b10;    
        end
        else if(opcode == 7'b1100011) begin //beg instruction
            alu_src = 0; mem_to_reg = 0; reg_write = 0; mem_read = 0;
            //mem_to_reg is "X"(don't care)
            mem_write = 0; branch = 1; alu_op = 2'b01;
        end
    end
    // --------------------------------------------------------------------

    // --------------------------------------------------------------------
    /* ALU control unit:
     * ALU control unit generate alu_control signal which selects ALU operations
     * Generating control signals using alu_op, funct7, and funct3 fileds
     */
    logic   [6:0]   funct7;
    logic   [2:0]   funct3;
    // COMPLETE THE ALU CONTROL UNIT HERE
    always_comb begin
        if(alu_op == 2'b00) alu_control = 4'b0010;
        else if(alu_op[0] == 1) alu_control = 4'b0110;
        else if(alu_op[1] == 1) begin
            if(funct7 == 7'b0000000 && funct3 == 3'b000) alu_control <= 4'b0010;
            else if(funct7 == 7'b0100000 && funct3 == 3'b000) alu_control <= 4'b0110;
            else if(funct7 == 7'b0000000 && funct3 == 3'b111) alu_control <= 4'b0000;
            else if(funct7 == 7'b0000000 && funct3 == 3'b110) alu_control <= 4'b0001;
        end
    end
    // ---------------------------------------------------------------------

    // ---------------------------------------------------------------------
    /* Immediate generator:
     * Generating immediate value from inst[31:0]
     */
    logic   [63:0]  imm64;
    logic   [63:0]  imm64_branch;  // imm64 left shifted by 1
    logic   [11:0]  imm12;  // 12-bit immediate value extracted from inst
    // COMPLETE IMMEDIATE GENERATOR HERE
    //using inst[32], make imm12[12] --> by imm12[12] ~~~~
    always_comb begin
        if(opcode == 7'b0000011) begin //ld instruction
            //imm12 gen
            imm12 = inst[31:20];
            //64bit
            imm64[63:12] = imm12[11];
            imm64[11:0] = imm12;
        end
        else if(opcode == 7'b0100011)begin //sd instruction
            //imm12 gen
            imm12[11:5] = inst[31:25];
            imm12[4:0] = inst[11:7];
            //64bit
            imm64[63:12] = imm12[11];
            imm64[11:0] = imm12;
        end
        else if(opcode == 7'b1100111) begin //beg instruction
            //imm12 gen
            imm12[11] = inst[31];
            imm12[10] = inst[7];
            imm12[9:4] = inst[30:25];
            imm12[3:0] = inst[11:8];
            //64bit
            imm64[63:12] = imm12[11];
            imm64[11:0] = imm12;
            //left shift
            imm64_branch = imm64 << 1;
        end
    end
    // ----------------------------------------------------------------------

    // ----------------------------------------------------------------------
    // Program counter
    logic   [63:0]  pc_curr, pc_next;
    logic   [63:0]  pc_next_plus4, pc_next_branch;
    
    assign pc_next_plus4 = pc_curr + 4;    // FILL THIS "origin: assign pc_next_plus4 = ;"
    always_ff @ (posedge clk or negedge reset_b) begin
        if (~reset_b) begin //when the "reset_b" is 0, again, when reset
            pc_curr <= 'b0;
        end
        else begin //of course, this is by clk
            pc_curr <= pc_next;        // FILL THIS "origin: pc_curr <= ;"
        end
    end
    // ----------------------------------------------------------------------

    // ----------------------------------------------------------------------
    // MUXes: 1.PC_NEXT, 2.ALU inputs, 3.Register File din  
    // !!COMPLETE MUXES HERE!!
    // 1.PC_NEXT
    assign pc_next_branch = pc_curr + imm64_branch;   // FILL THIS "origin: assign pc_next_branch = ;"
    always_comb begin
        if(branch && alu_zero)  pc_next = pc_next_branch;
        else                    pc_next = pc_next_plus4;
    end
    // 2.ALU inputs
    always_comb begin
        if(alu_src)             alu_in2 = imm64;
        else                    alu_in2 = rs2_dout;
    end
    // 3.RF din
    always_comb begin
        if(mem_to_reg)          rd_din <= dmem_dout;
        else                    rd_din <= alu_result;
    end
    // ----------------------------------------------------------------------

    // ----------------------------------------------------------------------
    // COMPLETE CONNECTIONS HERE
    // imem!!
    assign imem_addr = pc_curr >> 2;
    assign opcode = inst[6:0];
    assign rs1 = inst[19:15];
    assign rs2 = inst[24:20];
    assign rd = inst[11:7];
    assign funct3 = inst[14:12];
    assign funct7 = inst[31:25];
    //assign inst = inst;
    //din is dealed with MUX
    
    // regfile!!
    assign alu_in1 = rs1_dout;
    //alu_in2 is dealed with MUX
    assign dmem_din = rs2_dout;

    // dmem
    assign dmem_addr = alu_result >> 3; // the unit is 8, but the array's unit is 1
    // ----------------------------------------------------------------------

    // -----------------------------------------------------------------------
    /* Instantiation of datapath elements
     * All input/output ports should be connected
     */
    // IMEM
    // .module_name         (this_name)
    imem #(
        .IMEM_DEPTH         (IMEM_DEPTH),
        .IMEM_ADDR_WIDTH    (IMEM_ADDR_WIDTH)
    ) u_imem_0 (
        .addr               (imem_addr),
        .dout               (inst)
    );
    // REGFILE
    regfile #(
        .REG_WIDTH          (REG_WIDTH)
    ) u_regfile_0 (
        .clk                (clk),
        .rs1                (rs1),
        .rs2                (rs2),
        .rd                 (rd),
        .rd_din             (rd_din),
        .reg_write          (reg_write),
        .rs1_dout           (rs1_dout),
        .rs2_dout           (rs2_dout)
    );
    // ALU
    alu #(
        .REG_WIDTH          (REG_WIDTH)
    ) u_alu_0 (
        .in1                (alu_in1),
        .in2                (alu_in2),
        .alu_control        (alu_control),
        .result             (alu_result),
        .zero               (alu_zero)
    );
    // DMEM
    dmem #(
        .DMEM_DEPTH         (DMEM_DEPTH),
        .DMEM_ADDR_WIDTH    (DMEM_ADDR_WIDTH)
    ) u_dmem_0 (
        .clk                (clk),
        .addr               (dmem_addr),
        .din                (dmem_din),
        .mem_read           (mem_read),
        .mem_write          (mem_write),
        .dout               (dmem_dout)
    );
    // ----------------------------------------------------------------------
endmodule