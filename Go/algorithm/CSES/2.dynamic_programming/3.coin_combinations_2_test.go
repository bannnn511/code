package __dynamic_programming

import "testing"

func Test_coinCombination2(t *testing.T) {
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
				coins: []int{2, 3, 5},
				x:     9,
			},
			want: 3,
		},
		{
			name: "case 2",
			args: args{
				coins: []int{2, 3, 5},
				x:     6,
			},
			want: 2,
		},
	}
	for _, tt := range tests {
		t.Run(tt.name, func(t *testing.T) {
			if got := coinCombination2(tt.args.coins, tt.args.x); got != tt.want {
				t.Errorf("coinCombination2() = %v, want %v", got, tt.want)
			}
		})
	}
}
