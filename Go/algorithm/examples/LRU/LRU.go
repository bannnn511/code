package main

type node struct {
	key   int
	value int
	next  *node
	prev  *node
}

func newNode(key, value int) *node {
	return &node{
		key:   key,
		value: value,
	}
}

type LRUCache struct {
	count int
	cap   int
	cache []*node
	head  *node
	tail  *node
}

func Constructor(capacity int) LRUCache {
	head := new(node)
	tail := new(node)
	head.next = tail
	tail.prev = head

	return LRUCache{
		cap:   capacity,
		cache: make([]*node, 10000),
		head:  head,
		tail:  tail,
	}
}

func (l *LRUCache) Get(key int) int {
	n := l.cache[key]
	if n == nil {
		return -1
	}

	l.removeNode(n)
	l.addNode(n)
	return n.value
}

func (l *LRUCache) Put(key int, value int) {
	if n := l.cache[key]; n != nil {
		l.removeNode(n)
		n.value = value
		l.addNode(n)
		return
	}

	if l.count >= l.cap {
		l.removeLastUse()
	}

	n := newNode(key, value)
	l.cache[key] = n
	l.addNode(n)
	l.count++
}

func (l *LRUCache) removeLastUse() {
	tmp := l.tail.prev
	l.removeNode(tmp)
	l.cache[tmp.key] = nil
}

func (l *LRUCache) removeNode(n *node) {
	if n == nil {
		return
	}
	prevNode := n.prev
	nextNode := n.next
	prevNode.next = nextNode
	nextNode.prev = prevNode
}

func (l *LRUCache) addNode(leaf *node) {
	if leaf == nil {
		return
	}
	tmp := l.head.next
	tmp.prev = leaf
	leaf.next = tmp
	leaf.prev = l.head
	l.head.next = leaf
}
