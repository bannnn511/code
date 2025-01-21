package graph

import "iter"

type Graph interface {
	Graph(v int)
	V() int
	E() int
	AddEdge(int, int)
	Adj(v int) iter.Seq[int]
	String() string
}

var _ Graph = (*AdjacencyList)(nil)

type Node struct {
	v    int
	next *Node
}

func NewNode(v int) *Node {
	return &Node{
		v:    v,
		next: nil,
	}
}

type Bag struct {
	first *Node
}

func NewBag() *Bag {
	return &Bag{}
}

func (b *Bag) All() iter.Seq[int] {
	return func(yield func(int) bool) {

		for e := b.first; e != nil; e = e.next {
			if !yield(e.v) {
				return
			}
		}
	}
}

func (b *Bag) Add(v int) {
	if b.first == nil {
		b.first = NewNode(v)
		return
	}
	olfFirst := b.first
	first := NewNode(v)
	first.next = olfFirst
	b.first = first
}

type AdjacencyList struct {
	VCount int    // number of vertices
	ECount int    // number of edges
	adj    []*Bag // adjacency list
}

func NewAdjacencyList(v int) *AdjacencyList {
	l := &AdjacencyList{}
	l.Graph(v)

	return l
}

func (a *AdjacencyList) Graph(v int) {
	a.VCount = v
	a.adj = make([]*Bag, v)
	for i := range a.adj {
		a.adj[i] = NewBag()
	}
}

func (a *AdjacencyList) V() int {
	return a.VCount
}

func (a *AdjacencyList) E() int {
	return a.ECount
}

func (a *AdjacencyList) AddEdge(v int, w int) {
	a.adj[v].Add(w)
	a.adj[w].Add(v)
	a.ECount++
}

func (a *AdjacencyList) Adj(v int) iter.Seq[int] {
	return a.adj[v].All()
}

func (a *AdjacencyList) String() string {
	// TODO implement me
	panic("implement me")
}
