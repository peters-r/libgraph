#include "Edge.h"


//-------------------------------------------------------------------------------------------------

Edge::Edge(Node& rSrc, Node& rDst) 
    : m_srcNode(rSrc), m_dstNode(rDst)  
{  
    rSrc.getOutEdges().push_back(this);
    rDst.getInEdges().push_back(this);
}


//-------------------------------------------------------------------------------------------------

Edge::~Edge()
{
    m_srcNode.getOutEdges().remove(this);
    m_dstNode.getInEdges().remove(this);
}


//-------------------------------------------------------------------------------------------------

bool Edge::isConnectedTo(const Node& rNode) const
{
    return &m_srcNode == &rNode || &m_dstNode == &rNode;
}


//-------------------------------------------------------------------------------------------------

std::string Edge::toString() const 
{ 
    return m_srcNode.getId() + " -> " + m_dstNode.getId();
}


//-------------------------------------------------------------------------------------------------
