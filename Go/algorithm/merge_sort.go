package algorithm

import "cmp"

func merge[T cmp.Ordered](a []T, lo, mid, hi int) {
	i := lo
	j := mid + 1

	aux := make([]T, len(a))
	for idx := lo; idx <= hi; idx++ { // Copy a[lo...hi] to aux[lo...hi]
		aux[idx] = a[idx]
	}

	for k := lo; k <= hi; k++ { // Merge back to a[lo...hi]
		if i > mid {
			a[k] = a[j]
			j++
		} else if j > hi {
			a[k] = a[i]
			i++
		} else if a[i] > a[j] {
			a[k] = a[j]
			j++
		} else {
			a[k] = a[i]
			i++
		}
	}
}

func sort[T cmp.Ordered](a []T, lo, hi int) {
	if lo >= hi {
		return
	}
	mid := lo + (hi-lo)/2
	sort(a, lo, mid)
	sort(a, mid+1, hi)
	merge(a, lo, mid, hi)
}

func MergeSort[T cmp.Ordered](a []T) {
	sort(a, 0, len(a)-1)
}

func IsSorted[T cmp.Ordered](x []T) bool {
	for i := len(x) - 1; i > 0; i-- {
		if x[i] < x[i-1] {
			return false
		}
	}
	return true
}

func MergeSortBU[T cmp.Ordered](a []T) {
	N := len(a)
	for sz := 1; sz < N; sz = sz + sz {
		for lo := 0; lo < N-sz; lo += sz + sz {
			merge(a, lo, lo+sz-1, min(lo+sz+sz-1, N-1))
		}
	}
}
