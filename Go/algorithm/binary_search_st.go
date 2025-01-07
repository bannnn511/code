package algorithm

import "cmp"

type BinarySearchST[T cmp.Ordered, K cmp.Ordered] struct {
	keys   []T
	values []K
	n      int
}

func NewBinarySearchST[T, K cmp.Ordered](n int) *BinarySearchST[T, K] {
	keys := make([]T, n)
	values := make([]K, n)
	return &BinarySearchST[T, K]{
		keys:   keys,
		values: values,
		n:      0,
	}
}

func (t *BinarySearchST[T, K]) Size() int {
	return t.n
}

func (t *BinarySearchST[T, K]) Put(key T, value K) {
	i := t.Rank(key)
	if i < t.n && t.keys[i] == key {
		t.values[i] = value
		return
	}

	for idx := t.n; idx > i; idx-- {
		t.keys[idx], t.values[idx] = t.keys[idx-1], t.values[idx-1]
	}
	t.keys[i] = key
	t.values[i] = value
	t.n++
}

func (t *BinarySearchST[T, K]) Rank(key T) int {
	lo, hi := 0, t.n
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
	rank := t.Rank(key)
}
