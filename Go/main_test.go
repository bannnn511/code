package main

import (
	"fmt"
	"reflect"
	"testing"
)

func TestAppend(t *testing.T) {
	type args struct {
		slice []int
		data  []int
	}
	tests := []struct {
		name string
		args args
		want []int
	}{
		{
			name: "1",
			args: args{
				slice: []int{1, 2, 3},
				data:  []int{4, 5},
			},
			want: []int{1, 2, 3, 4, 5},
		},
	}

	for _, tt := range tests {
		t.Run(tt.name, func(t *testing.T) {
			if got := Append(tt.args.slice, tt.args.data...); !reflect.DeepEqual(got, tt.want) {
				t.Errorf("Append() = %v, want %v", got, tt.want)
			}
		})
	}
}

func TestChangeArray(t *testing.T) {
	type args struct {
		slice []int
	}
	tests := []struct {
		name string
		args args
	}{
		{
			name: "1",
			args: args{
				[]int{1, 2},
			},
		},
	}
	for _, tt := range tests {
		t.Run(tt.name, func(t *testing.T) {
			ChangeArray(tt.args.slice)
			fmt.Println("here", tt.args.slice)
		})
	}
}

func Test_concurrentUpdate(t *testing.T) {
	type args struct {
		loops int
	}

	tests := []struct {
		name string
		args args
		want int
	}{
		{
			name: "1e5",
			args: args{
				loops: 1e5,
			},
			want: 100,
		},
		{
			name: "1e8",
			args: args{
				loops: 1e8,
			},
			want: 100,
		},
	}
	for _, tt := range tests {
		t.Run(tt.name, func(t *testing.T) {
			if got := concurrentUpdate(tt.args.loops); got != tt.want {
				t.Errorf("concurrentUpdate() = %v, want %v", got, tt.want)
			}
		})
	}
}
