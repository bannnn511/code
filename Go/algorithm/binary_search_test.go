package algorithm

import (
	"testing"
)

const N = 100_000_0

func makeSortedInts(n int) []int {
	ints := make([]int, n)
	for i := 0; i < n; i++ {
		ints[i] = i
	}
	return ints
}

func TestSearchInts(t *testing.T) {
	type args struct {
		a []int
		x int
	}
	tests := []struct {
		name string
		args args
		want int
	}{
		{
			name: "Test case 1",
			args: args{
				a: []int{1, 2, 3, 4, 5, 6, 7, 8, 9},
				x: 5,
			},
			want: 4,
		},
		{
			name: "Test case 2",
			args: args{
				a: []int{1, 19, 23, 43, 52, 61, 73, 88, 99},
				x: 41,
			},
			want: -1,
		},
		{
			name: "Test case 3",
			args: args{
				a: []int{1, 2, 3, 4, 5, 6, 7, 8, 9},
				x: 4,
			},
			want: 3,
		},
		{
			name: "Test case 5",
			args: args{
				a: []int{1, 2, 3, 4, 5, 6, 7, 8, 9},
				x: 1,
			},
			want: 0,
		},
		{
			name: "Test case 6",
			args: args{
				a: []int{1, 2, 3, 4, 5, 6, 7, 8, 9},
				x: 9,
			},
			want: 8,
		},
		{
			name: "Test case 7",
			args: args{
				a: []int{1, 2, 3, 5, 5, 5, 7, 8, 9},
				x: 5,
			},
			want: 3,
		},
	}

	for _, tt := range tests {
		t.Run(tt.name, func(t *testing.T) {
			if got := SearchInts(tt.args.a, tt.args.x); got != tt.want {
				t.Errorf("SearchInts() = %v, want %v", got, tt.want)
			}
		})
	}
}

func TestSearchInts2(t *testing.T) {
	type args struct {
		a []int
		x int
	}
	tests := []struct {
		name string
		args args
		want int
	}{
		{
			name: "Test case 1",
			args: args{
				a: []int{1, 2, 3, 4, 5, 6, 7, 8, 9},
				x: 5,
			},
			want: 4,
		},
		{
			name: "Test case 2",
			args: args{
				a: []int{1, 19, 23, 43, 52, 61, 73, 88, 99},
				x: 41,
			},
			want: -1,
		},
		{
			name: "Test case 3",
			args: args{
				a: []int{1, 2, 3, 4, 5, 6, 7, 8, 9},
				x: 4,
			},
			want: 3,
		},
		{
			name: "Test case 5",
			args: args{
				a: []int{1, 2, 3, 4, 5, 6, 7, 8, 9},
				x: 1,
			},
			want: 0,
		},
		{
			name: "Test case 6",
			args: args{
				a: []int{1, 2, 3, 4, 5, 6, 7, 8, 9},
				x: 9,
			},
			want: 8,
		},
	}

	for _, tt := range tests {
		t.Run(tt.name, func(t *testing.T) {
			if got := SearchInts2(tt.args.a, tt.args.x); got != tt.want {
				t.Errorf("SearchInts2() = %v, want %v", got, tt.want)
			}
		})
	}
}

func BenchmarkSearchInts(b *testing.B) {
	for i := 0; i < b.N; i++ {
		b.StopTimer()
		ints := makeSortedInts(N)
		b.StartTimer()
		SearchInts(ints, 100)
	}
}

func BenchmarkSearchInts2(b *testing.B) {
	for i := 0; i < b.N; i++ {
		b.StopTimer()
		ints := makeSortedInts(N)
		b.StartTimer()
		SearchInts2(ints, 100)
	}
}

func TestSmallestSolution(t *testing.T) {
	type args struct {
		a []int
		x int
	}
	tests := []struct {
		name string
		args args
		want int
	}{
		{
			name: "1. Find k where arr[k] > 5",
			args: args{
				a: []int{1, 2, 3, 5, 5, 5, 7, 8, 9},
				x: 5,
			},
			want: 6,
		},
		{
			name: "1. Find k where arr[k] > 3",
			args: args{
				a: []int{1, 2, 3, 5, 5, 5, 7, 8, 9},
				x: 3,
			},
			want: 3,
		},
		{
			name: "1. Find k where arr[k] > 8",
			args: args{
				a: []int{1, 2, 3, 5, 5, 5, 7, 8, 9},
				x: 8,
			},
			want: 8,
		},
	}

	for _, tt := range tests {
		t.Run(tt.name, func(t *testing.T) {
			if got := FindSmallestSolution(tt.args.a, tt.args.x); got != tt.want {
				t.Errorf("FindSmallestSolution() = %v, want %v", got, tt.want)
			}
		})
	}
}

func TestMaximumValue(t *testing.T) {
	type args struct {
		a []int
	}
	tests := []struct {
		name string
		args args
		want int
	}{
		{
			name: "1. Find 60",
			args: args{
				a: []int{1, 10, 20, 30, 40, 50, 60, 55, 45, 35, 25, 15, 5},
			},
			want: 6,
		},
		{
			name: "2. Find 55",
			args: args{
				a: []int{1, 10, 20, 30, 40, 50, 53, 55, 45, 35, 25, 15, 5},
			},
			want: 7,
		},
		{
			name: "3. Find 10",
			args: args{
				a: []int{1, 10, 8, 7, 6},
			},
			want: 1,
		},
		{
			name: "4. Find 10",
			args: args{
				a: []int{10, 8, 7, 6},
			},
			want: 0,
		},
		{
			name: "5. Find 20",
			args: args{
				a: []int{0, 1, 12, 15, 20},
			},
			want: 4,
		},
	}

	for _, tt := range tests {
		t.Run(tt.name, func(t *testing.T) {
			if got := FindMaximumValue(tt.args.a); got != tt.want {
				t.Errorf("FindMaximumValue() = %v, want %v", got, tt.want)
			}
		})
	}
}
