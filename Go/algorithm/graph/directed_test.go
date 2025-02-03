package graph

import (
	"testing"
)

func TestDirected(t *testing.T) {
	g := NewDigraph(5)

	if g.V() != 5 {
		t.Errorf("g.V() = %d, want: %d", g.V(), 5)
	}

	if g.E() != 0 {
		t.Errorf("g.E() = %d, want: %d", g.E(), 0)
	}

	g.AddEdge(0, 1)
	g.AddEdge(0, 2)
	g.AddEdge(1, 2)
	g.AddEdge(1, 3)
	g.AddEdge(3, 4)

	if g.E() != 5 {
		t.Errorf("g.E() = %d, want: %d", g.E(), 5)
	}

	expectedAdj := map[int][]int{
		0: {2, 1},
		1: {3, 2},
		3: {4},
	}

	for v, expected := range expectedAdj {
		var result []int

		for x := range g.Adj(v) {
			result = append(result, x)
		}

		if len(result) != len(expected) {
			t.Errorf("len(g.Adj(%d)) = %d, want: %d", v, len(result), len(expected))
		}

		for i, w := range result {
			if w != expected[i] {
				t.Errorf("g.Adj(%d)[%d] = %d, want: %d", v, i, w, expected[i])
			}
		}
	}
}
