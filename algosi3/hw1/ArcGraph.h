//
// Created by moira-q on 17.05.20.
//

#ifndef ALGOSI3_ARCGRAPH_H
#define ALGOSI3_ARCGRAPH_H

#include "IGraph.h"

class ArcGraph: public IGraph {

public:
    explicit ArcGraph(size_t vertices_count) : ver_count(vertices_count) {
    }

    ArcGraph(const IGraph& graph) : ver_count(graph.VerticesCount()) {
        for (int i = 0; i < graph.VerticesCount(); ++i) {
            for (const auto& v: graph.GetNextVertices(i)) {
                AddEdge(i, v);
            }
        }
    }


    ~ArcGraph() override = default ;

    // Добавление ребра от from к to.
    void AddEdge(int from, int to) override {
        vertices_pair.push_back({from, to});

    }

    int VerticesCount() const override {
        return ver_count;
    }

    std::vector<int> GetNextVertices(int vertex) const override {
        std::vector<int> result;
        for (const auto& v_pair: vertices_pair) {
            if (v_pair.first == vertex) {
                result.push_back(v_pair.second);
            }
        }
        return result;
    }

    std::vector<int> GetPrevVertices(int vertex) const override {
        std::vector<int> result;
        for (const auto& v_pair: vertices_pair) {
            if (v_pair.second == vertex) {
                result.push_back(v_pair.first);
            }
        }
        return result;
    }

private:
    std::vector<std::pair<int, int>> vertices_pair;
    size_t ver_count;
};
#endif //ALGOSI3_ARCGRAPH_H
