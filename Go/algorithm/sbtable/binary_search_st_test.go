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

	v, _ := bst.Get("a")
	if v != 1 {
		t.Errorf("bst.Get(a) = %d, want: %d", v, 1)
	}

	if v1, _ := bst.Get("c"); v1 != 0 {
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

	if ok := bst.Contains("b"); !ok {
		t.Errorf("bst.Contains(b) = %v, want: %v", ok, true)
	}

	if ok := bst.Contains("z"); ok {
		t.Errorf("bst.Contains(z) = %v, want: %v", ok, false)
	}
}

func TestBinarySearchST_resize(t *testing.T) {
	bst := NewBST[int, string]()
	bst.Put(1, "a")

	bst.Put(2, "b")
	if v, _ := bst.Get(2); v != "b" {
		t.Errorf("bst.Get(2) = %v, want %v", v, "b")
	}

	bst.Put(3, "c")

	if bst.capacity != 4 {
		t.Errorf("bst.length = %d, want %d", bst.length, 4)
	}

	if v, _ := bst.Get(1); v != "a" {
		t.Errorf("bst.Get(1) = %v, want %v", v, "a")
	}

	if v, _ := bst.Get(3); v != "c" {
		t.Errorf("bst.Get(3) = %v, want %v", v, "c")
	}
}

func TestBinarySearchST_Delete(t *testing.T) {
	bst := NewBST[string, int]()
	bst.Put("a", 1)
	bst.Put("b", 2)
	bst.Put("c", 3)

	// Test deleting an existing key
	bst.Delete("b")
	if bst.Contains("b") {
		t.Errorf("bst.Contains(b) = true, want: false")
	}
	if bst.Size() != 2 {
		t.Errorf("bst.Size() = %d, want: %d", bst.Size(), 2)
	}

	// Test deleting a non-existing key
	bst.Delete("d")
	if bst.Size() != 2 {
		t.Errorf("bst.Size() = %d, want: %d", bst.Size(), 2)
	}

	// Test deleting the first key
	bst.Delete("a")
	if bst.Contains("a") {
		t.Errorf("bst.Contains(a) = true, want: false")
	}
	if bst.Size() != 1 {
		t.Errorf("bst.Size() = %d, want: %d", bst.Size(), 1)
	}

	// Test deleting the last key
	bst.Delete("c")
	if bst.Contains("c") {
		t.Errorf("bst.Contains(c) = true, want: false")
	}
	if !bst.IsEmpty() {
		t.Errorf("bst.IsEmpty() = false, want: true")
	}
}
