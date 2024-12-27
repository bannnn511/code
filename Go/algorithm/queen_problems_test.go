package algorithm

import "testing"

func TestQueenProblem_search(t *testing.T) {
	type args struct {
		y int
	}
	tests := []struct {
		name string
		args args
		want int
	}{
		{
			name: "n = 3",
			args: args{y: 4},
			want: 2,
		},
		{
			name: "n =5",
			args: args{y: 5},
			want: 10,
		},
	}
	for _, tt := range tests {
		t.Run(tt.name, func(t *testing.T) {
			q := NewQueenProblem(tt.args.y)
			q.search(0)
			if q.count != tt.want {
				t.Errorf("got %d, want %d", q.count, tt.want)
			}
		})
	}
}
