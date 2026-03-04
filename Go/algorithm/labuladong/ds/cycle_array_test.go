package ds_test

import (
	"testing"

	"github.com/bannnn511/code/go/algorithm/labuladong/ds"
)

func TestCycleArray_AddFirst(t *testing.T) {
	ca := ds.NewCycleArray[int]()
	ca.AddFirst(2)
	ca.PrintDebug()
	// [2]

	if first := ca.GetFirst(); first != 2 {
		t.Errorf("exptected %d, got %d", 2, first)
	}

	ca.AddFirst(3)
	ca.PrintDebug()
	// [2,3]
	if first := ca.GetFirst(); first != 3 {
		t.Errorf("exptected %d, got %d", 3, first)
	}

	ca.AddFirst(4)
	ca.PrintDebug()
	// [2,3,_,4]
	if first := ca.GetFirst(); first != 4 {
		t.Errorf("exptected %d, got %d", 4, first)
	}

	ca.AddLast(6)
	// [2,3,6,4]
	ca.PrintDebug()
	if first := ca.GetFirst(); first != 4 {
		t.Errorf("exptected %d, got %d", 4, first)
	}

	if last := ca.GetLast(); last != 6 {
		t.Errorf("exptected %d, got %d", 6, last)
	}

	_ = ca.DeleteFirst()
	// [2,3,6, _]
	if first := ca.GetFirst(); first != 3 {
		t.Errorf("exptected %d, got %d", 3, first)
	}

	_ = ca.DeleteFirst()
	// [2,_,6, _]
	if first := ca.GetFirst(); first != 2 {
		t.Errorf("exptected %d, got %d", 2, first)
	}

	_ = ca.DeleteLast()
	// [2,_,_, _]
	if last := ca.GetLast(); last != 2 {
		t.Errorf("exptected %d, got %d", 2, last)
	}

	if first := ca.GetFirst(); first != 2 {
		t.Errorf("exptected %d, got %d", 2, first)
	}
}
