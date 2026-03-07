package ds

type SimpleMinPQ struct {
	heap []int

	size int
}

func NewSimpleMinPQ() *SimpleMinPQ {
	return &SimpleMinPQ{
		heap: make([]int, 10),
		size: 1,
	}
}

func (q *SimpleMinPQ) parent(x int) int {
	return x / 2
}

func (q *SimpleMinPQ) left(x int) int {
	return x * 2
}

func (q *SimpleMinPQ) right(x int) int {
	return x*2 + 1
}

func (q *SimpleMinPQ) Push(x int) {
	q.heap[q.size] = x
	q.swim(q.size)
	q.size++
}

func (q *SimpleMinPQ) swim(node int) {
	parent := q.parent(node)
	cur := node
	for q.heap[parent] > q.heap[cur] {
		q.heap[parent], q.heap[cur] = q.heap[cur], q.heap[parent]

		cur = parent
		parent = q.parent(parent)
	}
}

func (q *SimpleMinPQ) Pop() int {
	cur := q.heap[1]
	q.heap[1] = q.heap[q.size-1]
	q.sink(1)
	q.size--

	return cur
}

func (q *SimpleMinPQ) sink(node int) {
	for q.left(node) < q.size || q.right(node) < q.size {
		min := node
		if q.left(node) < q.size && q.heap[q.left(node)] < q.heap[min] {
			min = q.left(node)
		}

		if q.right(node) < q.size && q.heap[q.right(node)] < q.heap[min] {
			min = q.right(node)
		}

		if min == node {
			break
		}

		q.heap[min], q.heap[node] = q.heap[node], q.heap[min]
		node = min
	}
}
