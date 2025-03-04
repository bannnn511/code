package main

import (
	"crypto/tls"
	"crypto/x509"
	"fmt"
	"log"
	"net/http"
	"os"
)

// CORS middleware to allow cross-origin requests
func corsMiddleware(next http.Handler) http.Handler {
	return http.HandlerFunc(func(w http.ResponseWriter, r *http.Request) {
		// Set CORS headers
		w.Header().Set("Access-Control-Allow-Origin", "*") // Allow any origin
		w.Header().Set("Access-Control-Allow-Methods", "GET, POST, PUT, DELETE, OPTIONS")
		w.Header().Set("Access-Control-Allow-Headers", "Content-Type, Authorization")

		// Handle preflight requests
		if r.Method == "OPTIONS" {
			w.WriteHeader(http.StatusOK)
			return
		}

		// Process the request
		next.ServeHTTP(w, r)
	})
}

func main() {
	// Load CA certificate (for client verification)
	caCert, err := os.ReadFile("./certs/ca.crt")
	if err != nil {
		log.Fatalf("Error loading CA certificate: %v", err)
	}

	// Create a CA certificate pool and add our CA certificate
	caCertPool := x509.NewCertPool()
	caCertPool.AppendCertsFromPEM(caCert)

	// Load server certificate and key
	serverCert, err := tls.LoadX509KeyPair("./certs/server.crt", "./certs/server.key")
	if err != nil {
		log.Fatalf("Error loading server certificate: %v", err)
	}

	// Configure TLS with mutual authentication
	tlsConfig := &tls.Config{
		Certificates: []tls.Certificate{serverCert},
		ClientCAs:    caCertPool,
		ClientAuth:   tls.RequireAndVerifyClientCert, // Enforce client certificate authentication
	}

	// Create a new multiplexer and apply CORS middleware
	mux := http.NewServeMux()

	// Register your handlers
	mux.HandleFunc("/hello", func(w http.ResponseWriter, r *http.Request) {
		fmt.Fprintf(w, "Hello, mTLS World!")
	})

	// Create server with the wrapped handler
	server := &http.Server{
		Addr:      ":8443",
		TLSConfig: tlsConfig,
		Handler:   corsMiddleware(mux), // Apply CORS middleware to all routes
	}

	log.Println("mTLS API Server running on https://localhost:8443")
	log.Fatal(server.ListenAndServeTLS("./certs/server.crt", "./certs/server.key"))
}
