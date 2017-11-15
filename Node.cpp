#include "Node.h"
#include "Edge.h"

#include <sstream>
#include <iomanip>
#include <cctype> // für isalnum() (siehe http://www.cplusplus.com/reference/cctype/isalnum/?kw=isalnum)

int Node::s_numInstances = 0;


//-------------------------------------------------------------------------------------------------

Node::Node()
{
    std::stringstream s;
    s << "n" << std::setw(4) << std::setfill('0') << s_numInstances;
    m_id = s.str();

    s_numInstances += 1;
}


//-------------------------------------------------------------------------------------------------

Node::Node(std::string id) : m_id(id)
{
    s_numInstances += 1;
}


//-------------------------------------------------------------------------------------------------

std::list<Node*> Node::getNeighbours(Direction direction)
{
    std::list<Node*> ret;

    if (direction == DIR_OUT || direction == DIR_BOTH) {
        for (Edge* pEdge : m_outEdges)
            ret.push_back(&pEdge->getDstNode());
    }

    if (direction == DIR_IN || direction == DIR_BOTH) {
        for (Edge* pEdge : m_inEdges)
            ret.push_back(&pEdge->getSrcNode());
    }

    return ret;
}


//-------------------------------------------------------------------------------------------------
