from fastapi import FastAPI
import csv
from fastapi.middleware.cors import CORSMiddleware

app = FastAPI()

app.add_middleware(
    CORSMiddleware,
    allow_origins=["*"],
    allow_methods=["*"],
    allow_headers=["*"],
)

@app.get("/metrics")
def get_metrics():
    with open("../cpp/metrics.log") as f:
        data = list(csv.reader(f))
    latest = data[-10:]  # send last 10 readings
    return {"cpu": [float(x[0]) for x in latest], "mem": [float(x[1]) for x in latest]}
