package __dynamic_programming

import "testing"

func Test_diceCombination(t *testing.T) {
	type args struct {
		n int
	}
	tests := []struct {
		name string
		args args
		want int
	}{
		{
			name: "dice 1",
			args: args{n: 1},
			want: 1,
		},
		{
			name: "dice 2",
			args: args{n: 2},
			want: 2,
		},
		{
			name: "dice 3",
			args: args{n: 3},
			want: 4,
		},
		{
			name: "dice 4",
			args: args{n: 4},
			want: 8,
		},
		{
			name: "dice 5",
			args: args{n: 5},
			want: 16,
		},
	}
	for _, tt := range tests {
		t.Run(tt.name, func(t *testing.T) {
			if got := diceCombination(tt.args.n); got != tt.want {
				t.Errorf("diceCombination() = %v, want %v", got, tt.want)
			}
		})
	}
}
