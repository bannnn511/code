package __dynamic_programming

func removingDigits(n int) int {
	dp := make([]int, n+1)
	for i := range dp {
		dp[i] = n
	}
	dp[0] = 0

	for i := 0; i <= n; i++ {
		d := i
		for d > 0 {
			if d%10 != 0 {
				dp[i] = min(dp[i], dp[i-d%10]+1)
			}
			d /= 10
		}
	}

	return dp[n]
}
