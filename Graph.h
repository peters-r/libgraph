#ifndef GRAPH_H
#define GRAPH_H

#include <list>
#include <string>
#include <deque>
#include <set>
#include <vector>
#include <map>
#include <algorithm>
#include <memory>

#include "Node.h"
#include "Edge.h"


/* --------------------------------------------------------------------------------------------- */

class Graph
{

private:

    //! @Datataypes

    // This is a helper function object in order to sort the nodes by id in a set of nodes.
    struct SortNodeByIdHelper {
        bool operator()(const Node* l, const Node* r) const {
            return l->getId() < r->getId();
        }
    };

    // some typedefs for containers that are used in this class
    typedef std::set<Node*, SortNodeByIdHelper> tNodePtrSet;
    typedef std::list<Edge*> tEdgePtrList;
    typedef std::deque<Edge*> tPath;
    typedef std::vector<Edge*> tEdges;
    typedef std::vector<Node*> tNodes;

    struct tDijkstraInfo
    {
        double distance;
        Node* prevNode;
        Edge* prevEdge;
    };

    typedef std::map<Node*, tDijkstraInfo> tDijkstraMap;


public:

    class Exception;
    class NodeCreationException;
    class InvalidNodeException;
    class NotFoundException;


public:

    virtual ~Graph();   

    /**
    */
    template<class T>
    T& makeNode(T&& node);

    template<class T, class... Args>
    T& makeNode(Args&&... args) { return makeNode(T(std::forward<Args>(args)...)); }

    template<class T>
    T& makeEdge(T&& edge);

    template<class T, class... Args>
    T& makeEdge(Args&&... args) { return makeEdge(T(std::forward<Args>(args)...)); }

    /** Constructs to Edges. */
    template<class T, class... Args>
    void makeBiEdge(Node& n1, Node& n2, Args&&... args) { 
        makeEdge(T(n1, n2, std::forward<Args>(args)...));
        makeEdge(T(n2, n1, std::forward<Args>(args)...));
    }

    template<class T>
    Graph& operator << (T&& rEdge) {
        // forward as r-value reference
        makeEdge(std::move(rEdge))
        return *this;
    }

    tNodePtrSet& getNodes() { return m_nodes; }

    /**
    * Deletes the given Edge from the graph.
    * The Edge object will be destroyed after this function call.
    * @return true if the Edge was found and deleted, false otherwise. 
    */
    bool remove(const Edge& rEdge);

    /**
    * Deletes the given Node from the graph.
    * The Node object will be destroyed after this function call.
    * All connected edges will be destroyed, too.
    * @return true if the Node was found and deleted, false otherwise.
    */
    bool remove(const Node& rNode);

    /** 
    * Retrieves a node by the given id. 
    * @return a pointer to the node or NULL if not found. 
    */
    Node* Graph::findNodeById(const std::string& id);

    /** Retrieves all edges that have rSrc as source node and rDst as destination node. */
    tEdges findEdges(const Node& rSrc, const Node& rDst);

    /** Retrieves all edges whose id for the source node is rSrc and rDst for the dest node. */
    tEdges findEdges(const std::string& srcId, const std::string& dstId);

    /** Generates a list of all connected nodes. Each line represents an edge. */
	std::string toString() const;

    /**
    * Saves the graph as dot file. The tool Graphiz can generate an image from this file.
    * @param rFimename the target file name.
    */
    void saveAsDot(const std::string& rFilename) const;

    /**
    * The Dijkstra algorithm calculates the shortest path of all nodes to a single root node.
    * @param rSrcNode is the node to calculate the distance to.
    * @param pDstNode the algorithm stops, if the path to *pDstNode is found.
    * @param pFoundDst contains the address of the destination node or is set to NULL, if no path was found.
    * @return a map of nodes with associated routing information to the source node..
    */
    tDijkstraMap findDistancesDijkstra(const Node& rSrcNode, const Node* pDstNode, Node** pFoundDst);

    /**
    * Calculate the shortest path from a source node to a destination node.
    * @param the source node.
    * @param the destination node.
    * @return tPath is a deque of edges and represents the route from rSrc to rDst.
    */
    tPath findShortestPathDijkstra(const Node& rSrc, const Node& rDst);


protected:

    tNodePtrSet m_nodes;
    tEdgePtrList m_edges;

#ifdef TESTING
    friend class GraphTesting;
#endif
};


/* --------------------------------------------------------------------------------------------- */

class Graph::Exception
{
public:
    Exception(const std::string& what) : m_what(what) { }
    virtual ~Exception() {}
    const std::string& what() const { return m_what; }

private:
    std::string m_what;
};

class Graph::NodeCreationException : public Graph::Exception {
    public: NodeCreationException(const std::string& what) : Exception(what) { }
};

class Graph::InvalidNodeException : public Graph::Exception {
    public: InvalidNodeException(const std::string& what) : Exception(what) { }
};

class Graph::NotFoundException : public Graph::Exception {
    public: NotFoundException(const std::string& what) : Exception(what) { }
};


/* --------------------------------------------------------------------------------------------- */

template<class T>
T& Graph::makeNode(T&& node)
{
    // is there already a node with the given id?
    auto it = m_nodes.lower_bound(&node);
    if (it != m_nodes.end() && (*it)->getId() == node.getId()) {
        throw NodeCreationException("NodeID is not unique: " + node.getId());
    }

    // if not, create a new node
    auto ret = m_nodes.insert(it, new T(std::move(node)));

    // ret.first is an iterator to the new element. 
    // We must dereference it twice (iterator & unique_ptr) in order to get the node reference.
    return **ret;
}


/* --------------------------------------------------------------------------------------------- */

template<class T>
T& Graph::makeEdge(T&& edge)
{
    // check if src and destination nodes are in the graph
    if (std::find(m_nodes.begin(), m_nodes.end(), &edge.getDstNode()) == m_nodes.end()) {
        throw InvalidNodeException("source node is not in the graph");
    }

    if (std::find(m_nodes.begin(), m_nodes.end(), &edge.getSrcNode()) == m_nodes.end()) {
        throw InvalidNodeException("destination node is not in the graph");
    }

    T* newEdge = new T(std::move(edge));
    m_edges.push_back(newEdge);
    return *newEdge;
}


/* --------------------------------------------------------------------------------------------- */

#endif