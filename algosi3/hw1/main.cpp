#include <iostream>
#include <assert.h>
#include "ListGraph.h"
#include "MatrixGraph.h"
#include "SetGraph.h"
#include "ArcGraph.h"
#include <algorithm>

void assert_vector(std::vector<int>lhs, std::vector<int> rhs ) {
    std::sort(lhs.begin(), lhs.end());
    std::sort(rhs.begin(), rhs.end());

    assert(lhs.size() == rhs.size());
    for (int j = 0; j < lhs.size(); ++j) {
        assert(lhs[j] == rhs[j]);
    }
}

void TESTLISTGRAPH() {
    MatrixGraph init_graph(8);
    init_graph.AddEdge(0, 6);
    init_graph.AddEdge(0, 5);
    init_graph.AddEdge(5, 2);
    init_graph.AddEdge(2, 1);
    init_graph.AddEdge(1, 4);
    init_graph.AddEdge(3, 1);
    init_graph.AddEdge(1, 0);

    ListGraph graph(init_graph);

    assert_vector(graph.GetPrevVertices(1), {2, 3});
    assert_vector(graph.GetPrevVertices(2), {5});
    assert_vector(graph.GetPrevVertices(3), {});
    assert_vector(graph.GetNextVertices(0), {5, 6});
    assert_vector(graph.GetNextVertices(1), {0, 4});
    assert_vector(graph.GetNextVertices(6), {});
}

void TESTMATRIXGRAPH() {
    SetGraph init_graph(8);
    init_graph.AddEdge(0, 6);
    init_graph.AddEdge(0, 5);
    init_graph.AddEdge(5, 2);
    init_graph.AddEdge(2, 1);
    init_graph.AddEdge(1, 4);
    init_graph.AddEdge(3, 1);
    init_graph.AddEdge(1, 0);

    MatrixGraph graph(init_graph);

    assert_vector(graph.GetPrevVertices(1), {2, 3});
    assert_vector(graph.GetPrevVertices(2), {5});
    assert_vector(graph.GetPrevVertices(3), {});
    assert_vector(graph.GetNextVertices(0), {5, 6});
    assert_vector(graph.GetNextVertices(1), {0, 4});
    assert_vector(graph.GetNextVertices(6), {});

}

void TESTSETGRAPH() {
    ArcGraph init_graph(8);
    init_graph.AddEdge(0, 6);
    init_graph.AddEdge(0, 5);
    init_graph.AddEdge(5, 2);
    init_graph.AddEdge(2, 1);
    init_graph.AddEdge(1, 4);
    init_graph.AddEdge(3, 1);
    init_graph.AddEdge(1, 0);

    SetGraph graph(init_graph);

    assert_vector(graph.GetPrevVertices(1), {2, 3});
    assert_vector(graph.GetPrevVertices(2), {5});
    assert_vector(graph.GetPrevVertices(3), {});
    assert_vector(graph.GetNextVertices(0), {5, 6});
    assert_vector(graph.GetNextVertices(1), {0, 4});
    assert_vector(graph.GetNextVertices(6), {});

}

void TESTARCGRAPH() {
    ListGraph init_graph(8);
    init_graph.AddEdge(0, 6);
    init_graph.AddEdge(0, 5);
    init_graph.AddEdge(5, 2);
    init_graph.AddEdge(2, 1);
    init_graph.AddEdge(1, 4);
    init_graph.AddEdge(3, 1);
    init_graph.AddEdge(1, 0);

    ArcGraph graph(init_graph);

    assert_vector(graph.GetPrevVertices(1), {2, 3});
    assert_vector(graph.GetPrevVertices(2), {5});
    assert_vector(graph.GetPrevVertices(3), {});
    assert_vector(graph.GetNextVertices(0), {5, 6});
    assert_vector(graph.GetNextVertices(1), {0, 4});
    assert_vector(graph.GetNextVertices(6), {});

}

void TESTALL() {
    TESTLISTGRAPH();
    TESTMATRIXGRAPH();
    TESTSETGRAPH();
    TESTARCGRAPH();

    ListGraph init_graph(8);
    init_graph.AddEdge(0, 6);
    init_graph.AddEdge(0, 5);
    init_graph.AddEdge(5, 2);
    init_graph.AddEdge(2, 1);
    init_graph.AddEdge(1, 4);
    init_graph.AddEdge(3, 1);
    init_graph.AddEdge(1, 0);

    MatrixGraph minit_graph(init_graph);
    SetGraph sinit_graph(minit_graph);
    ArcGraph graph(sinit_graph);

    assert_vector(graph.GetPrevVertices(1), {2, 3});
    assert_vector(graph.GetPrevVertices(2), {5});
    assert_vector(graph.GetPrevVertices(3), {});
    assert_vector(graph.GetNextVertices(0), {5, 6});
    assert_vector(graph.GetNextVertices(1), {0, 4});
    assert_vector(graph.GetNextVertices(6), {});

}

int main() {
    TESTALL();
    return 0;
}