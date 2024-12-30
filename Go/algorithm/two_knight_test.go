package algorithm

import "testing"

func TestTwoKnight_search(t *testing.T) {
	type args struct {
		n int
	}
	tests := []struct {
		name string
		args args
		want int
	}{
		{
			name: "case n=1",
			args: args{n: 1},
			want: 0,
		},
		{
			name: "case n=2",
			args: args{n: 2},
			want: 6,
		},
		{
			name: "case n = 3",
			args: args{n: 3},
			want: 28,
		},
		{
			name: "case n = 4",
			args: args{n: 4},
			want: 96,
		},
	}
	for _, tt := range tests {
		t.Run(tt.name, func(t *testing.T) {
			q := NewTwoKnight(tt.args.n)
			q.search()
			if q.count != tt.want {
				t.Fatalf("got: %d, want: %d", q.count, tt.want)
			}
		})
	}
}
