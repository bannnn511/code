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

func TestPrint(t *testing.T) {
	t.Helper()
	bst := Tree[string, int]{}
	bst.Put("d", 4)
	bst.Put("e", 5)
	bst.Put("b", 2)
	bst.Put("a", 1)
	bst.Put("c", 3)
	bst.Put("g", 7)
	bst.Put("f", 6)
	bst.PrintDFS(-1)
	println()
	bst.PrintDFS(-1)
	println()
	bst.PrintDFS(-1)

	keys := bst.LevelOrder(0)
	if len(keys) != 1 {
		t.Errorf("len(keys)= %d, want %d", len(keys), 1)
	}

	keys = bst.LevelOrder(1)
	if len(keys) != 3 {
		t.Errorf("len(keys)= %d, want %d", len(keys), 3)
	}
	if keys[0] != "d" || keys[1] != "b" || keys[2] != "e" {
		t.Errorf("keys= %v, want %v", keys, []string{"d", "b", "c"})
	}

	keys = bst.LevelOrder(2)
	if len(keys) != 6 {
		t.Errorf("len(keys)= %d, want %d", len(keys), 6)
	}

	if keys[0] != "d" || keys[1] != "b" || keys[2] != "e" || keys[3] != "a" || keys[4] != "c" || keys[5] != "g" {
		t.Errorf("keys= %v, want %v", keys, []string{"d", "b", "e", "a", "c", "g"})
	}
}
