package algorithm

import (
	"fmt"
	"slices"
	"testing"
)

func TestQuickSort1(t *testing.T) {
	a := []int{2, 3, 1, 5, 4}
	QuickSort(a)
	if !IsSorted(a) {
		t.Errorf("   got %v", a)
	}
	fmt.Println(a)
}

func TestQuickSortInts(t *testing.T) {
	data := slices.Clone(ints[:])
	QuickSort(data)
	if !IsSorted(data) {
		t.Errorf("sorted %v", ints)
		t.Errorf("   got %v", data)
	}
}

func TestQuickSortFloat64(t *testing.T) {
	data := slices.Clone(float64s[:])
	QuickSort(data)
	if !IsSorted(data) {
		t.Errorf("sorted %v", float64s)
		t.Errorf("   got %v", data)
	}
}

func TestQuickSortString(t *testing.T) {
	data := slices.Clone(strs[:])
	QuickSort(data)
	if !IsSorted(data) {
		t.Errorf("sorted %v", strs)
		t.Errorf("   got %v", data)
	}
}
