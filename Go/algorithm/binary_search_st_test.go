package algorithm

import (
	"testing"
)

func TestBinarySearchST_Put(t *testing.T) {
	bst := NewBinarySearchST[string, int](2)
	bst.Put("a", 1)

	test1 := bst.Rank("a")
	if test1 != 0 {
		t.Errorf("bst.Rank(a) = %d, want: %d", test1, 0)
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

	test3 := bst.Rank("c")
	if test3 != 0 {
		t.Errorf("bst.Rank(c) = %d, want: %d", test3, 0)
	}

	test3 = bst.Rank("d")
	if test3 != 0 {
		t.Errorf("bst.Rank(c) = %d, want: %d", test3, 0)
	}
}
