package sbtable

import (
	"fmt"
)

type Node[K, V Ordered] struct {
	key   K
	value V
	left  *Node[K, V]
	right *Node[K, V]
	n     int
}

func newNode[K, V Ordered](key K, v V, n int) *Node[K, V] {
	return &Node[K, V]{
		key:   key,
		value: v,
		n:     n,
	}
}

type Tree[K, V Ordered] struct {
	root *Node[K, V]
}

var _ ISymbolTale[int, int] = &Tree[int, int]{}

// Contains implements ISymbolTale.
func (t *Tree[K, V]) Contains(key K) bool {
	_, ok := t.Get(key)
	return ok
}

// Delete implements ISymbolTale.
func (t *Tree[K, V]) Delete(key K) {
	panic("unimplemented")
}

func (t *Tree[K, V]) get(node Node[K, V], key K) (V, bool) {
	var result V
	if node.key > key {
		if node.right != nil {
			return t.get(*node.right, key)
		}
	} else {
		if node.left != nil {
			return t.get(*node.left, key)
		}
	}

	if node.key == key {
		return node.value, true
	}

	return result, false
}

// Get implements ISymbolTale.
func (t *Tree[K, V]) Get(key K) (V, bool) {
	return t.get(*t.root, key)
}

// IsEmpty implements ISymbolTale.
func (t *Tree[K, V]) IsEmpty() bool {
	return t.root.n > 0
}

func (t *Tree[K, V]) put(node *Node[K, V], key K, value V) *Node[K, V] {
	if node == nil {
		return newNode(key, value, 1)
	}
	if key > node.key {
		node.right = t.put(node.right, key, value)
	} else if key < node.key {
		node.left = t.put(node.left, key, value)
	} else {
		node.value = value
	}
	node.n = t.size(node.left) + t.size(node.right) + 1

	return node
}

// Put implements ISymbolTale.
func (t *Tree[K, V]) Put(key K, value V) {
	if t.root == nil {
		t.root = newNode(key, value, 1)
		return
	}

	t.root = t.put(t.root, key, value)
}

func (t *Tree[K, V]) size(node *Node[K, V]) int {
	if node == nil {
		return 0
	}

	return node.n
}

// Size implements ISymbolTale.
func (t *Tree[K, V]) Size() int {
	if t.root == nil {
		return 0
	}

	return t.size(t.root)
}

func (t *Tree[K, V]) printDFS(node *Node[K, V], order int) {
	if node == nil || order == -1 {
		return
	}
	if order == 0 {
		fmt.Printf("%v ", node.key)
		t.printDFS(node.left, 0)
		t.printDFS(node.right, 0)
	} else if order == 1 {
		t.printDFS(node.left, 1)
		fmt.Printf("%v ", node.key)
		t.printDFS(node.right, 1)
	} else {
		t.printDFS(node.left, 2)
		t.printDFS(node.right, 2)
		fmt.Printf("%v ", node.key)
	}
}

func (t *Tree[K, V]) PrintDFS(order int) {
	t.printDFS(t.root, order)
}

func (t *Tree[K, V]) LevelOrder(level int) []K {
	if t.root == nil {
		return nil
	}

	queue := []*Node[K, V]{t.root}
	counter := 0
	var keys []K
	for len(queue) > 0 {
		if counter > level {
			return keys
		}
		for _, node := range queue {
			if node.left != nil {
				queue = append(queue, node.left)
			}
			if node.right != nil {
				queue = append(queue, node.right)
			}

			keys = append(keys, node.key)
			queue = queue[1:]
		}
		counter++
	}

	return keys
}
