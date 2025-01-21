package main

import (
	"fmt"
	"testing"
)

func TestLRU(t *testing.T) {
	lru := NewLRU(2)
	lru.Set(1, 2)
	lru.Set(2, 3)
	lru.Set(1, 5)
	lru.Set(4, 5)
	lru.Set(6, 7)
	v := lru.Get(4)
	fmt.Println("value", v)
	lru.Set(1, 2)
	v = lru.Get(3)
	fmt.Println("value", v)

	v = lru.Get(1)
	fmt.Println("value", v)
}
