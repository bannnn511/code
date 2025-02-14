package __sort_and_search

func find(m map[int]int, x int) (int, bool) {
	for k, _ := range m {
		if k == x {
			return k, true
		}
	}

	return 0, false
}

func towers(cubes []int) int {
	count := 0
	towers := map[int]int{}

	for _, c := range cubes {
	}

	return count
}
