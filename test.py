import subprocess
import time

def test_FEN(FEN_string):
    t0 = time.perf_counter()
    output = subprocess.run("./chess", capture_output=True, text=True, input=FEN_string)
    t1 = time.perf_counter()
    print(output.stdout)
    print("Time:", t1-t0, "s")

test_FEN("4r3/KB6/1B6/3N3q/8/8/8/7k w - - 0 1")
test_FEN("4r3/KB6/1B6/3N4/8/8/8/4q2k b - - 0 1")
test_FEN("rnb1k1nr/ppqp1ppp/3pp3/6b1/3P4/2N2N2/PPP1PPPP/R1BQKB1R w KQkq - 0 1")
#test_FEN("2bqkbnr/5ppp/pB1pp3/p2N4/4P3/P4N2/2P1QPPP/1R4K1 b k - 0 1")
#test_FEN("3B1bnr/1Q3ppp/3ppk2/p7/4P3/P4N2/2q2PPP/1R4K1 b - - 0 1")
