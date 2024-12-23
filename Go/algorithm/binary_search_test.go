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

func TestSmallestValue(t *testing.T) {
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
				a: []int{1, 2, 3, 5, 5, 5, 7, 8, 9},
				x: 5,
			},
			want: 3,
		},
	}

	for _, tt := range tests {
		t.Run(tt.name, func(t *testing.T) {
			if got := SmallestValue(tt.args.a, tt.args.x); got != tt.want {
				t.Errorf("SmallestValue() = %v, want %v", got, tt.want)
			}
		})
	}
}
