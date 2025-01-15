package __dynamic_programming

import "testing"

func Test_removingDigits(t *testing.T) {
	type args struct {
		n int
	}
	tests := []struct {
		name string
		args args
		want int
	}{
		{
			name: "1",
			args: args{
				1,
			},
			want: 1,
		},
		{
			name: "15",
			args: args{
				15,
			},
			want: 3,
		},
		{
			name: "21",
			args: args{
				n: 21,
			},
			want: 4,
		},
		{
			name: "27",
			args: args{
				n: 27,
			},
			want: 5,
		},
		{
			name: "30",
			args: args{
				n: 30,
			},
			want: 6,
		},
		{
			name: "34",
			args: args{
				34,
			},
			want: 7,
		},
		{
			name: "43",
			args: args{
				n: 43,
			},
			want: 8,
		},
		{
			name: "81",
			args: args{
				n: 81,
			},
			want: 14,
		},
	}
	for _, tt := range tests {
		t.Run(tt.name, func(t *testing.T) {
			if got := removingDigits(tt.args.n); got != tt.want {
				t.Errorf("removingDigits() = %v, want %v", got, tt.want)
			}
		})
	}
}
