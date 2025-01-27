package graph

import "testing"

func TestDFS(t *testing.T) {
	g := NewAdjacencyList(6)

	g.AddEdge(0, 5)
	g.AddEdge(2, 4)
	g.AddEdge(2, 3)
	g.AddEdge(1, 2)
	g.AddEdge(0, 1)
	g.AddEdge(3, 4)
	g.AddEdge(3, 5)
	g.AddEdge(0, 2)

	DFS(g, 0)

	if got := Count(g, 0); got != 6 {
		t.Errorf("Count()=%d, want: %d", got, 6)
	}
}
