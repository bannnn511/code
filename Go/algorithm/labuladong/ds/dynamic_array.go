package ds

/*
	Implementation for dynamic array
*/

const capacity = 1

type MyDynamicArray struct {
	array    []int
	size     int
	capacity int
}

func NewDynamicArray() MyDynamicArray {
	array := make([]int, capacity)
	return MyDynamicArray{array: array, size: 0, capacity: capacity}
}

func (l *MyDynamicArray) Get(index int) int {
	if !l.checkElementIndex(index) {
		return -1
	}

	return l.array[index]
}

func (l *MyDynamicArray) AddAtHead(val int) {
	l.AddAtIndex(0, val)
}

func (l *MyDynamicArray) AddAtTail(val int) {
	l.AddAtIndex(l.size, val)
}

func (l *MyDynamicArray) AddAtIndex(index int, val int) {
	if !l.checkPositionIndex(index) {
		return
	}

	if l.size == l.capacity {
		l.resize()
	}

	for i := l.size; i > index; i-- {
		l.array[i] = l.array[i-1]
	}

	l.array[index] = val
	l.size++
}

func (l *MyDynamicArray) DeleteAtIndex(index int) {
	if !l.checkElementIndex(index) {
		return
	}
	for i := index + 1; i < l.size; i++ {
		l.array[i-1] = l.array[i]
	}
	l.array[l.size-1] = 0
	l.size--
}

func (l *MyDynamicArray) resize() {
	l.capacity *= 2
	newArr := make([]int, l.capacity)
	copy(newArr, l.array)

	l.array = newArr
}

// checkElementIndex returns index < size
func (l MyDynamicArray) checkElementIndex(index int) bool {
	return index >= 0 && index < l.size
}

func (l MyDynamicArray) checkPositionIndex(index int) bool {
	return index >= 0 && index <= l.size
}
