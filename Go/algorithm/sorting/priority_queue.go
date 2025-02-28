package algorithm

/*
node at k: k/2
two children in node: 2k and 2k+1
*/

type MaxPQ struct {
	pq       []int
	n        int
	capacity int
}

func NewMaxPQ(maxN int) *MaxPQ {
	pq := make([]int, maxN+1)
	return &MaxPQ{
		pq:       pq,
		n:        0,
		capacity: maxN,
	}
}

func (h *MaxPQ) Insert(key int) {
	h.n++
	if h.n == h.capacity {
		h.capacity *= 2
		h.pq = append(h.pq, make([]int, h.capacity)...)
	}
	h.pq[h.n] = key
	h.swim(h.n)
}

func (h *MaxPQ) RemoveMax() int {
	max := h.pq[1]
	h.swap(1, h.n)
	h.n--
	h.pq[h.n+1] = 0
	h.sink(1)

	return max
}

func (h *MaxPQ) less(i, j int) bool {
	return h.pq[i] < h.pq[j]
}

func (h *MaxPQ) swap(i, j int) {
	h.pq[i], h.pq[j] = h.pq[j], h.pq[i]
}

func (h *MaxPQ) swim(k int) {
	for k > 1 && h.less(k/2, k) {
		h.swap(k/2, k)
		k = k / 2
	}
}

func (h *MaxPQ) sink(k int) {
	for 2*k <= h.n {
		j := 2 * k
		if j < h.n && h.less(j, j+1) {
			j++
		}
		if !h.less(k, j) {
			break
		}
		h.swap(k, j)
		k = j
	}
}
