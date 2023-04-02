import subprocess
import time

url = "http://localhost:8888/"

start_time = time.time()

# Run 1000 requests with 10 concurrent users
result = subprocess.run(["ab", "-n", "1000", "-c", "10", url], capture_output=True, text=True)

end_time = time.time()

print(result.stdout)

print(f"Test completed in {end_time - start_time:.2f} seconds")
