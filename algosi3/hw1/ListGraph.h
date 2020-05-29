//
// Created by moira-q on 17.05.20.
//

#ifndef ALGOSI3_LISTGRAPH_H
#define ALGOSI3_LISTGRAPH_H

#include "IGraph.h"

class ListGraph: public IGraph {
public:
    explicit ListGraph(size_t vertices_count) : vertices_f_to(vertices_count), vertices_to_f(vertices_count) {
    }

    ListGraph(const IGraph& graph): vertices_f_to(graph.VerticesCount()), vertices_to_f(graph.VerticesCount())  {
        for (int i = 0; i < graph.VerticesCount(); ++i) {
            for (const auto& v: graph.GetNextVertices(i)) {
                AddEdge(i, v);
            }
        }
    }


    ~ListGraph() override = default ;

    // Добавление ребра от from к to.
    void AddEdge(int from, int to) override {
        vertices_f_to[from].push_back(to);
        vertices_to_f[to].push_back(from);

    }

    int VerticesCount() const override {
        return vertices_to_f.size();
    }

    std::vector<int> GetNextVertices(int vertex) const override {
        std::vector<int> result;

        result.resize(vertices_f_to[vertex].size());
        std::copy(vertices_f_to[vertex].begin(), vertices_f_to[vertex].end(), result.begin());

        return result;
    }

    std::vector<int> GetPrevVertices(int vertex) const override {
        std::vector<int> result;
        result.resize(vertices_to_f[vertex].size());
        std::copy(vertices_to_f[vertex].begin(), vertices_to_f[vertex].end(), result.begin());

        return result;
    }

private:
    std::vector<std::vector<int>> vertices_f_to;
    std::vector<std::vector<int>> vertices_to_f;
};


#endif //ALGOSI3_LISTGRAPH_H
