package sbtable

import "testing"

func TestBST(t *testing.T) {
	bst := Tree[string, int]{}
	bst.Put("a", 1)
	if v, ok := bst.Get("a"); !ok && v != 1 {
		t.Errorf("bst.Get(\"a\")= %d, want %d", v, 1)
	}

	if v, ok := bst.Get("b"); ok || v != 0 {
		t.Errorf("bst.Get(\"b\")= %d, want %d", v, 0)
	}

	if size := bst.Size(); size != 1 {
		t.Errorf("bst.Size()= %d, want %d", size, 1)
	}

	bst.Put("b", 2)
	if size := bst.Size(); size != 2 {
		t.Errorf("bst.Size()= %d, want %d", size, 2)
	}
}
