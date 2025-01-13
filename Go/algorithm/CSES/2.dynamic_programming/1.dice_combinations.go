package __dynamic_programming

var dices = [...]int{1, 2, 3, 4, 5, 6}

func diceCombination(n int) int {
	dp := make([]int, n+1)
	dp[0] = 1
	for i := 1; i <= n; i++ {
		for j := 1; j < len(dices) && i-j >= 0; j++ {
			dp[i] = dp[i] + dp[i-j]
		}
	}

	return dp[n]
}
