//
// Created by moira-q on 17.05.20.
//

#ifndef ALGOSI3_SETGRAPH_H
#define ALGOSI3_SETGRAPH_H

#include <set>

#include "IGraph.h"

class SetGraph: public IGraph {

public:
    explicit SetGraph(size_t vertices_count) : vertices(vertices_count){
    }

    SetGraph(const IGraph& graph): vertices(graph.VerticesCount()) {
        for (int i = 0; i < graph.VerticesCount(); ++i) {
            for (const auto& v: graph.GetNextVertices(i)) {
                AddEdge(i, v);
            }
        }
    }


    ~SetGraph() override = default ;

    // Добавление ребра от from к to.
    void AddEdge(int from, int to) override {
        vertices[from].insert(to);

    }

    int VerticesCount() const override {
        return vertices.size();
    }

    std::vector<int> GetNextVertices(int vertex) const override {
        std::vector<int> result;
        result.reserve(vertices[vertex].size());
        for(const auto& v: vertices[vertex]) {
            result.push_back(v);
        }
        return result;
    }

    std::vector<int> GetPrevVertices(int vertex) const override {
        std::vector<int> result;
        for (int i = 0; i < VerticesCount(); ++i) {
            if (vertices[i].count(vertex)) {
                result.push_back(i);
            }
        }
        return result;
    }

private:
    std::vector<std::set<int>> vertices;
};

#endif //ALGOSI3_SETGRAPH_H
