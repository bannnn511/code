package algorithm

import (
	"fmt"
	"math"
	"slices"
	"testing"
)

var ints = [...]int{74, 59, 238, -784, 9845, 959, 905, 0, 0, 42, 7586, -5467984, 7586}
var float64s = [...]float64{74.3, 59.0, math.Inf(1), 238.2, -784.0, 2.3, math.Inf(-1), 9845.768, -959.7485, 905, 7.8, 7.8, 74.3, 59.0, math.Inf(1), 238.2, -784.0, 2.3}
var strs = [...]string{"", "Hello", "foo", "bar", "foo", "f00", "%*&^*&^&", "***"}

func TestMergeSortInts(t *testing.T) {
	data := slices.Clone(ints[:])
	MergeSort(data)
	if !IsSorted(data) {
		t.Errorf("sorted %v", ints)
		t.Errorf("   got %v", data)
	}
}

func TestMergeSortFloat64(t *testing.T) {
	data := slices.Clone(float64s[:])
	MergeSort(data)
	if !IsSorted(data) {
		t.Errorf("sorted %v", float64s)
		t.Errorf("   got %v", data)
	}
}

func TestMergeSortSBUtring(t *testing.T) {
	data := slices.Clone(strs[:])
	MergeSort(data)
	if !IsSorted(data) {
		t.Errorf("sorted %v", strs)
		t.Errorf("   got %v", data)
	}
}
func TestMergeSortBUInts(t *testing.T) {
	data := slices.Clone(ints[:])
	MergeSortBU(data)
	if !IsSorted(data) {
		t.Errorf("sorted %v", ints)
		t.Errorf("   got %v", data)
	}
}

func TestMergeSortBUFloat64(t *testing.T) {
	data := slices.Clone(float64s[:])
	MergeSortBU(data)
	if !IsSorted(data) {
		t.Errorf("sorted %v", float64s)
		t.Errorf("   got %v", data)
	}
}

func TestMergeSortString(t *testing.T) {
	data := slices.Clone(strs[:])
	MergeSortBU(data)
	if !IsSorted(data) {
		t.Errorf("sorted %v", strs)
		t.Errorf("   got %v", data)
	}
}

func TestMergeSort1(t *testing.T) {
	data := []int{5, 2, 4, 1, 7, 4, 6, 9}
	MergeSortBU(data)
	fmt.Println(data)
}
