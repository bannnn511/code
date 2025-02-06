package __Graph

import (
	"reflect"
	"testing"
)

func Test_buildingRoads(t *testing.T) {
	type args struct {
		n     int
		m     int
		roads []road
	}
	tests := []struct {
		name string
		args args
		want []int
	}{
		{
			name: "case1",
			args: args{
				n: 3,
				m: 2,
				roads: []road{
					{0, 1},
					{2, 3},
				},
			},
			want: []int{1, 3},
		},
	}
	for _, tt := range tests {
		t.Run(tt.name, func(t *testing.T) {
			if got := buildingRoads(tt.args.n, tt.args.m, tt.args.roads); !reflect.DeepEqual(got, tt.want) {
				t.Errorf("buildingRoads() = %v, want %v", got, tt.want)
			}
		})
	}
}
