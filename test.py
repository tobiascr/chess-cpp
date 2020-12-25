import subprocess
import time

total_time = 0

def test_FEN(FEN_string):
    t0 = time.perf_counter()
    output = subprocess.run("./chess", capture_output=True, text=True, input=FEN_string)
    t1 = time.perf_counter()
    print(output.stdout)
    print("Time:", t1-t0, "s")
    global total_time
    total_time += t1 - t0

# Starting position
test_FEN("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1")

test_FEN("4r3/KB6/1B6/3N3q/8/8/8/7k w - - 0 1")
test_FEN("4r3/KB6/1B6/3N4/8/8/8/4q2k b - - 0 1")
test_FEN("rnb1k1nr/ppqp1ppp/3pp3/6b1/3P4/2N2N2/PPP1PPPP/R1BQKB1R w KQkq - 0 1")
test_FEN("r4r2/1pp4k/p3P2p/2Pp1p2/bP5Q/P3q3/1B1n4/K6R w - - 1 33")
test_FEN("2bqkbnr/5ppp/pB1pp3/p2N4/4P3/P4N2/2P1QPPP/1R4K1 b k - 0 1")
test_FEN("3B1bnr/1Q3ppp/3ppk2/p7/4P3/P4N2/2q2PPP/1R4K1 b - - 0 1")

# Lichess puzzle, best move: b7a6
test_FEN("r4rk1/1b2q1p1/2n1p2p/1Qb1Pp2/3p1P2/P4N1N/2PB2PP/R4R1K b - - 2 21")

# Lichess puzzle, best move: e8e1
test_FEN("k3r2r/p2Q4/2p4p/6p1/1R3pq1/2P5/PP3P1P/3R3K b - - 13 29")

# Lichess puzzle, best move: h6h7
test_FEN("1r3qrk/p1b4p/4pPpQ/2p3P1/P1PpP1R1/3n3P/8/2B2RK1 w - - 4 32")

# Blacks king is in check but might be tempted to capture a queen instead of moving
# away from the check.
8/2R5/1R3n2/7Q/6p1/8/5K2/1k6 b - - 0 1

print()
print("Total time:", total_time)
