package sbtable

import (
	"cmp"
)

type BinarySearchST[T cmp.Ordered, K cmp.Ordered] struct {
	keys     []T
	values   []K
	length   int
	capacity int
}

func NewBST[T, K cmp.Ordered]() *BinarySearchST[T, K] {
	keys := make([]T, 2)
	values := make([]K, 2)
	return &BinarySearchST[T, K]{
		keys:     keys,
		values:   values,
		length:   0,
		capacity: 2,
	}
}

func (t *BinarySearchST[T, K]) Size() int {
	return t.length
}

func (t *BinarySearchST[T, K]) Put(key T, value K) {
	i := t.Rank(key)
	if i < t.length && t.keys[i] == key {
		t.values[i] = value
		return
	}

	for idx := t.length; idx > i; idx-- {
		t.keys[idx], t.values[idx] = t.keys[idx-1], t.values[idx-1]
	}
	t.keys[i] = key
	t.values[i] = value
	t.length++
	if t.length == t.capacity {
		t.resize(2 * t.capacity)
	}
}

func (t *BinarySearchST[T, K]) Rank(key T) int {
	lo, hi := 0, t.length
	for lo < hi {
		mid := lo + (hi-lo)/2
		value := t.keys[mid]
		if value > key {
			lo = mid + 1
		} else if value < key {
			hi = mid - 1
		} else {
			return mid
		}
	}

	return lo
}

func (t *BinarySearchST[T, K]) Rank2(key T, low, high int) int {
	if high < low {
		return low
	}
	mid := low + (high-low)/2
	val := t.keys[mid]
	if val > key {
		return t.Rank2(key, mid+1, high)
	} else if val < key {
		return t.Rank2(key, low, mid-1)
	}

	return mid
}

func (t *BinarySearchST[T, K]) Get(key T) K {
	for i, x := range t.keys {
		if x == key {
			return t.values[i]
		}
	}

	var result K

	return result
}

func (t *BinarySearchST[T, K]) resize(max int) {
	keys := make([]T, max)
	values := make([]K, max)

	for i := 0; i < t.length; i++ {
		keys[i], values[i] = t.keys[i], t.values[i]
	}

	t.keys = keys
	t.values = values
	t.capacity = max
}
