package main

import (
	"fmt"
	"testing"
)

func TestLRU(t *testing.T) {
	lru := Constructor(2)
	lru.Put(1, 2)
	lru.Put(2, 3)
	lru.Put(1, 5)
	lru.Put(4, 5)
	lru.Put(6, 7)
	v := lru.Get(4)
	fmt.Println("value", v)
	lru.Put(1, 2)
	v = lru.Get(3)
	fmt.Println("value", v)

	v = lru.Get(1)
	fmt.Println("value", v)
}

func TestLRU2(t *testing.T) {
	lru := Constructor(2)
	lru.Put(1, 1)
	lru.Put(2, 2)
	if res := lru.Get(1); res != 1 {
		t.Errorf("expected 1 got %d", res)
	}
	lru.Put(3, 3)
	if res := lru.Get(2); res != -1 {
		t.Errorf("expected -1 got %d", res)
	}
	lru.Put(4, 4)
	if res := lru.Get(1); res != -1 {
		t.Errorf("expected -1 got %d", res)
	}
	if res := lru.Get(3); res != 3 {
		t.Errorf("expected 3 got %d", res)
	}
	if res := lru.Get(4); res != 4 {
		t.Errorf("expected 4 got %d", res)
	}
}
func TestLRU3(t *testing.T) {
	lru := Constructor(2)
	if res := lru.Get(2); res != -1 {
		t.Errorf("expected -1 got %d", res)
	}
	lru.Put(2, 6)
	if res := lru.Get(1); res != -1 {
		t.Errorf("expected -1 got %d", res)
	}
	lru.Put(1, 5)
	lru.Put(1, 2)

	if res := lru.Get(1); res != 2 {
		t.Errorf("expected 2 got %d", res)
	}

	if res := lru.Get(2); res != 6 {
		t.Errorf("expected -1 got %d", res)
	}
}
