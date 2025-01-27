package graph

import "iter"

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
