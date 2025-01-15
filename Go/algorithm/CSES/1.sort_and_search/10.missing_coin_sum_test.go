package __sort_and_search

import "testing"

func Test_missingCoinSum(t *testing.T) {
	type args struct {
		coins []int
	}
	tests := []struct {
		name string
		args args
		want int
	}{
		{
			name: "2 1 4 3",
			args: args{
				coins: []int{2, 1, 4, 3},
			},
			want: 11,
		},
		{
			name: "2 2 2 2",
			args: args{coins: []int{2, 2, 2, 2}},
			want: 1,
		},
		{
			name: "1 9 9 1 2 2",
			args: args{
				coins: []int{1, 9, 9, 1, 2, 2},
			},
			want: 7,
		},
	}

	for _, tt := range tests {
		t.Run(tt.name, func(t *testing.T) {
			if got := missingCoinSum(tt.args.coins); got != tt.want {
				t.Errorf("missingCoinSum() = %v, want %v", got, tt.want)
			}
		})
	}
}
