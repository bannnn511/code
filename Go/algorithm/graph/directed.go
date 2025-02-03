package graph

import (
	"fmt"
	"iter"
)

type Digraph struct {
	VCount int    // number of vertices
	ECount int    // number of edges
	adj    []*Bag // adjacency list
}

var _ IGraph = (*Digraph)(nil)

func NewDigraph(v int) *Digraph {
	d := &Digraph{}
	d.Graph(v)

	return d
}

func (d *Digraph) Graph(v int) {
	d.VCount = v
	d.adj = make([]*Bag, v)
	for i := range d.adj {
		d.adj[i] = NewBag()
	}
}

func (d *Digraph) V() int {
	return d.VCount
}

func (d *Digraph) E() int {
	return d.ECount
}

func (d *Digraph) AddEdge(v int, w int) {
	d.ECount++
	d.adj[v].Add(w)
}

func (d *Digraph) Adj(v int) iter.Seq[int] {
	return d.adj[v].All()
}

func (d *Digraph) String() string {
	s := fmt.Sprintf("%d Vertices, %d Edges\n", d.VCount, d.ECount)
	for v := 0; v < d.VCount; v++ {
		s += fmt.Sprintf("%d: ", v)
		for w := range d.adj[v].All() {
			s += fmt.Sprintf("%d ", w)
		}
		s += "\n"
	}

	return s
}

func (d *Digraph) Reverse() *Digraph {
	r := NewDigraph(d.VCount)
	for v := 0; v < d.VCount; v++ {
		for w := range d.Adj(v) {
			r.AddEdge(w, v)
		}
	}

	return r
}
