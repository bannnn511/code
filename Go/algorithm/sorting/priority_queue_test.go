package algorithm

import (
	"sort"
	"testing"
)

func TestHeapInsert(t *testing.T) {
	heap := NewMaxPQ(10)
	values := []int{3, 1, 4, 1, 5, 9, 2, 6}

	for _, v := range values {
		heap.Insert(v)
	}

	// Check if heap property is maintained
	for i := 1; i <= len(heap.pq)/2; i++ {
		if 2*i < len(heap.pq) && heap.pq[i] < heap.pq[2*i] {
			t.Errorf("Heap property violated at index %d: parent %d less than left child %d",
				i, heap.pq[i], heap.pq[2*i])
		}
		if 2*i+1 < len(heap.pq) && heap.pq[i] < heap.pq[2*i+1] {
			t.Errorf("Heap property violated at index %d: parent %d less than right child %d",
				i, heap.pq[i], heap.pq[2*i+1])
		}
	}
}

func TestHeapInsert2(t *testing.T) {
	heap := NewMaxPQ(10)
	values := []int{4, 2, 6, 1, 3, 8, 7, 9}
	for i, v := range values {
		heap.Insert(v)
		if i == 0 && heap.pq[1] != 4 {
			t.Errorf("Expected 4, got %d", heap.pq[1])
		}
		if i == 2 && heap.pq[1] != 6 {
			t.Errorf("Expected 6, got %d", heap.pq[1])
		}
		if i == 5 && heap.pq[1] != 8 {
			t.Errorf("Expected 8, got %d", heap.pq[1])
		}
	}
}

func TestHeapRemoveMax(t *testing.T) {
	heap := NewMaxPQ(10)
	values := []int{3, 1, 4, 1, 5, 9, 2, 6}

	for _, v := range values {
		heap.Insert(v)
	}

	// Create a sorted copy to compare against
	sortedValues := make([]int, len(values))
	copy(sortedValues, values)
	sort.Sort(sort.Reverse(sort.IntSlice(sortedValues)))

	// Remove all elements and verify they come out in descending order
	for i := range values {
		max := heap.RemoveMax()
		if max != sortedValues[i] {
			t.Errorf("RemoveMax returned %d, expected %d", max, sortedValues[i])
		}
	}
}

func TestHeapWithDuplicates(t *testing.T) {
	heap := NewMaxPQ(10)
	values := []int{5, 5, 5, 3, 3, 7, 7}

	for _, v := range values {
		heap.Insert(v)
	}

	// Create a sorted copy to compare against
	sortedValues := make([]int, len(values))
	copy(sortedValues, values)
	sort.Sort(sort.Reverse(sort.IntSlice(sortedValues)))

	// Remove all elements and verify they come out in descending order
	for i := range values {
		max := heap.RemoveMax()
		if max != sortedValues[i] {
			t.Errorf("RemoveMax returned %d, expected %d", max, sortedValues[i])
		}
	}
}

func TestHeapEdgeCases(t *testing.T) {
	// Test with a single element
	heap := NewMaxPQ(1)
	heap.Insert(42)
	if max := heap.RemoveMax(); max != 42 {
		t.Errorf("RemoveMax returned %d, expected 42", max)
	}

	// Test with large values
	heap = NewMaxPQ(3)
	heap.Insert(1000000)
	heap.Insert(2000000)
	heap.Insert(3000000)

	if max := heap.RemoveMax(); max != 3000000 {
		t.Errorf("RemoveMax returned %d, expected 3000000", max)
	}
	if max := heap.RemoveMax(); max != 2000000 {
		t.Errorf("RemoveMax returned %d, expected 2000000", max)
	}
	if max := heap.RemoveMax(); max != 1000000 {
		t.Errorf("RemoveMax returned %d, expected 1000000", max)
	}
}

func TestHeapWithNegativeValues(t *testing.T) {
	heap := NewMaxPQ(5)
	values := []int{-5, -10, -3, -7, -1}

	for _, v := range values {
		heap.Insert(v)
	}

	// Create a sorted copy to compare against
	sortedValues := make([]int, len(values))
	copy(sortedValues, values)
	sort.Sort(sort.Reverse(sort.IntSlice(sortedValues)))

	// Remove all elements and verify they come out in descending order
	for i := 0; i < len(values); i++ {
		max := heap.RemoveMax()
		if max != sortedValues[i] {
			t.Errorf("RemoveMax returned %d, expected %d", max, sortedValues[i])
		}
	}
}

func BenchmarkHeapOperations(b *testing.B) {
	b.Run("Insert", func(b *testing.B) {
		heap := NewMaxPQ(b.N)
		b.ResetTimer()
		for i := 0; i < b.N; i++ {
			heap.Insert(i)
		}
	})

	b.Run("RemoveMax", func(b *testing.B) {
		heap := NewMaxPQ(b.N)
		for i := 0; i < b.N; i++ {
			heap.Insert(i)
		}

		b.ResetTimer()
		for i := 0; i < b.N; i++ {
			heap.RemoveMax()
		}
	})
}
