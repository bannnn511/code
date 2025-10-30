package ds

import (
	"fmt"
	"testing"

	"github.com/stretchr/testify/assert"
)

func TestMyLinkedList(t *testing.T) {
	list := NewLinkedList()

	list.AddAtHead(1)
	fmt.Println(list.String())
	list.AddAtTail(3)
	fmt.Println(list.String())
	list.AddAtIndex(1, 2)
	fmt.Println(list.String())
	result1 := list.Get(1)
	list.DeleteAtIndex(1)
	fmt.Println(list.String())
	result2 := list.Get(1)

	assert.Equal(t, 2, result1)
	assert.Equal(t, 3, result2)
}

// ...existing code...

func TestMyLinkedList2(t *testing.T) {
	list := NewLinkedList()

	list.AddAtHead(7)
	fmt.Println(list.String())

	list.AddAtHead(2)
	fmt.Println(list.String())

	list.AddAtHead(1)
	fmt.Println(list.String())

	list.AddAtIndex(3, 0)
	fmt.Println(list.String())

	list.DeleteAtIndex(2)
	fmt.Println(list.String())

	list.AddAtHead(6)
	fmt.Println(list.String())

	list.AddAtTail(4)
	fmt.Println(list.String())

	result1 := list.Get(4)
	fmt.Printf("Get(4) = %d\n", result1)

	list.AddAtHead(4)
	fmt.Println(list.String())

	list.AddAtIndex(5, 0)
	fmt.Println(list.String())

	list.AddAtHead(6)
	fmt.Println(list.String())

	assert.Equal(t, 4, result1)
}
