package algorithm

type QueenProblem struct {
	n      int
	count  int
	column []int
	diag1  []int
	diag2  []int
}

func NewQueenProblem(n int) *QueenProblem {
	return &QueenProblem{
		n:      n,
		count:  0,
		column: make([]int, n),
		diag1:  make([]int, n*(n-1)),
		diag2:  make([]int, n*(n-1)),
	}
}

func (q *QueenProblem) search(y int) {
	if y == q.n {
		q.count++
	}

	for x := 0; x < q.n; x++ {
		diag1Index := x + y
		diag2Index := x - y + q.n - 1
		if q.column[x] == 1 ||
			q.diag1[diag1Index] == 1 ||
			q.diag2[diag2Index] == 1 {
			continue
		}

		q.column[x], q.diag1[diag1Index], q.diag2[diag2Index] = 1, 1, 1
		q.search(y + 1)
		q.column[x], q.diag1[diag1Index], q.diag2[diag2Index] = 0, 0, 0

	}
}
