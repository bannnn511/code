package algorithm

import (
	"slices"
	"testing"
)

func Test_bitPresentation(t *testing.T) {
	type args struct {
		x int
	}
	tests := []struct {
		name string
		args args
		want [][]int
	}{
		{
			name: "1. subset of 1",
			args: args{x: 1},
			want: [][]int{
				{},
				{0},
			},
		},
		{
			name: "2. subset of 2",
			args: args{x: 2},
			want: [][]int{
				{},
				{0},
				{1},
				{0, 1},
			},
		},
	}
	for _, tt := range tests {
		t.Run(tt.name, func(t *testing.T) {
			got := subsetOf(tt.args.x)
			if !compare2DSlices(got, tt.want) {
				t.Errorf("subsetOf(%v) = %v, want %v", tt.args, got, tt.want)
			}
		})
	}
}

func compare2DSlices(a, b [][]int) bool {
	if len(a) != len(b) {
		return false
	}

	for i := 0; i < len(a); i++ {
		if slices.Compare(a[i], b[i]) != 0 {
			return false
		}
	}

	return true
}
