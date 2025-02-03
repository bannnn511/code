package graph

import "testing"

func TestPath(t *testing.T) {
	g := NewUndirected(6)

	g.AddEdge(0, 5)
	g.AddEdge(2, 4)
	g.AddEdge(2, 3)
	g.AddEdge(1, 2)
	g.AddEdge(0, 1)
	g.AddEdge(3, 4)
	g.AddEdge(3, 5)
	g.AddEdge(0, 2)

	path := NewPath(g, 0)

	for _, e := range path.PathTo(4) {
		println(e)
	}
}
