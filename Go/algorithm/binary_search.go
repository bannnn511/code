package algorithm

// 1 2 3 4 5 6 7 8 9
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
