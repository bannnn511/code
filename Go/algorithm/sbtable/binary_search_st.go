package sbtable

type Ordered interface {
	~int | ~int8 | ~int16 | ~int32 | ~int64 |
	~uint | ~uint8 | ~uint16 | ~uint32 | ~uint64 | ~uintptr |
	~float32 | ~float64 |
	~string
}

type BinarySearchST[K Ordered, V Ordered] struct {
	keys     []K
	values   []V
	length   int
	capacity int
}

var _ SymbolTale[int, int] = &BinarySearchST[int, int]{}

func NewBST[K, V Ordered]() *BinarySearchST[K, V] {
	keys := make([]K, 2)
	values := make([]V, 2)
	return &BinarySearchST[K, V]{
		keys:     keys,
		values:   values,
		length:   0,
		capacity: 2,
	}
}

func (t *BinarySearchST[K, V]) Size() int {
	return t.length
}

func (t *BinarySearchST[K, V]) Put(key K, value V) {
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

func (t *BinarySearchST[K, V]) Rank(key K) int {
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

func (t *BinarySearchST[K, V]) Rank2(key K, low, high int) int {
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

func (t *BinarySearchST[K, V]) Get(key K) (V, bool) {
	var result V
	if t.IsEmpty() {
		return result, false
	}

	i := t.Rank(key)
	if i < t.length && t.keys[i] == key {
		return t.values[i], true
	}

	return result, false
}

func (t *BinarySearchST[K, V]) resize(max int) {
	keys := make([]K, max)
	values := make([]V, max)

	for i := 0; i < t.length; i++ {
		keys[i], values[i] = t.keys[i], t.values[i]
	}

	t.keys = keys
	t.values = values
	t.capacity = max
}

func (t *BinarySearchST[K, V]) Delete(key K) {
	idx := t.Rank(key)
	if idx == t.length || t.keys[idx] != key {
		return
	}
	for i := idx; i < t.length; i++ {
		t.keys[i], t.values[i] = t.keys[i+1], t.values[i+1]
	}
	t.length--
}

func (t *BinarySearchST[K, V]) Contains(key K) bool {
	_, ok := t.Get(key)
	return ok
}

func (t *BinarySearchST[K, V]) IsEmpty() bool {
	return t.length == 0
}
