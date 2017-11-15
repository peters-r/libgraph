#ifndef SIMPLEEDGE_H
#define SIMPLEEDGE_H

#include "Edge.h"

/* A simple implementation of an edge. */
class SimpleEdge : public Edge
{
public:
    SimpleEdge(Node& src, Node& dst, double weight) : Edge(src, dst), m_weight(weight) { }

    virtual double getWeight() const { return m_weight; }

private:
    double m_weight;
};


#endif