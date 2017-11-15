#include "Graph.h"

#include <map>
#include <limits>


//-------------------------------------------------------------------------------------------------

Graph::~Graph() 
{ 
    // free all nodes and edges
    for (Edge* pEdge : m_edges) delete pEdge;
    for (Node* pNode : m_nodes) delete pNode;
}


//-------------------------------------------------------------------------------------------------

bool Graph::remove(const Edge& rEdge)
{
    auto it = std::find(m_edges.begin(), m_edges.end(), &rEdge);
    if (it != m_edges.end()) {
        delete *it;
        m_edges.erase(it);
        return true;
    }

    return false;
}


//-------------------------------------------------------------------------------------------------

bool Graph::remove(const Node& rNode)
{
    auto it = std::find(m_nodes.begin(), m_nodes.end(), &rNode);
    if (it != m_nodes.end()) {
        // delete all edges that are connected with the given node
        auto eIt = m_edges.begin();
        while (eIt != m_edges.end()) {
            if ((*eIt)->isConnectedTo(rNode)) {
                delete *eIt;
                eIt = m_edges.erase(eIt);
            }
            else {
                eIt++;
            }
        }
        // delete the node
        delete *it;
        m_nodes.erase(it);
        return true;
    }
    return false;
}


//-------------------------------------------------------------------------------------------------

Node* Graph::findNodeById(const std::string& id)
{
    auto it = m_nodes.begin();
    // just need to check the lower bound, since nodes are sorted by id.
    while (it != m_nodes.end() && (*it)->getId() < id) {
        it++;
    }

    // return the node, if found
    if (it != m_nodes.end() && (*it)->getId() == id) {
        return *it;
    }

    return NULL;
}


//-------------------------------------------------------------------------------------------------

Graph::tEdges Graph::findEdges(const Node& rSrc, const Node& rDst)
{
    tEdges ret;

    for (auto it = m_edges.begin(); it != m_edges.end(); it++) {
        if ((&(*it)->getSrcNode() == &rSrc) && (&(*it)->getDstNode() == &rDst)) {
            ret.push_back(*it);
        }
    }

    return ret;
}


//-------------------------------------------------------------------------------------------------

Graph::tEdges Graph::findEdges(const std::string& srcId, const std::string& dstId)
{
    Node* pSrc = findNodeById(srcId);
    if (pSrc == NULL) {
        return tEdges();
    }

    Node* pDst = findNodeById(dstId);
    if (pDst == NULL) {
        return tEdges();
    }

    return findEdges(*pSrc, *pDst);
}


//-------------------------------------------------------------------------------------------------

std::string Graph::toString() const
{
	std::string result;

	for (auto it = m_edges.begin(); it != m_edges.end(); it++)
	{
		result += (*it)->toString() + "\n";
	}

	return result;
}


//-------------------------------------------------------------------------------------------------

void Graph::saveAsDot(const std::string& rFilename) const
{

}


//-------------------------------------------------------------------------------------------------

/**
* This is based on https://en.wikipedia.org/wiki/Dijkstra%27s_algorithm
*/
Graph::tDijkstraMap Graph::findDistancesDijkstra(
        const Node& rSrcNode, const Node* pDstNode, Node** pFoundDst)
{
    tDijkstraMap nodeTable;
    std::list<Node*> Q;

    auto srcIt = std::find(m_nodes.begin(), m_nodes.end(), &rSrcNode);
    if (srcIt == m_nodes.end()) {
        throw InvalidNodeException("source node is not in the graph");
    }
    Node* pSrc = *srcIt;

    Node* pDst = NULL;
    if (pDstNode != NULL) {
        auto dstIt = std::find(m_nodes.begin(), m_nodes.end(), pDstNode);
        if (dstIt == m_nodes.end()) {
            throw InvalidNodeException("destination node is not in the graph");
        }
        pDst = *dstIt;
    }

    /*
    for each vertex v in Graph:          // Initialization
    dist[v] ← INFINITY                  // Unknown distance from source to v
    prev[v] ← UNDEFINED                 // Previous node in optimal path from source
    add v to Q                          // All nodes initially in Q (unvisited nodes)
    */
    for (Node* pNode : m_nodes) {
        Q.push_back(pNode);
        nodeTable[pNode] = { std::numeric_limits<double>::max(), NULL, NULL };
    }
    // dist[source] ← 0
    nodeTable[pSrc].distance = 0;

    // while Q is not empty:
    while (!Q.empty()) {

        // u = vertex in Q with min dist[u]
        Node* u = Q.front();
        for (Node* pCurrentNode : Q) {
            if (nodeTable[pCurrentNode].distance < nodeTable[u].distance) {
                u = pCurrentNode;
            }
        }

        // abort criteria (leave while-loop)
        if (u == pDst) {
            *pFoundDst = u;
            return nodeTable;
        }

        // remove u from Q
        Q.remove(u);

        // for each neighbor v of u:
        for (Edge* pOutEdge : u->getOutEdges()) {
            Node* v = &pOutEdge->getDstNode();
            // alt ← dist[u] + length(u, v)
            double newDistance = nodeTable[u].distance + pOutEdge->getWeight();
            // update dijkstra entry if new < dist[v]:
            tDijkstraInfo& vEntry = nodeTable[v];
            if (newDistance < vEntry.distance) {
                vEntry.distance = newDistance;
                vEntry.prevNode = u;
                vEntry.prevEdge = pOutEdge;
            }
        }
    }
    *pFoundDst = NULL;
    return nodeTable;
}


//-------------------------------------------------------------------------------------------------

Graph::tPath Graph::findShortestPathDijkstra(const Node& rSrc, const Node& rDst)
{
    Graph::tPath path;
    Node* currentNode;
    
    // currentNode will be NULL, if no path was found.
    tDijkstraMap nodeTable = findDistancesDijkstra(rSrc, &rDst, &currentNode);
    
    // insert the path to a deque
    if (currentNode != NULL) {
        while (nodeTable[currentNode].prevNode != NULL) {
            tDijkstraInfo& curr = nodeTable[currentNode];
            path.push_front(curr.prevEdge);
            currentNode = curr.prevNode;
        }
    }

    return path;
}


//-------------------------------------------------------------------------------------------------
