package graph

import (
	"fmt"
	"iter"
)

type IGraph interface {
	Graph(v int)
	V() int
	E() int
	AddEdge(int, int)
	Adj(v int) iter.Seq[int]
	String() string
}

var _ IGraph = (*Undirected)(nil)

type Undirected struct {
	VCount int    // number of vertices
	ECount int    // number of edges
	adj    []*Bag // adjacency list
}

func NewUndirected(v int) *Undirected {
	l := &Undirected{}
	l.Graph(v)

	return l
}

func (a *Undirected) Graph(v int) {
	a.VCount = v
	a.adj = make([]*Bag, v)
	for i := range a.adj {
		a.adj[i] = NewBag()
	}
}

func (a *Undirected) V() int {
	return a.VCount
}

func (a *Undirected) E() int {
	return a.ECount
}

func (a *Undirected) AddEdge(v int, w int) {
	a.adj[v].Add(w)
	a.adj[w].Add(v)
	a.ECount++
}

func (a *Undirected) Adj(v int) iter.Seq[int] {
	return a.adj[v].All()
}

func (a *Undirected) String() string {
	s := fmt.Sprintf("%d Vertices, %d Edges\n", a.VCount, a.ECount)
	for v := 0; v < a.VCount; v++ {
		s += fmt.Sprintf("%d: ", v)
		for w := range a.adj[v].All() {
			s += fmt.Sprintf("%d ", w)
		}
		s += "\n"
	}

	return s
}

func (a *Undirected) Degree(v int) int {
	degree := 0
	for range a.adj[v].All() {
		degree++
	}
	return degree
}

func (a *Undirected) MaxDegree() int {
	maxDegree := 0
	for i := 0; i < a.VCount; i++ {
		maxDegree = max(maxDegree, a.Degree(i))
	}

	return maxDegree
}

func (a *Undirected) HasEdge(v, w int) bool {
	for e := range a.Adj(v) {
		if e == w {
			return true
		}
	}

	return false
}
