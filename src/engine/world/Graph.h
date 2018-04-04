#ifndef GRAPH_H
#define GRAPH_H

#include <vector>
#include <list>
#include <map>
#include <unordered_map>
#include <algorithm>
#include <memory>
#include <queue>
#include <set>
#include <unordered_set>
#include <iostream>

template <class V, class W> struct Edge;

template <class V, class W> struct Vertex {
public:
    Vertex(V v) : data(v) {
        edges.reserve(12);
    }

    inline void addEdge(Edge<V, W> e) {
        edges.push_back(e);
    }

    inline void removeEdge(Edge<V, W> e) {
        edges.erase(std::remove(edges.begin(), edges.end(), e), edges.end());
    }

    inline bool operator==(const Vertex<V, W>& other) {
        return data == other.data;
    }

    V data;
    std::vector<Edge<V, W>> edges;
};

template <class V, class W> struct Edge {
public:
    Edge(Vertex<V, W> *start, Vertex<V, W> *end, W weight) : start(start), end(end), weight(weight) {}


    Vertex<V, W> *start, *end;
    W weight;
};

template <class V, class W> struct Path {
public:
    Path(Edge<V, W> edge) {
        edges.insert(edges.end(), edge);
        cost = edge.weight;
    }

    Path(Edge<V, W> edge, Path<V, W> previous) {
        edges = previous.edges;
        edges.insert(edges.end(), edge);
        cost = previous.cost + edge.weight;
    }

    friend bool operator< (const Path<V, W>& lhs, const Path<V, W>& rhs) {
        return lhs.cost < rhs.cost;
    }

    std::list<Edge<V, W>> edges;
    W cost;

};

template <class V, class W> class Graph {
public:
    Graph(int limit) : limit(limit) {}

    void addVertex(V data) {
        vmap.insert(std::make_pair(data, Vertex<V, W>(data)));
    }

    void addEdge(V v1, V v2, W weight) {

        auto start = vmap.find(v1);
        auto end = vmap.find(v2);

        if (start == vmap.end() || end == vmap.end()) {
            return;
        }

        Vertex<V, W> *st = &(*start).second;
        Vertex<V, W> *en = &(*end).second;

        st->addEdge(Edge<V, W>(st, en, weight));
    }

    std::list<Edge<V, W>> aStar(V s, V g) {

        std::list<Edge<V, W>> res;
        if (s == g) {
            //std::cout << "same" << std::endl;
            return res;
        }

        auto sf = vmap.find(s);
        if (sf == vmap.end()) {
            //std::cout << "not found" << std::endl;
            return res;
        }
        auto gf = vmap.find(g);
        if (gf == vmap.end()) {
            //std::cout << "not found" << std::endl;
            return res;
        }

        Vertex<V, W> &start = (*sf).second;
        Vertex<V, W> &goal = (*gf).second;

        //std::cout << "Start: " << start.data << std::endl;
        //std::cout << "Goal: " << goal.data << std::endl;

        std::priority_queue<Path<V, W>> paths;
        std::set<V> visited;

        for (Edge<V, W> edge : start.edges) {
            paths.push(Path<V, W>(edge));
            //std::cout << "path: " << edge.start.data << " " << edge.end.data << std::endl;
        }

        while (!paths.empty()) {
            Path<V, W> path = paths.top();
            paths.pop();

            Vertex<V, W> *vertex = path.edges.back().end;
            if (*vertex == goal) {
                return path.edges;
            }

            visited.insert(vertex->data);
            for (Edge<V, W> edge : vertex->edges) {
                Vertex<V, W> *neighbor = edge.end;
                if (visited.find(neighbor->data) == visited.end()) {
                    paths.push(Path<V, W>(edge, path));
                }
            }
        }

        //std::cout << "diff" << std::endl;

        return res;
    }

private:

    std::unordered_map<V, Vertex<V, W>> vmap;
    int limit;
};



#endif // GRAPH_H
