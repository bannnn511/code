package ds

import (
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

	assert.Equal(t, pq.Pop(), 1)
	assert.Equal(t, pq.Pop(), 2)
	assert.Equal(t, pq.Pop(), 3)
	assert.Equal(t, pq.Pop(), 4)
	assert.Equal(t, pq.Pop(), 5)
}
