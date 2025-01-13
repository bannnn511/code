package __dynamic_programming

import "testing"

func Test_minimizingCoins(t *testing.T) {
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
			name: "case 1",
			args: args{
				coins: []int{1, 5, 7},
				x:     1,
			},
			want: 1,
		},
		{
			name: "case 2",
			args: args{
				coins: []int{1, 5, 7},
				x:     5,
			},
			want: 1,
		},
		{
			name: "case 3",
			args: args{
				coins: []int{1, 5, 7},
				x:     6,
			},
			want: 2,
		},
		{
			name: "case 4",
			args: args{
				coins: []int{1, 5, 7},
				x:     8,
			},
			want: 2,
		},
		{
			name: "case 5",
			args: args{
				coins: []int{1, 5, 7},
				x:     13,
			},
			want: 3,
		},
		{
			name: "case 6",
			args: args{
				coins: []int{1, 5, 7},
				x:     11,
			},
			want: 3,
		},
	}

	for _, tt := range tests {
		t.Run(tt.name, func(t *testing.T) {
			if got := minimizingCoins(tt.args.coins, tt.args.x); got != tt.want {
				t.Errorf("minimizingCoins() = %v, want %v", got, tt.want)
			}
		})
	}
}
