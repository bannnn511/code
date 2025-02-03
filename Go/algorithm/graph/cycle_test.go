package graph

import "testing"

func TestIsCycle(t *testing.T) {
	g := NewUndirected(13)

	edges := [][2]int{
		{0, 6},
		{1, 0},
		{2, 1},
		{3, 4},
		{5, 4},
		{6, 4},
	}

	for _, edge := range edges {
		g.AddEdge(edge[0], edge[1])
	}

	if got := IsCycle(g, 0); got {
		t.Errorf("IsCycle()= %v, want: %v", got, false)
	}

	g.AddEdge(1, 1)
	if got := IsCycle(g, 0); !got {
		t.Errorf("IsCycle()= %v, want: %v", got, true)
	}
}
