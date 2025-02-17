package stuff

import (
	"sort"
)

type Request struct {
	Index          int
	ArriveTime     int
	BurstTime      int
	ServerAssigned int
}

type Server struct {
	Index          int
	ReadyAfterTime int // unit: second

}

func RoundRobin(servers []Server, requests []Request) []Request {
	sort.Slice(requests, func(i, j int) bool {
		return requests[i].Index < requests[j].Index
	})

	sort.Slice(requests, func(i, j int) bool {
		return requests[i].ArriveTime < requests[j].ArriveTime
	})

	for i := range requests {
		server, ok := getServerReady(servers, requests[i].ArriveTime, requests[i].BurstTime)
		if !ok {
			requests[i].ServerAssigned = -1
		} else {
			requests[i].ServerAssigned = server.Index
		}

	}

	sort.Slice(requests, func(i, j int) bool {
		return requests[i].Index < requests[j].Index
	})

	return requests
}

func getServerReady(servers []Server, arriveTime int, burstTime int) (Server, bool) {

	for i := range servers {
		// Check if the server is ready to process the request
		if servers[i].ReadyAfterTime <= arriveTime {
			servers[i].ReadyAfterTime = burstTime + arriveTime
			return servers[i], true
		}
	}

	return Server{}, false
}
