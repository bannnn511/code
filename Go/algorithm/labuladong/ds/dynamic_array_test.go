package ds

import (
	"testing"

	"github.com/stretchr/testify/assert"
)

func TestMyDynamicArray(t *testing.T) {
	arr := NewDynamicArray()

	arr.AddAtHead(1)
	assert.Equal(t, 1, arr.array[0])

	arr.AddAtTail(3)
	assert.Equal(t, 3, arr.array[1])
}

func TestMyDynamicArray2(t *testing.T) {

	arr := NewDynamicArray()

	arr.AddAtHead(1)
	assert.Equal(t, 1, arr.array[0])

	arr.DeleteAtIndex(0)
	assert.Equal(t, 0, arr.size)
}

func TestMyDynamicArray3(t *testing.T) {
	arr := NewDynamicArray()

	arr.AddAtHead(2)
	arr.DeleteAtIndex(1)
	arr.AddAtHead(2)
	arr.AddAtHead(7)
	arr.AddAtHead(3)
	arr.AddAtHead(2)
	arr.AddAtHead(5)
	arr.AddAtTail(5)
	arr.DeleteAtIndex(6)
	arr.DeleteAtIndex(4)
}
