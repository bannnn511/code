package algorithm

import "cmp"

func partition[T cmp.Ordered](a []T, lo, hi int) int {
	i := lo + 1
	j := hi
	v := a[lo]
	for {
		for ; a[i] < v; i++ {
			if i == hi {
				break
			}
		}
		for ; a[j] > v; j-- {
			if j == lo {
				break
			}
		}
		if i >= j {
			break
		}
		a[i], a[j] = a[j], a[i]
	}
	a[lo], a[j] = a[j], a[lo]

	return j
}

func qSort[T cmp.Ordered](a []T, lo, hi int) {
	if hi <= lo {
		return
	}
	j := partition(a, lo, hi)
	qSort(a, lo, j-1)
	qSort(a, j+1, hi)
}

func QuickSort[T cmp.Ordered](a []T) {
	qSort(a, 0, len(a)-1)
}
