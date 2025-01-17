package __dynamic_programming

import "testing"

func Test_gridPath(t *testing.T) {
	type args struct {
		squares [][]int
	}
	tests := []struct {
		name string
		args args
		want int
	}{
		{
			name: "case 1",
			args: args{
				squares: [][]int{
					{0, 0, 0},
					{0, 0, 0},
					{0, 0, 0},
				},
			},
			want: 2,
		},
		{
			name: "case 2",
			args: args{
				squares: [][]int{
					{0, 0, 0},
					{0, 0, -1},
					{0, 0, 0},
				},
			},
			want: 1,
		},
		{
			name: "case 3",
			args: args{
				squares: [][]int{
					{0, 0, 0},
					{0, 0, -1},
					{0, -1, 0},
				},
			},
			want: 0,
		},
		{
			name: "case 4",
			args: args{
				squares: [][]int{
					{0, 0, 0, 0, 0},
					{0, 0, 0, 0, 0},
					{0, 0, -1, 0, 0},
					{0, 0, 0, 0, -1},
					{0, -1, 0, 0, 0},
				}},
			want: 3,
		},
		{
			name: "case 5",
			args: args{
				squares: [][]int{
					{0, 0},
					{0, -1},
				}},
			want: 0,
		},
		{
			name: "case 6",
			args: args{
				squares: [][]int{
					{0, 0, 0},
					{0, -1, 0},
					{0, 0, 0},
				}},
			want: 0,
		},
	}
	for _, tt := range tests {
		t.Run(tt.name, func(t *testing.T) {
			if got := gridPath(tt.args.squares); got != tt.want {
				t.Errorf("gridPath() = %v, want %v", got, tt.want)
			}
		})
	}
}
