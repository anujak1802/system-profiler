import { useEffect, useState } from "react";
import axios from "axios";
import {
  LineChart, Line, CartesianGrid, XAxis, YAxis, Tooltip, ResponsiveContainer,
} from "recharts";
import "./App.css";

function App() {
  const [data, setData] = useState([]);

  useEffect(() => {
    const fetchData = async () => {
      const res = await axios.get("http://localhost:8000/metrics");
      const merged = res.data.cpu.map((cpu, i) => ({
        cpu,
        mem: res.data.mem[i],
        time: i,
      }));
      setData(merged);
    };
    fetchData();
    const interval = setInterval(fetchData, 2000);
    return () => clearInterval(interval);
  }, []);

  return (
    <>
      {/* full-screen blur layer */}
      <div className="bg-overlay" />

      {/* centered content */}
      <div className="app">
        <div className="glass-card">
          <h1 className="title">System Resource Profiler</h1>
          <div className="chart-wrap">
            <ResponsiveContainer width="100%" height="100%">
              <LineChart data={data} margin={{ top: 10, right: 20, bottom: 10, left: 0 }}>
                <Line type="monotone" dataKey="cpu" dot={false} strokeWidth={2} />
                <Line type="monotone" dataKey="mem" dot={false} strokeWidth={2} />
                <CartesianGrid strokeOpacity={0.2} />
                <XAxis dataKey="time" tick={{ fill: "#c7ccff" }} />
                <YAxis tick={{ fill: "#c7ccff" }} />
                <Tooltip />
              </LineChart>
            </ResponsiveContainer>
          </div>
        </div>
      </div>
    </>
  );
}

export default App;
