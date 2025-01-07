package sbtable

type SymbolTale[K, V Ordered] interface {
	Put(key K, value V)
	Get(key K) (V, bool)
	Delete(key K)
	Contains(key K) bool
	IsEmpty() bool
	Size() int
}
