package CSES

import (
	"testing"
)

func Test_findMissingNumber(t *testing.T) {
	type args struct {
		x int
		a []int
	}
	tests := []struct {
		name string
		args args
		want int
	}{
		{
			name: "5 - 2 3 1 5",
			args: args{
				x: 5,
				a: []int{2, 3, 1, 5},
			},
			want: 4,
		},
		{
			name: "3 - 2 3",
			args: args{
				x: 3,
				a: []int{2, 3},
			},
			want: 1,
		},
	}
	for _, tt := range tests {
		t.Run(tt.name, func(t *testing.T) {
			if got := findMissingNumber(tt.args.x, tt.args.a); got != tt.want {
				t.Errorf("findMissingNumber() = %v, want %v", got, tt.want)
			}
		})
	}
}
