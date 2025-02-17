package stuff

import (
	"fmt"
	"testing"
)

func Test_RoundRobin(t *testing.T) {
	n := 3
	m := 5
	arrival := []int{2, 4, 1, 8, 9}
	burst := []int{7, 9, 2, 4, 5}

	servers := make([]Server, n)
	for i := range servers {
		servers[i] = Server{Index: i}
	}

	requests := make([]Request, m)
	for i := range requests {
		requests[i] = Request{Index: i, ArriveTime: arrival[i], BurstTime: burst[i]}
	}

	afterProcessing := RoundRobin(servers, requests)

	rs := make([]int, m)
	for i := range afterProcessing {
		rs[i] = afterProcessing[i].ServerAssigned + 1
	}

	fmt.Println(rs)
}
