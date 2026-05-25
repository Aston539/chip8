#include <cstdio>

#include <decoder/decoder.h>
#include <disassembler/disassembler.h>

STATIC
BOOL
Chip8IsValidRegister(
    _In_ BYTE Register
)
{
    return Register >= 0 && Register < 16;
}

STATIC
BOOL
Chip8InitializeRegisterOperand(
    _Inout_ CHIP8_OPERAND* Operand,
    _In_ BYTE Register
)
{
    if ( Operand == NULL || Chip8IsValidRegister( Register ) == FALSE )
    {
        return FALSE;
    }

    Operand->Type = CHIP8_OPERAND_TYPE_REGISTER;
    Operand->Register = Register;
    Operand->Size = 8;

    return TRUE;
}

STATIC
BOOL
Chip8InitializeImmediateOperand(
    _Inout_ CHIP8_OPERAND* Operand,
    _In_ BYTE Immediate,
    _In_ BYTE Size
)
{
    if ( Operand == NULL )
    {
        return FALSE;
    }

    Operand->Type = CHIP8_OPERAND_TYPE_IMMEDIATE;
    Operand->Immediate = Immediate;
    Operand->Size = Size;

    return TRUE;
}

STATIC
BOOL
Chip8InitializeAddressOperand(
    _Inout_ CHIP8_OPERAND* Operand,
    _In_ UINT16 Address
)
{
    if ( Operand == NULL )
    {
        return FALSE;
    }

    Operand->Type = CHIP8_OPERAND_TYPE_ADDRESS;
    Operand->Address = Address;
    Operand->Size = 12;

    return TRUE;
}

BOOL
Chip8DisassembleInstruction(
    _In_ CONST UINT16* CONST Instruction,
    _Inout_ CHIP8_INSTRUCTION* DisassembledInstruction
)
{
    CHIP8_DECODED_INSTRUCTION Decoded = { };
    if ( Chip8DecodeInstruction( Instruction, &Decoded ) == FALSE )
    {
        return FALSE;
    }

    //
    // mnemonic lookup
    //
    switch ( Decoded.OpcodeKey )
    {
        case CHIP8_OPCODE_KEY_SPECIAL_CLS: DisassembledInstruction->Mnemonic = CHIP8_MNEMONIC_CLS; break;
        case CHIP8_OPCODE_KEY_SPECIAL_RET: DisassembledInstruction->Mnemonic = CHIP8_MNEMONIC_RET; break;

        case CHIP8_OPCODE_KEY_ALU_MOV: DisassembledInstruction->Mnemonic = CHIP8_MNEMONIC_MOV; break;
        case CHIP8_OPCODE_KEY_ALU_OR: DisassembledInstruction->Mnemonic = CHIP8_MNEMONIC_OR; break;
        case CHIP8_OPCODE_KEY_ALU_AND: DisassembledInstruction->Mnemonic = CHIP8_MNEMONIC_AND; break;
        case CHIP8_OPCODE_KEY_ALU_XOR: DisassembledInstruction->Mnemonic = CHIP8_MNEMONIC_XOR; break;
        case CHIP8_OPCODE_KEY_ALU_ADD: DisassembledInstruction->Mnemonic = CHIP8_MNEMONIC_ADD; break;
        case CHIP8_OPCODE_KEY_ALU_SUB: DisassembledInstruction->Mnemonic = CHIP8_MNEMONIC_SUB; break;
        case CHIP8_OPCODE_KEY_ALU_SHR: DisassembledInstruction->Mnemonic = CHIP8_MNEMONIC_SHR; break;
        case CHIP8_OPCODE_KEY_ALU_SUBN: DisassembledInstruction->Mnemonic = CHIP8_MNEMONIC_SUBN; break;
        case CHIP8_OPCODE_KEY_ALU_SHL: DisassembledInstruction->Mnemonic = CHIP8_MNEMONIC_SHL; break;

        case CHIP8_OPCODE_KEY_INPUT_SKP: DisassembledInstruction->Mnemonic = CHIP8_MNEMONIC_SKP; break;
        case CHIP8_OPCODE_KEY_INPUT_SKNP: DisassembledInstruction->Mnemonic = CHIP8_MNEMONIC_SKNP; break;

        case CHIP8_OPCODE_KEY_SYSTEM_GET_DELAY_TIMER: DisassembledInstruction->Mnemonic = CHIP8_MNEMONIC_MOV; break;
        case CHIP8_OPCODE_KEY_SYSTEM_WAIT_KEY_CLICK: DisassembledInstruction->Mnemonic = CHIP8_MNEMONIC_HLT; break;
        case CHIP8_OPCODE_KEY_SYSTEM_SET_DELAY_TIMER: DisassembledInstruction->Mnemonic = CHIP8_MNEMONIC_MOV; break;
        case CHIP8_OPCODE_KEY_SYSTEM_SET_SOUND_TIMER: DisassembledInstruction->Mnemonic = CHIP8_MNEMONIC_MOV; break;
        case CHIP8_OPCODE_KEY_SYSTEM_ADD_IDX: DisassembledInstruction->Mnemonic = CHIP8_MNEMONIC_ADD; break;
        case CHIP8_OPCODE_KEY_SYSTEM_SET_SPRITE: DisassembledInstruction->Mnemonic = CHIP8_MNEMONIC_MOV; break;
        case CHIP8_OPCODE_KEY_SYSTEM_STORE_BCD: DisassembledInstruction->Mnemonic = CHIP8_MNEMONIC_MOV; break;
        case CHIP8_OPCODE_KEY_SYSTEM_STORE_CONTEXT: DisassembledInstruction->Mnemonic = CHIP8_MNEMONIC_VMSAVE; break;
        case CHIP8_OPCODE_KEY_SYSTEM_LOAD_CONTEXT: DisassembledInstruction->Mnemonic = CHIP8_MNEMONIC_VMLOAD; break;

        case CHIP8_OPCODE_KEY_JMP: DisassembledInstruction->Mnemonic = CHIP8_MNEMONIC_JMP; break;
        case CHIP8_OPCODE_KEY_CALL: DisassembledInstruction->Mnemonic = CHIP8_MNEMONIC_CALL; break;
        case CHIP8_OPCODE_KEY_SE_VX_IMM: DisassembledInstruction->Mnemonic = CHIP8_MNEMONIC_SE; break;
        case CHIP8_OPCODE_KEY_SNE_VX_IMM: DisassembledInstruction->Mnemonic = CHIP8_MNEMONIC_SNE; break;
        case CHIP8_OPCODE_KEY_SE_VX_VY: DisassembledInstruction->Mnemonic = CHIP8_MNEMONIC_SE; break;
        case CHIP8_OPCODE_KEY_SET_VX_IMM: DisassembledInstruction->Mnemonic = CHIP8_MNEMONIC_MOV; break;
        case CHIP8_OPCODE_KEY_ADD_VX_IMM: DisassembledInstruction->Mnemonic = CHIP8_MNEMONIC_ADD; break;
        case CHIP8_OPCODE_KEY_SNE_VX_VY: DisassembledInstruction->Mnemonic = CHIP8_MNEMONIC_SNE; break;
        case CHIP8_OPCODE_KEY_DRAW_VX_VY_N: DisassembledInstruction->Mnemonic = CHIP8_MNEMONIC_DRAW; break;
        case CHIP8_OPCODE_KEY_SET_IDX: DisassembledInstruction->Mnemonic = CHIP8_MNEMONIC_MOV; break;
        case CHIP8_OPCODE_KEY_JMP_REL: DisassembledInstruction->Mnemonic = CHIP8_MNEMONIC_JMP; break;
        case CHIP8_OPCODE_KEY_RAND: DisassembledInstruction->Mnemonic = CHIP8_MNEMONIC_RAND; break;

        default: return FALSE;
    }

    //
    // operand lookup
    //
    switch ( Decoded.OpcodeKey )
    {
        //
        // none
        //
        case CHIP8_OPCODE_KEY_SPECIAL_CLS:
        case CHIP8_OPCODE_KEY_SPECIAL_RET:
            break;

        //
        // register, register
        //
        case CHIP8_OPCODE_KEY_ALU_MOV: 
        case CHIP8_OPCODE_KEY_ALU_OR:  
        case CHIP8_OPCODE_KEY_ALU_AND: 
        case CHIP8_OPCODE_KEY_ALU_XOR: 
        case CHIP8_OPCODE_KEY_ALU_ADD: 
        case CHIP8_OPCODE_KEY_ALU_SUB: 
        case CHIP8_OPCODE_KEY_ALU_SHR: 
        case CHIP8_OPCODE_KEY_ALU_SUBN:
        case CHIP8_OPCODE_KEY_ALU_SHL:
        case CHIP8_OPCODE_KEY_SE_VX_VY:
        case CHIP8_OPCODE_KEY_SNE_VX_VY:
        {
            DisassembledInstruction->OperandsCount = 2;
            Chip8InitializeRegisterOperand( &DisassembledInstruction->Operands[ 0 ], Decoded.RegisterX );
            Chip8InitializeRegisterOperand( &DisassembledInstruction->Operands[ 1 ], Decoded.RegisterY );

        } break;

        //
        // register
        //
        case CHIP8_OPCODE_KEY_INPUT_SKP:  
        case CHIP8_OPCODE_KEY_INPUT_SKNP:
        case CHIP8_OPCODE_KEY_SYSTEM_WAIT_KEY_CLICK:
        case CHIP8_OPCODE_KEY_SYSTEM_STORE_CONTEXT:
        case CHIP8_OPCODE_KEY_SYSTEM_LOAD_CONTEXT:
        {
            DisassembledInstruction->OperandsCount = 1;
            Chip8InitializeRegisterOperand( &DisassembledInstruction->Operands[ 0 ], Decoded.RegisterX );

        } break;

        //
        // register, immediate
        //
        case CHIP8_OPCODE_KEY_SE_VX_IMM: 
        case CHIP8_OPCODE_KEY_SNE_VX_IMM:
        case CHIP8_OPCODE_KEY_SET_VX_IMM:
        case CHIP8_OPCODE_KEY_ADD_VX_IMM:
        case CHIP8_OPCODE_KEY_RAND:
        {
            DisassembledInstruction->OperandsCount = 2;
            Chip8InitializeRegisterOperand( &DisassembledInstruction->Operands[ 0 ], Decoded.RegisterX );
            Chip8InitializeImmediateOperand( &DisassembledInstruction->Operands[ 1 ], Decoded.Immediate, 8 );

        } break;

        //
        // address
        //
        case CHIP8_OPCODE_KEY_JMP:
        case CHIP8_OPCODE_KEY_CALL:
        {
            DisassembledInstruction->OperandsCount = 1;
            Chip8InitializeAddressOperand( &DisassembledInstruction->Operands[ 0 ], Decoded.Address );

        } break;

        //
        // relative address
        //
        case CHIP8_OPCODE_KEY_JMP_REL:
        {
            DisassembledInstruction->OperandsCount = 1;
            Chip8InitializeAddressOperand( &DisassembledInstruction->Operands[ 0 ], Decoded.Address );

            DisassembledInstruction->Operands[ 0 ].Flags |= CHIP8_OPERAND_FLAG_RELATIVE_R0;

        } break;

        case CHIP8_OPCODE_KEY_SYSTEM_GET_DELAY_TIMER:
        {
            DisassembledInstruction->OperandsCount = 2;

            Chip8InitializeRegisterOperand( &DisassembledInstruction->Operands[ 0 ], Decoded.RegisterX );

            DisassembledInstruction->Operands[ 1 ].Type = CHIP8_OPERAND_TYPE_DELAY_TIMER;
            DisassembledInstruction->Operands[ 1 ].Size = 8;

        } break;

        case CHIP8_OPCODE_KEY_SYSTEM_SET_DELAY_TIMER:
        {
            DisassembledInstruction->OperandsCount = 2;

            DisassembledInstruction->Operands[ 0 ].Type = CHIP8_OPERAND_TYPE_DELAY_TIMER;
            DisassembledInstruction->Operands[ 0 ].Size = 8;

            Chip8InitializeRegisterOperand( &DisassembledInstruction->Operands[ 1 ], Decoded.RegisterX );

        } break;

        case CHIP8_OPCODE_KEY_SYSTEM_SET_SOUND_TIMER:
        {
            DisassembledInstruction->OperandsCount = 2;

            DisassembledInstruction->Operands[ 0 ].Type = CHIP8_OPERAND_TYPE_SOUND_TIMER;
            DisassembledInstruction->Operands[ 0 ].Size = 8;

            Chip8InitializeRegisterOperand( &DisassembledInstruction->Operands[ 1 ], Decoded.RegisterX );

        } break;

        case CHIP8_OPCODE_KEY_SYSTEM_ADD_IDX:
        {
            DisassembledInstruction->OperandsCount = 2;

            DisassembledInstruction->Operands[ 0 ].Type = CHIP8_OPERAND_TYPE_MEMORY_INDEX;
            DisassembledInstruction->Operands[ 0 ].Size = 8;

            Chip8InitializeRegisterOperand( &DisassembledInstruction->Operands[ 1 ], Decoded.RegisterX );

        } break;

        case CHIP8_OPCODE_KEY_SET_IDX:
        {
            DisassembledInstruction->OperandsCount = 2;

            DisassembledInstruction->Operands[ 0 ].Type = CHIP8_OPERAND_TYPE_MEMORY_INDEX;
            DisassembledInstruction->Operands[ 0 ].Size = 8;

            Chip8InitializeAddressOperand( &DisassembledInstruction->Operands[ 1 ], Decoded.Address );

        } break;

        case CHIP8_OPCODE_KEY_SYSTEM_SET_SPRITE:
        {
            DisassembledInstruction->OperandsCount = 2;

            DisassembledInstruction->Operands[ 0 ].Type = CHIP8_OPERAND_TYPE_MEMORY_INDEX;
            DisassembledInstruction->Operands[ 0 ].Size = 8;

            Chip8InitializeRegisterOperand( &DisassembledInstruction->Operands[ 1 ], Decoded.RegisterX );
            DisassembledInstruction->Operands[ 1 ].Flags |= CHIP8_OPERAND_FLAG_SPRITE_INDEX;

        } break;

        case CHIP8_OPCODE_KEY_SYSTEM_STORE_BCD:
        {
            DisassembledInstruction->OperandsCount = 2;

            DisassembledInstruction->Operands[ 0 ].Type = CHIP8_OPERAND_TYPE_MEMORY_INDEX;
            DisassembledInstruction->Operands[ 0 ].Size = 8 * 3;
            DisassembledInstruction->Operands[ 0 ].Flags |= CHIP8_OPERAND_FLAG_MEMORY_ACCESS;

            Chip8InitializeRegisterOperand( &DisassembledInstruction->Operands[ 1 ], Decoded.RegisterX );
            DisassembledInstruction->Operands[ 1 ].Flags |= CHIP8_OPERAND_FLAG_BINARY_CODED_DECIMAL;

        } break;

        case CHIP8_OPCODE_KEY_DRAW_VX_VY_N:
        {
            DisassembledInstruction->OperandsCount = 3;
            Chip8InitializeRegisterOperand( &DisassembledInstruction->Operands[ 0 ], Decoded.RegisterX );
            Chip8InitializeRegisterOperand( &DisassembledInstruction->Operands[ 1 ], Decoded.RegisterY );
            Chip8InitializeImmediateOperand( &DisassembledInstruction->Operands[ 2 ], Decoded.Immediate, 4 );

        } break;

        default: return FALSE;
    }

    return TRUE;
}

BOOL
Chip8DisassembleProgramBasicBlock(
    _In_ BYTE* ProgramSpace, // BYTE[ 4096 ]
    _Inout_ CHIP8_FUNCTION* Function,
    _In_ UINT16 BasicBlockAddress
)
{
    if ( Chip8FunctionLookupBasicBlock( Function, BasicBlockAddress, NULL ) )
    {
        return FALSE;
    }

    CHIP8_BASIC_BLOCK* BasicBlock = Chip8BasicBlockCreate( );
    BasicBlock->Address = BasicBlockAddress;

    UINT16 ProgramCounter = BasicBlock->Address;
    while ( ProgramCounter )
    {
        CHIP8_CONTROL_FLOW_NODE* CollidingNode = NULL;
        if ( Chip8ControlFlowGraphLookupNodeByAddress( Function->ControlFlowGraph, ProgramCounter, &CollidingNode ) )
        {
            if ( CollidingNode->Address != BasicBlockAddress )
            {
                Chip8FunctionAddBasicBlock( Function, BasicBlock );

                return TRUE;
            }
        }

        CHIP8_INSTRUCTION* Instruction = Chip8CreateInstruction( );
        if ( Chip8DisassembleInstruction( ( CONST UINT16 CONST* )( ProgramSpace + ProgramCounter ), Instruction ) == FALSE )
        {
            return FALSE;
        }

        switch ( Instruction->Mnemonic )
        {
            //
            // record instruction, end block
            //
            case CHIP8_MNEMONIC_RET:
            case CHIP8_MNEMONIC_JMP:
            case CHIP8_MNEMONIC_SE:
            case CHIP8_MNEMONIC_SNE:
            case CHIP8_MNEMONIC_SKP:
            case CHIP8_MNEMONIC_SKNP:
            {
                Chip8BasicBlockAddInstruction( BasicBlock, Instruction );

                ProgramCounter = NULL;

            } break;

            //
            // record instruction, advance program counter
            //
            default:
            {
                Chip8BasicBlockAddInstruction( BasicBlock, Instruction );

                ProgramCounter += sizeof( CHIP8_ENCODED_INSTRUCTION );

            } break;
        }
    }

    Chip8FunctionAddBasicBlock( Function, BasicBlock );

    return TRUE;
}

VOID
Chip8DiscoverBasicBlocks(
    _In_ BYTE* ProgramSpace,
    _Inout_ CHIP8_FUNCTION* Function,
    _In_ UINT16 BasicBlockAddress
)
{
    CHIP8_CONTROL_FLOW_NODE* CurrentBlockNode = Chip8ControlFlowNodeCreate( Function->ControlFlowGraph );
    CurrentBlockNode->Address = BasicBlockAddress;

    UINT16 ProgramCounter = BasicBlockAddress;
    while ( ProgramCounter )
    {
        CHIP8_INSTRUCTION Instruction = { };
        if ( Chip8DisassembleInstruction( ( CONST UINT16 CONST* )( ProgramSpace + ProgramCounter ), &Instruction ) == FALSE )
        {
            return;
        }

        switch ( Instruction.Mnemonic )
        {
            case CHIP8_MNEMONIC_RET:
            {
                //
                // record the current block
                //
                Chip8ControlFlowGraphAddControlFlowNode( Function->ControlFlowGraph, CurrentBlockNode );

                return;

            } break;

            //
            // record instruction, follow new block, end current block
            //
            case CHIP8_MNEMONIC_JMP:
            {
                if ( Instruction.Operands[ 0 ].Flags & CHIP8_OPERAND_FLAG_RELATIVE_R0 )
                {
                    ProgramCounter += sizeof( CHIP8_ENCODED_INSTRUCTION );

                    break;
                }

                //
                // record the current block
                //
                Chip8ControlFlowGraphAddControlFlowNode( Function->ControlFlowGraph, CurrentBlockNode );

                //
                // check whether we have already discovered this block, if not, discover it
                //
                CHIP8_CONTROL_FLOW_NODE* SuccessorNode = NULL;
                if ( Chip8ControlFlowGraphLookupNodeByAddress( Function->ControlFlowGraph, Instruction.Operands[ 0 ].Address, &SuccessorNode ) == FALSE )
                {
                    Chip8DiscoverBasicBlocks( ProgramSpace, Function, Instruction.Operands[ 0 ].Address );

                    if ( Chip8ControlFlowGraphLookupNodeByAddress( Function->ControlFlowGraph, Instruction.Operands[ 0 ].Address, &SuccessorNode ) )
                    {
                        //
                        // create successor and predecessor links between our two nodes
                        //
                        Chip8ControlFlowNodeAddSuccessor( CurrentBlockNode, SuccessorNode->Address );
                        Chip8ControlFlowNodeAddPredecessor( SuccessorNode, CurrentBlockNode->Address );
                    }
                }

                return;

            } break;

            //
            // record instruction, follow new blocks, end current block
            //
            case CHIP8_MNEMONIC_SE:
            case CHIP8_MNEMONIC_SNE:
            case CHIP8_MNEMONIC_SKP:
            case CHIP8_MNEMONIC_SKNP:
            {
                //
                // record the current block
                //
                Chip8ControlFlowGraphAddControlFlowNode( Function->ControlFlowGraph, CurrentBlockNode );

                //
                // check whether we have already discovered this block, if not, discover it
                //
                CHIP8_CONTROL_FLOW_NODE* SuccessNode = NULL;
                if ( Chip8ControlFlowGraphLookupNodeByAddress( Function->ControlFlowGraph, ProgramCounter + ( sizeof( CHIP8_ENCODED_INSTRUCTION ) * 1 ), &SuccessNode ) == FALSE )
                {
                    Chip8DiscoverBasicBlocks( ProgramSpace, Function, ProgramCounter + ( sizeof( CHIP8_ENCODED_INSTRUCTION ) * 1 ) );

                    if ( Chip8ControlFlowGraphLookupNodeByAddress( Function->ControlFlowGraph, ProgramCounter + ( sizeof( CHIP8_ENCODED_INSTRUCTION ) * 1 ), &SuccessNode ) )
                    {
                        //
                        // create successor and predecessor links between our two nodes
                        //
                        Chip8ControlFlowNodeAddSuccessor( CurrentBlockNode, SuccessNode->Address );
                        Chip8ControlFlowNodeAddPredecessor( SuccessNode, CurrentBlockNode->Address );
                    }
                }

                //
                // check whether we have already discovered this block, if not, discover it
                //
                CHIP8_CONTROL_FLOW_NODE* FailureNode = NULL;
                if ( Chip8ControlFlowGraphLookupNodeByAddress( Function->ControlFlowGraph, ProgramCounter + ( sizeof( CHIP8_ENCODED_INSTRUCTION ) * 2 ), &FailureNode ) == FALSE )
                {
                    Chip8DiscoverBasicBlocks( ProgramSpace, Function, ProgramCounter + ( sizeof( CHIP8_ENCODED_INSTRUCTION ) * 2 ) );

                    if ( Chip8ControlFlowGraphLookupNodeByAddress( Function->ControlFlowGraph, ProgramCounter + ( sizeof( CHIP8_ENCODED_INSTRUCTION ) * 2 ), &FailureNode ) )
                    {
                        //
                        // create successor and predecessor links between our two nodes
                        //
                        Chip8ControlFlowNodeAddSuccessor( CurrentBlockNode, FailureNode->Address );
                        Chip8ControlFlowNodeAddPredecessor( FailureNode, CurrentBlockNode->Address );
                    }
                }

                return;

            } break;

            //
            // record instruction, advance program counter
            //
            default:
            {
                ProgramCounter += sizeof( CHIP8_ENCODED_INSTRUCTION );

            } break;
        }
    }
}

VOID
Chip8DiscoverFunctionBasicBlocks(
    _In_ BYTE* ProgramSpace,
    _Inout_ CHIP8_FUNCTION* Function
)
{
    return Chip8DiscoverBasicBlocks( ProgramSpace, Function, Function->Address );
}

STATIC
LONG32
Chip8CompareBasicBlocks(
    VOID CONST* A,
    VOID CONST* B
)
{
    CONST CHIP8_BASIC_BLOCK CONST* BlockA = *( CONST CHIP8_BASIC_BLOCK** )A;
    CONST CHIP8_BASIC_BLOCK CONST* BlockB = *( CONST CHIP8_BASIC_BLOCK** )B;

    return BlockA->Address - BlockB->Address;
}

STATIC
LONG32
Chip8CompareControlFlowNodes(
    VOID CONST* A,
    VOID CONST* B
)
{
    CONST CHIP8_CONTROL_FLOW_NODE CONST* NodeA = *( CONST CHIP8_CONTROL_FLOW_NODE** )A;
    CONST CHIP8_CONTROL_FLOW_NODE CONST* NodeB = *( CONST CHIP8_CONTROL_FLOW_NODE** )B;

    return NodeA->Address - NodeB->Address;
}

BOOL
Chip8DisassembleProgramFunction(
    _In_ BYTE* ProgramSpace,
    _In_ UINT16 FunctionAddress,
    _Inout_ CHIP8_DISASSEMBLED_PROGRAM* Disassembly
)
{
    CHIP8_FUNCTION* Function = Chip8FunctionCreate( );
    Function->Address = FunctionAddress;

    Chip8DiscoverFunctionBasicBlocks( ProgramSpace, Function );

    for ( BYTE NodeIndex = NULL;
               NodeIndex < Function->ControlFlowGraph->NodesCount;
               NodeIndex++ )
    {
        CONST CHIP8_CONTROL_FLOW_NODE CONST* CurrentBlockNode = Function->ControlFlowGraph->Nodes[ NodeIndex ];
        if ( Chip8DisassembleProgramBasicBlock( ProgramSpace, Function, CurrentBlockNode->Address ) == FALSE )
        {
            return FALSE;
        }
    }

    qsort( Function->BasicBlocks, Function->BasicBlocksCount, sizeof( CHIP8_BASIC_BLOCK* ), Chip8CompareBasicBlocks );
    qsort( Function->ControlFlowGraph->Nodes, Function->ControlFlowGraph->NodesCount, sizeof( CHIP8_CONTROL_FLOW_NODE* ), Chip8CompareControlFlowNodes );

    Disassembly->Functions.push_back( Function );

    return TRUE;
}

VOID
Chip8DiscoverFunctions(
    _In_ CONST BYTE CONST* Program,
    _In_ UINT16 Function,
    _Inout_ std::vector<UINT16>& Functions
)
{
    if ( std::find( Functions.begin( ), Functions.end( ), Function ) == Functions.end( ) )
    {
        Functions.push_back( Function );
    }

    UINT16 ProgramCounter = Function;
    while ( ProgramCounter )
    {
        CHIP8_INSTRUCTION Instruction = { };
        if ( Chip8DisassembleInstruction( ( CONST UINT16 CONST* )( Program + ProgramCounter ), &Instruction ) == FALSE )
        {
            return;
        }

        switch ( Instruction.Mnemonic )
        {
            //
            // record instruction for block, follow call for disassembly
            //
            case CHIP8_MNEMONIC_CALL:
            {
                Chip8DiscoverFunctions( Program, Instruction.Operands[ 0 ].Address, Functions );

                ProgramCounter += sizeof( CHIP8_ENCODED_INSTRUCTION );

            } break;

            case CHIP8_MNEMONIC_RET: return;

            //
            // record instruction, advance program counter
            //
            default:
            {
                ProgramCounter += sizeof( CHIP8_ENCODED_INSTRUCTION );

            } break;
        }
    }
}

VOID
Chip8DiscoverProgramFunctions(
    _In_ CONST BYTE CONST* Program,
    _Inout_ std::vector<UINT16>& Functions
)
{
    return Chip8DiscoverFunctions( Program, 512, Functions );
}

BOOL
Chip8DisassembleProgram(
    _In_ CONST BYTE CONST* Program,
    _In_ SIZE_T ProgramSize,
    _Inout_ CHIP8_DISASSEMBLED_PROGRAM* Disassembly
)
{
    if ( ProgramSize > ( 4096 - 512 ) )
    {
        return FALSE;
    }

    BYTE ProgramSpace[ 4096 ] = { };
    memcpy( ProgramSpace + 512, Program, ProgramSize );

    std::vector<UINT16> ProgramFunctions = { };
    Chip8DiscoverProgramFunctions( ProgramSpace, ProgramFunctions );

    for ( UINT16 Function : ProgramFunctions )
    {
        if ( Chip8DisassembleProgramFunction( ProgramSpace, Function, Disassembly ) == FALSE )
        {
            return FALSE;
        }
    }

    return TRUE;
}

STATIC
LPCSTR
Chip8GetMnemonicString(
    _In_ CHIP8_MNEMONIC Mnemonic
)
{
    switch ( Mnemonic )
    {   
        case CHIP8_MNEMONIC_CLS:    return "CLS";
        case CHIP8_MNEMONIC_RET:    return "RET";
        case CHIP8_MNEMONIC_JMP:    return "JMP";
        case CHIP8_MNEMONIC_CALL:   return "CALL";
        case CHIP8_MNEMONIC_SE:     return "SE";
        case CHIP8_MNEMONIC_SNE:    return "SNE";
        case CHIP8_MNEMONIC_MOV:    return "MOV";
        case CHIP8_MNEMONIC_OR:     return "OR";
        case CHIP8_MNEMONIC_AND:    return "AND";
        case CHIP8_MNEMONIC_XOR:    return "XOR";
        case CHIP8_MNEMONIC_ADD:    return "ADD";
        case CHIP8_MNEMONIC_SUB:    return "SUB";
        case CHIP8_MNEMONIC_SHR:    return "SHR";
        case CHIP8_MNEMONIC_SUBN:   return "SUBN";
        case CHIP8_MNEMONIC_SHL:    return "SHL";
        case CHIP8_MNEMONIC_SKP:    return "SKP";
        case CHIP8_MNEMONIC_SKNP:   return "SKNP";
        case CHIP8_MNEMONIC_HLT:    return "HLT";
        case CHIP8_MNEMONIC_RAND:   return "RAND";
        case CHIP8_MNEMONIC_DRAW:   return "DRAW";
        case CHIP8_MNEMONIC_VMSAVE: return "VMSAVE";
        case CHIP8_MNEMONIC_VMLOAD: return "VMLOAD";

        default: return NULL;
    }

    return NULL;
}

STATIC
SIZE_T
Chip8FormatOperand(
    _In_ CONST CHIP8_OPERAND CONST* Operand,
    _Inout_ CHAR* Buffer,
    _In_ SIZE_T BufferSize
)
{
    SIZE_T PrintedSize = NULL;

    switch ( Operand->Type )
    {
        case CHIP8_OPERAND_TYPE_REGISTER:
        {
            PrintedSize += snprintf( Buffer + PrintedSize, BufferSize - PrintedSize, "R%d", Operand->Register );

            if ( Operand->Flags & CHIP8_OPERAND_FLAG_BINARY_CODED_DECIMAL )
            {
                PrintedSize += snprintf( Buffer + PrintedSize, BufferSize - PrintedSize, " BCD" );
            }
            else if ( Operand->Flags & CHIP8_OPERAND_FLAG_SPRITE_INDEX )
            {
                PrintedSize += snprintf( Buffer + PrintedSize, BufferSize - PrintedSize, " SPRITE" );
            }

        } break;

        case CHIP8_OPERAND_TYPE_ADDRESS:
        {
            PrintedSize += snprintf( Buffer + PrintedSize, BufferSize - PrintedSize, "%03lX", Operand->Address );

            if ( Operand->Flags & CHIP8_OPERAND_FLAG_RELATIVE_R0 )
            {
                PrintedSize += snprintf( Buffer + PrintedSize, BufferSize - PrintedSize, ", R0" );
            }

        } break;

        case CHIP8_OPERAND_TYPE_IMMEDIATE:
        {
            PrintedSize += snprintf( Buffer + PrintedSize, BufferSize - PrintedSize, "%03X", Operand->Immediate );

        } break;

        case CHIP8_OPERAND_TYPE_MEMORY_INDEX:
        {
            if ( Operand->Flags & CHIP8_OPERAND_FLAG_MEMORY_ACCESS )
            {
                PrintedSize += snprintf( Buffer + PrintedSize, BufferSize - PrintedSize, "[INDEX]" );
            }
            else
            {
                PrintedSize += snprintf( Buffer + PrintedSize, BufferSize - PrintedSize, "INDEX" );
            }

        } break;

        case CHIP8_OPERAND_TYPE_DELAY_TIMER:
        {
            PrintedSize += snprintf( Buffer + PrintedSize, BufferSize - PrintedSize, "DELAY" );

        } break;

        case CHIP8_OPERAND_TYPE_SOUND_TIMER:
        {
            PrintedSize += snprintf( Buffer + PrintedSize, BufferSize - PrintedSize, "SOUND" );

        } break;

        default: break;
    }

    return PrintedSize;
}

BOOL
Chip8FormatInstruction(
    _In_ CONST CHIP8_INSTRUCTION CONST* Instruction,
    _Inout_ CHAR* Buffer,
    _In_ SIZE_T BufferSize
)
{
    LPCSTR MnemonicString = Chip8GetMnemonicString( Instruction->Mnemonic );
    if ( MnemonicString == NULL )
    {
        return FALSE;
    }

    SIZE_T PrintSize = snprintf( Buffer, BufferSize, "%s", MnemonicString );

    for ( ULONG I = NULL; I < Instruction->OperandsCount; I++ )
    {
        if ( I > NULL )
        {
            PrintSize += snprintf( Buffer + PrintSize, BufferSize - PrintSize, ", " );
        }
        else
        {
            PrintSize += snprintf( Buffer + PrintSize, BufferSize - PrintSize, " " );
        }

        PrintSize += Chip8FormatOperand( &Instruction->Operands[ I ], Buffer + PrintSize, BufferSize - PrintSize );
    }

    return TRUE;
}