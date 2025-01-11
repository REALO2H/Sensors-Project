import React, { useState, useEffect } from "react";
import axios from "axios";
import { LineChart, Line, XAxis, YAxis, CartesianGrid, Tooltip } from "recharts";
import "./App.css";

const App = () => {
  const [ecgData, setEcgData] = useState([]); // State to hold ECG data
  const [isFetching, setIsFetching] = useState(true); // State to control data fetching
  const [restartKey, setRestartKey] = useState(0); // Unique key for re-rendering the chart

  useEffect(() => {
    if (!isFetching) return;

    // Fetch ECG data periodically
    const fetchData = async () => {
      try {
        const response = await axios.get("http://192.168.1.240:5000/api/ecg");
        const formattedData = response.data.map((item) => ({
          timestamp: new Date(item.timestamp).toLocaleTimeString(),
          value: item.value,
        }));
        setEcgData((prevData) => [...prevData, ...formattedData].slice(-100)); // Keep the last 100 points
      } catch (error) {
        console.error("Error fetching ECG data:", error);
      }
    };

    const interval = setInterval(fetchData, 1000);
    return () => clearInterval(interval); // Cleanup on component unmount
  }, [isFetching]);

  

  const handleRestart = () => {
    if (window.confirm("Are you sure you want to restart?")) {
      setIsFetching(false);
      setEcgData([]);
      setRestartKey((prevKey) => prevKey + 1);
      setTimeout(() => setIsFetching(true), 500);
    }
  };
  return (
    <div className="App">
      <h1>Real-Time ECG Monitor</h1>
      <LineChart
        key={restartKey} // Unique key forces re-render
        width={800}
        height={400}
        data={ecgData}
        margin={{ top: 20, right: 30, left: 20, bottom: 5 }}
      >
        <CartesianGrid strokeDasharray="3 3" />
        <XAxis dataKey="timestamp" />
        <YAxis domain={["auto", "auto"]} />
        <Tooltip />
        <Line type="monotone" dataKey="value" stroke="#8884d8" dot={false} />
      </LineChart>
      <button onClick={handleRestart}>Restart ECG Plotting</button>
    </div>
  );
};

export default App;
