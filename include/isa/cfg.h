#ifndef _CHIP8_CONTROL_FLOW_GRAPH_H_
#define _CHIP8_CONTROL_FLOW_GRAPH_H_

#include <vector>

#include <defines.h>

#define CHIP8_CFG_MAX_NODE_PREDECESSORS 6
#define CHIP8_CFG_MAX_NODE_SUCCESSORS   2

typedef struct _CHIP8_CONTROL_FLOW_NODE
{
    UINT16 Address;

    UINT16 Predecessors[ CHIP8_CFG_MAX_NODE_PREDECESSORS ];
    BYTE PredecessorsCount;

    UINT16 Successors[ CHIP8_CFG_MAX_NODE_SUCCESSORS ];
    BYTE SuccessorsCount;

} CHIP8_CONTROL_FLOW_NODE, *PCHIP8_CONTROL_FLOW_NODE;


typedef struct _CHIP8_CONTROL_FLOW_GRAPH
{
    CHIP8_CONTROL_FLOW_NODE** Nodes;
    BYTE NodesCount;

} CHIP8_CONTROL_FLOW_GRAPH, *PCHIP8_CONTROL_FLOW_GRAPH;

FORCEINLINE
CHIP8_CONTROL_FLOW_NODE*
Chip8ControlFlowNodeCreate(
    _Inout_ CHIP8_CONTROL_FLOW_GRAPH* ControlFlowGraph
)
{
    CHIP8_CONTROL_FLOW_NODE* ConstructedNode = ( CHIP8_CONTROL_FLOW_NODE* )malloc( sizeof( CHIP8_CONTROL_FLOW_NODE ) );
    memset( ConstructedNode, NULL, sizeof( CHIP8_CONTROL_FLOW_NODE ) );

    return ConstructedNode;
}

FORCEINLINE
VOID
Chip8ControlFlowNodeCreate(
    _Inout_ CHIP8_CONTROL_FLOW_GRAPH* ControlFlowGraph,
    _Inout_ CHIP8_CONTROL_FLOW_NODE** ControlFlowNode
)
{
    *ControlFlowNode = Chip8ControlFlowNodeCreate( ControlFlowGraph );
}

FORCEINLINE
VOID
Chip8ControlFlowNodeDestroy(
    _Inout_ CHIP8_CONTROL_FLOW_NODE* ControlFlowNode
)
{
    free( ControlFlowNode );
}

FORCEINLINE
BOOL
Chip8ControlFlowNodeHasPredecessor(
    _Inout_ CHIP8_CONTROL_FLOW_NODE* ControlFlowNode,
    _In_ UINT16 Predecessor
)
{
    for ( BYTE Index = NULL;
               Index < ControlFlowNode->PredecessorsCount;
               Index++ )
    {
        if ( ControlFlowNode->Predecessors[ Index ] == Predecessor )
        {
            return TRUE;
        }
    }

    return FALSE;
}

FORCEINLINE
VOID
Chip8ControlFlowNodeAddPredecessor(
    _Inout_ CHIP8_CONTROL_FLOW_NODE* ControlFlowNode,
    _In_ UINT16 Predecessor
)
{
    if ( Chip8ControlFlowNodeHasPredecessor( ControlFlowNode, Predecessor ) )
    {
        return;
    }

    ControlFlowNode->Predecessors[ ControlFlowNode->PredecessorsCount ] = Predecessor;
    ControlFlowNode->PredecessorsCount += 1;
}

FORCEINLINE
BOOL
Chip8ControlFlowNodeHasSuccessor(
    _Inout_ CHIP8_CONTROL_FLOW_NODE* ControlFlowNode,
    _In_ UINT16 Successor
)
{
    for ( BYTE Index = NULL; 
               Index < ControlFlowNode->SuccessorsCount; 
               Index++ )
    {
        if ( ControlFlowNode->Successors[ Index ] == Successor )
        {
            return TRUE;
        }
    }

    return FALSE;
}

FORCEINLINE
VOID
Chip8ControlFlowNodeAddSuccessor(
    _Inout_ CHIP8_CONTROL_FLOW_NODE* ControlFlowNode,
    _In_ UINT16 Successor
)
{
    if ( Chip8ControlFlowNodeHasSuccessor( ControlFlowNode, Successor ) )
    {
        return;
    }

    ControlFlowNode->Successors[ ControlFlowNode->SuccessorsCount ] = Successor;
    ControlFlowNode->SuccessorsCount += 1;
}


FORCEINLINE
CHIP8_CONTROL_FLOW_GRAPH*
Chip8ControlFlowGraphCreate(
    VOID
)
{
    CHIP8_CONTROL_FLOW_GRAPH* ControlFlowGraph = ( CHIP8_CONTROL_FLOW_GRAPH* )malloc( sizeof( CHIP8_CONTROL_FLOW_GRAPH ) );
    memset( ControlFlowGraph, NULL, sizeof( CHIP8_CONTROL_FLOW_GRAPH ) );

    return ControlFlowGraph;
}

FORCEINLINE
VOID
Chip8ControlFlowGraphCreate(
    _Inout_ CHIP8_CONTROL_FLOW_GRAPH** ControlFlowGraph
)
{
    *ControlFlowGraph = Chip8ControlFlowGraphCreate( );
}

FORCEINLINE
VOID
Chip8ControlFlowGraphDestroy(
    _In_ CHIP8_CONTROL_FLOW_GRAPH* ControlFlowGraph
)
{
    if ( ControlFlowGraph->Nodes )
    {
        for ( BYTE NodeIndex = NULL;
                   NodeIndex < ControlFlowGraph->NodesCount;
                   NodeIndex++ )
        {
            Chip8ControlFlowNodeDestroy( ControlFlowGraph->Nodes[ NodeIndex ] );
        }

        free( ControlFlowGraph->Nodes );
    }

    free( ControlFlowGraph );
}

FORCEINLINE
VOID
Chip8ControlFlowGraphAddControlFlowNode(
    _Inout_ CHIP8_CONTROL_FLOW_GRAPH* ControlFlowGraph,
    _In_ CHIP8_CONTROL_FLOW_NODE* ControlFlowNode
)
{
    ControlFlowGraph->NodesCount += 1;
    ControlFlowGraph->Nodes = ( CHIP8_CONTROL_FLOW_NODE** )realloc( ControlFlowGraph->Nodes, ControlFlowGraph->NodesCount * sizeof( CHIP8_CONTROL_FLOW_NODE* ) );

    ControlFlowGraph->Nodes[ ControlFlowGraph->NodesCount - 1 ] = ControlFlowNode;
}

FORCEINLINE
BOOL
Chip8ControlFlowGraphLookupNodeByAddress(
    _In_ CHIP8_CONTROL_FLOW_GRAPH* ControlFlowGraph,
    _In_ UINT16 Address,
    _Inout_opt_ CHIP8_CONTROL_FLOW_NODE** ControlFlowNode
)
{
    for ( BYTE NodeIndex = NULL;
               NodeIndex < ControlFlowGraph->NodesCount;
               NodeIndex++ )
    {
        CHIP8_CONTROL_FLOW_NODE* CurrentNode = ControlFlowGraph->Nodes[ NodeIndex ];
        if ( CurrentNode->Address == Address )
        {
            if ( ControlFlowNode )
            {
                *ControlFlowNode = CurrentNode;
            }

            return TRUE;
        }
    }

    return FALSE;
}

#endif // _CHIP8_CONTROL_FLOW_GRAPH_H_