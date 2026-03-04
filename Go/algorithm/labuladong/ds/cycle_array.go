package ds

import (
	"errors"
	"fmt"
)

type CycleArray[T any] struct {
	arr   []T
	start int
	end   int
	count int
	size  int
}

func NewCycleArray[T any]() *CycleArray[T] {
	return NewCycleArrayWithSize[T](1)
}

func NewCycleArrayWithSize[T any](size int) *CycleArray[T] {
	newArr := make([]T, size)

	return &CycleArray[T]{
		arr:  newArr,
		size: size,
	}
}

func (c *CycleArray[T]) resize(size int) {
	newArr := make([]T, size)
	for i := 0; i < c.count; i++ {
		newArr[i] = c.arr[(i+c.start)%c.size]
	}
	c.arr = newArr
	c.start = 0
	c.end = c.count
	c.size = size
}

func (c *CycleArray[T]) isFull() bool {
	return c.count == c.size
}

func (c *CycleArray[T]) AddFirst(value T) {
	if c.isFull() {
		c.resize(c.size * 2)
	}

	// start is closed interval, decrement start first
	c.start = (c.start - 1 + c.size) % c.size
	c.arr[c.start] = value
	c.count++
}

func (c *CycleArray[T]) AddLast(value T) {
	if c.isFull() {
		c.resize(c.size * 2)
	}

	c.arr[c.end] = value
	c.end = (c.end + 1) % c.size
	c.count++
}

func (c *CycleArray[T]) GetFirst() T {
	return c.arr[c.start]
}

func (c *CycleArray[T]) GetLast() T {
	return c.arr[c.end-1]
}

func (c *CycleArray[T]) DeleteFirst() error {
	if c.IsEmpty() {
		return errors.New("array empty")
	}

	c.arr[c.start] = *new(T)
	c.start = (c.start + 1) % c.size
	c.count--

	return nil
}

func (c *CycleArray[T]) DeleteLast() error {
	if c.IsEmpty() {
		return errors.New("array empty")
	}

	c.end = (c.end - 1 + c.size) % c.size
	c.arr[c.end] = *new(T)
	c.count--

	return nil
}

func (c *CycleArray[T]) PrintDebug() {
	fmt.Printf("start: %v, end %v\n", c.start, c.end)
	for _, a := range c.arr {
		fmt.Printf("%v ", a)
	}
	fmt.Println()
}

func (c *CycleArray[T]) IsEmpty() bool {
	return c.count == 0
}
