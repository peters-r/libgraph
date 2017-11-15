#ifndef NODE_H
#define NODE_H

#include <string>
#include <list>

class Edge;


//-------------------------------------------------------------------------------------------------

class Node
{

public:

    Node();

	Node(std::string id);

    enum Direction { DIR_IN, DIR_OUT, DIR_BOTH };

    virtual ~Node() {}

	const std::string& getId() const { return m_id; }

	std::list<Edge*>& getOutEdges() { return m_outEdges; }
    std::list<Edge*>& getInEdges() { return m_inEdges; }

    std::list<Node*> getNeighbours(Direction direction = DIR_BOTH);

    virtual bool operator==(const Node& rOther) const { return m_id == rOther.m_id; }
    virtual bool operator<(const Node& rOther) const { return m_id < rOther.m_id; }

private:

	std::string m_id;

	std::list<Edge*> m_outEdges;
    std::list<Edge*> m_inEdges;

    static int s_numInstances;

#ifdef TESTING
    friend class GraphTesting;
#endif
};


//-------------------------------------------------------------------------------------------------

#endif