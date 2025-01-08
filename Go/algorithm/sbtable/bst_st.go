package sbtable

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
	panic("unimplemented")
}

func (t *Tree[K, V]) put(node *Node[K, V], key K, value V) *Node[K, V] {
	if node == nil {
		return newNode(key, value, 1)
	}
	if node.key > key {
		node.right = t.put(node.right, key, value)
	} else if node.key < key {
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
