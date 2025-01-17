package __sort_and_search

func longestSequence(playlist []int) int {
	mp := map[int]int{}

	start := 0
	longest := 1
	mp[playlist[0]] = 0
	for i := 1; i < len(playlist); i++ {
		idx, ok := mp[playlist[i]]
		if ok {
			start = max(start, idx+1)
		}
		mp[playlist[i]] = i
		longest = max(longest, i-start+1)
	}

	return longest
}
