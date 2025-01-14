package __dynamic_programming

import "testing"

func Test_coinCombination(t *testing.T) {
	type args struct {
		coins []int
		x     int
	}
	tests := []struct {
		name string
		args args
		want int
	}{
		{
			name: "{1,2} sum = 3",
			args: args{
				coins: []int{1, 2},
				x:     3,
			},
			want: 3,
		},
		{
			name: "{1,2,3} sum = 4",
			args: args{
				coins: []int{1, 2, 3},
				x:     4,
			},
			want: 7,
		},
		{
			name: "{1,2,3,4} sum = 4",
			args: args{
				coins: []int{1, 2, 3, 4},
				x:     4,
			},
			want: 8,
		},
		{
			name: "{1} sum = 1000000",
			args: args{
				coins: []int{1},
				x:     1000000,
			},
			want: 1,
		},
	}

	for _, tt := range tests {
		t.Run(tt.name, func(t *testing.T) {
			if got := coinCombination(tt.args.coins, tt.args.x); got != tt.want {
				t.Errorf("coinCombination() = %v, want %v", got, tt.want)
			}
		})
	}
}
