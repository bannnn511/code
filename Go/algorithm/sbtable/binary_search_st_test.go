package sbtable

import (
	"testing"
)

func TestBinarySearchST_Put(t *testing.T) {
	bst := NewBST[string, int]()
	bst.Put("a", 1)

	test1 := bst.Rank("a")
	if test1 != 0 {
		t.Errorf("bst.Rank(a) = %d, want: %d", test1, 0)
	}

	v := bst.Get("a")
	if v != 1 {
		t.Errorf("bst.Get(a) = %d, want: %d", v, 1)
	}

	if v1 := bst.Get("c"); v1 != 0 {
		t.Errorf("bst.Get(c) = %d, want: %d", v, 1)
	}

	bst.Put("b", 2)
	test2 := bst.Rank("b")
	if test2 != 0 {
		t.Errorf("bst.Rank(b) = %d, want: %d", test2, 0)
	}

	test1 = bst.Rank("a")
	if test1 != 1 {
		t.Errorf("bst.Rank(a) = %d, want: %d", test1, 1)
	}
}

func TestBinarySearchST_resize(t *testing.T) {
	bst := NewBST[int, string]()
	bst.Put(1, "a")

	bst.Put(2, "b")
	if v := bst.Get(2); v != "b" {
		t.Errorf("bst.Get(2) = %v, want %v", v, "b")
	}

	bst.Put(3, "c")

	if bst.capacity != 4 {
		t.Errorf("bst.length = %d, want %d", bst.length, 4)
	}

	if v := bst.Get(1); v != "a" {
		t.Errorf("bst.Get(1) = %v, want %v", v, "a")
	}

	if v := bst.Get(3); v != "c" {
		t.Errorf("bst.Get(3) = %v, want %v", v, "c")
	}
}
