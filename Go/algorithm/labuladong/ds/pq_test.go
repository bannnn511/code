package ds

import (
	"container/heap"
	"testing"

	"github.com/stretchr/testify/assert"
)

func TestPQ(t *testing.T) {
	pq := NewSimpleMinPQ()
	pq.Push(3)
	pq.Push(2)
	pq.Push(1)
	pq.Push(5)
	pq.Push(4)

	assert.Equal(t, 1, pq.Pop())
	assert.Equal(t, 2, pq.Pop())
	assert.Equal(t, 3, pq.Pop())
	assert.Equal(t, 4, pq.Pop())
	assert.Equal(t, 5, pq.Pop())
}

func TestPriorityQueue(t *testing.T) {
	pq := PriorityQueue{}
	heap.Init(&pq)

	heap.Push(&pq, 3)
	heap.Push(&pq, 2)
	heap.Push(&pq, 1)
	heap.Push(&pq, 5)
	heap.Push(&pq, 4)

	assert.Equal(t, 1, heap.Pop(&pq))
	assert.Equal(t, 2, heap.Pop(&pq))
	assert.Equal(t, 3, heap.Pop(&pq))
	assert.Equal(t, 4, heap.Pop(&pq))
	assert.Equal(t, 5, heap.Pop(&pq))
}
