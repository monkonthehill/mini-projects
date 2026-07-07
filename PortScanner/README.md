# PortScanner

A fast, multithreaded TCP port scanner with banner grabbing, written in C++17.

## Features

- **Concurrent scanning** — scans multiple ports in parallel using a configurable thread pool (defaults to `hardware_concurrency() * 4`).
- **Non-blocking connects** — uses `poll()` with a short timeout per connection attempt instead of blocking sockets, keeping scans fast even against filtered/unresponsive hosts.
- **Banner grabbing** — for each open port, attempts to read a banner passively first (e.g. SSH, FTP), then falls back to sending a benign `GET / HTTP/1.0` probe for services that wait for input (e.g. HTTP, CUPS).
- **Flexible port specification** — scan a single port, a range, a comma-separated list, or any combination.
- **IPv4 and IPv6 support** via `getaddrinfo`.
- **CLI-first design** — all options are passed as arguments; no interactive prompts.

## Requirements

- A C++17 compiler (g++ or clang++)
- POSIX sockets (Linux/macOS)
- pthreads

## Build

```bash
g++ -std=c++17 -pthread src/cli.cpp -o scanner-cli
```

## Usage

```bash
./scanner-cli -H <host> -p <ports> [-t <threads>]
```

| Flag | Long form   | Description                                                                                   | Default                      |
|------|-------------|------------------------------------------------------------------------------------------------|-------------------------------|
| `-H` | `--host`    | Target host or IP address                                                                     | `127.0.0.1`                   |
| `-p` | `--ports`   | Port(s) to scan: single (`80`), range (`20-100`), list (`22,80,443`), or mixed (`20-100,443`)  | *(required)*                   |
| `-t` | `--threads` | Maximum number of concurrent scanning threads                                                 | `hardware_concurrency() * 4`   |
| `-h` | `--help`    | Show usage information                                                                         | —                              |

### Examples

Scan a single port:
```bash
./scanner-cli -H 127.0.0.1 -p 443
```

Scan a range:
```bash
./scanner-cli -H scanme.nmap.org -p 20-100
```

Scan a specific list of ports:
```bash
./scanner-cli -H 127.0.0.1 -p 22,80,443
```

Scan a range plus extra ports, with a custom thread cap:
```bash
./scanner-cli -H 192.168.1.1 -p 1-1024,8080,8443 -t 32
```

## Output

**Single port:**
```
127.0.0.1:443 is OPEN
Scan duration: 12 ms
```

**Multiple ports:**
```
Scanning 127.0.0.1 (11 ports)...
[Port 5000] Connected, but service remained silent to probes.
[Port 5001] Banner Received (Passive): SSH-2.0-OpenSSH_9.6

Scan Complete. Open ports (2): 5000, 5001
Scan duration: 401 ms
```

## Testing

Since ground-truth is needed to verify results, test against hosts/ports you control:

**Local listeners:**
```bash
nc -l 4444 &
./scanner-cli -H 127.0.0.1 -p 4444    # OPEN
./scanner-cli -H 127.0.0.1 -p 4445    # CLOSED
```

**Banner grab (passive):**
```bash
echo -e "SSH-2.0-TestBanner\r\n" | nc -l 6000 &
./scanner-cli -H 127.0.0.1 -p 6000
```

**Docker container with a known exposed port:**
```bash
docker run -d -p 8080:80 nginx
./scanner-cli -H 127.0.0.1 -p 8080
```

**Public test target:** [scanme.nmap.org](https://scanme.nmap.org) is explicitly provided by the Nmap project for scanning practice.

> ⚠️ Only scan hosts you own, control, or have explicit permission to test (e.g. `scanme.nmap.org`, your own machine, or your own Docker containers). Scanning third-party hosts without authorization may violate their terms of service or local law.

## Project Structure

```
PortScanner/
├── include/
│   └── scanner.hpp     # PortState enum, shared state, function declarations
├── src/
│   ├── scanner.cpp     # Core connect + banner-grab logic
│   ├── cli.cpp         # CLI entry point (argument parsing, thread orchestration)
│   └── main.cpp        # Legacy interactive entry point
└── README.md
```

## How It Works

1. Resolve the target host via `getaddrinfo`.
2. For each requested port, spawn a thread that:
   - Opens a non-blocking socket and attempts to connect.
   - Uses `poll()` to wait for the connection to complete (or fail) within a short timeout.
   - On success, polls for readable data:
     - If the service sends data unprompted, reads and reports it as a **passive** banner.
     - Otherwise, sends a minimal `GET / HTTP/1.0` probe and polls again for an **active** banner.
   - Records the port as open in a shared, mutex-protected results vector.
3. Threads are dispatched in batches capped by `--threads` to avoid exhausting file descriptors on large scans.
4. Results are collected, sorted, and printed with total scan duration.

## License

Add your license of choice here.
