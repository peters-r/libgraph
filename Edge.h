#ifndef EDGE_H
#define EDGE_H

#include <string>
#include "Node.h"


//-------------------------------------------------------------------------------------------------

class Edge
{

public:

    //! @Lifetime

    Edge(Node& rSrc, Node& rDst);

    Edge(const Edge& rOther) : Edge(rOther.m_srcNode, rOther.m_dstNode) { }

    virtual ~Edge();


    //! @Edge Information

    /** returns true, if the given node is connected with thie edge as source or destination. */
    bool isConnectedTo(const Node& rNode) const;

    /** get a string representation of this edge. */
    std::string toString() const;

    /** Override this function in order to retrieve the correct weight. */
    virtual double getWeight() const = 0;

	Node& getSrcNode() { return m_srcNode; }
	Node& getDstNode() { return m_dstNode; }


private:

	Node& m_srcNode;
	Node& m_dstNode;

#ifdef TESTING
    friend class GraphTesting;
#endif
};


//-------------------------------------------------------------------------------------------------

#endif