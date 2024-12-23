package algorithm

// SearchInts implements binary search using middle point
func SearchInts(a []int, x int) int {
	i, j := 0, len(a)-1
	for i <= j {
		k := (i + j) / 2
		if a[k] == x {
			return k
		}

		if a[k] > x {
			j = k - 1
		} else {
			i = k + 1
		}
	}

	return -1
}

// SearchInts2 implements binary search using increment jump then slow down when close to target
func SearchInts2(a []int, x int) int {
	k := 0
	n := len(a)
	for b := n / 2; b >= 1; b /= 2 {
		for k+b < n && a[k+b] <= x {
			k += b
		}
	}
	if a[k] == x {
		return k
	}
	return -1
}

// FindSmallestSolution finds the position where the value of a function change
// x    	 |0		|1		| ...		|k-1		|k		| k+1		|...
// ok(x)	|false	|false	| ...		|false		| true	| true		|...

func FindSmallestSolution(a []int, x int) int {
	return find(a, func(i int) bool {
		return a[i] <= x
	})
}

// FindMaximumValue finds the maximum value for a function that is
// first increasing and then decreasing
func FindMaximumValue(a []int) int {
	length := len(a)
	return find(a, func(i int) bool {
		if i+1 == length {
			return false
		}
		return a[i] < a[i+1]
	})
}

func find(a []int, f func(int) bool) int {
	k := -1
	n := len(a)
	for b := n; b >= 1; b /= 2 {
		for k+b < n && f(k+b) {
			k += b
		}
	}

	return k + 1
}
