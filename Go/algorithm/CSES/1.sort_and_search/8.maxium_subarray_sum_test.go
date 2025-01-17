package __sort_and_search

import "testing"

func Test_maximumSubArraySum(t *testing.T) {
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
			name: "case 1",
			args: args{
				a: []int{-1, 3, -2, 5, 3, -5, 2, 2},
			},
			want: 9,
		},
		{
			name: "case 2",
			args: args{
				a: []int{-1000000000},
			},
			want: -1000000000,
		},
		{
			name: "case 3",
			args: args{
				a: []int{-1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
			},
			want: -1,
		},
	}
	for _, tt := range tests {
		t.Run(tt.name, func(t *testing.T) {
			if got := maximumSubArraySum(tt.args.a, tt.args.x); got != tt.want {
				t.Errorf("maximumSubArraySum() = %v, want %v", got, tt.want)
			}
		})
	}
}
