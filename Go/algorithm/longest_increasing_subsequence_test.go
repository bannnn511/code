package algorithm

import "testing"

func Test_longestIncreasingSubsequence(t *testing.T) {
	type args struct {
		array []int
		n     int
	}
	tests := []struct {
		name string
		args args
		want int
	}{
		{
			name: "case 1",
			args: args{
				array: []int{6, 2, 5, 1, 7, 4, 8, 3},
				n:     0,
			},
			want: 1,
		},
		{
			name: "case 2",
			args: args{
				array: []int{6, 2, 5, 1, 7, 4, 8, 3},
				n:     1,
			},
			want: 1,
		},
		{
			name: "case 3",
			args: args{
				array: []int{6, 2, 5, 1, 7, 4, 8, 3},
				n:     2,
			},
			want: 2,
		},
		{
			name: "case 4",
			args: args{
				array: []int{6, 2, 5, 1, 7, 4, 8, 3},
				n:     3,
			},
			want: 1,
		},
		{
			name: "case 5",
			args: args{
				array: []int{6, 2, 5, 1, 7, 4, 8, 3},
				n:     4,
			},
			want: 3,
		},
	}
	for _, tt := range tests {
		t.Run(tt.name, func(t *testing.T) {
			if got := longestIncreasingSubsequence(tt.args.array, tt.args.n); got != tt.want {
				t.Errorf("longestIncreasingSubsequence() = %v, want %v", got, tt.want)
			}
		})
	}
}

func Test_longestIncreasingSubsequence2(t *testing.T) {
	type args struct {
		array []int
		n     int
	}
	tests := []struct {
		name string
		args args
		want int
	}{
		{
			name: "case 1",
			args: args{
				array: []int{6, 2, 5, 1, 7, 4, 8, 3},
				n:     0,
			},
			want: 1,
		},
		{
			name: "case 2",
			args: args{
				array: []int{6, 2, 5, 1, 7, 4, 8, 3},
				n:     1,
			},
			want: 1,
		},
		{
			name: "case 3",
			args: args{
				array: []int{6, 2, 5, 1, 7, 4, 8, 3},
				n:     2,
			},
			want: 2,
		},
		{
			name: "case 4",
			args: args{
				array: []int{6, 2, 5, 1, 7, 4, 8, 3},
				n:     3,
			},
			want: 1,
		},
		{
			name: "case 5",
			args: args{
				array: []int{6, 2, 5, 1, 7, 4, 8, 3},
				n:     4,
			},
			want: 3,
		},
	}
	for _, tt := range tests {
		t.Run(tt.name, func(t *testing.T) {
			if got := longestIncreasingSubsequence2(tt.args.array, tt.args.n); got != tt.want {
				t.Errorf("longestIncreasingSubsequence() = %v, want %v", got, tt.want)
			}
		})
	}
}
