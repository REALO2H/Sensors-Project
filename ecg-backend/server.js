const express = require('express');
const mongoose = require('mongoose');
const cors = require('cors');
const fs = require('fs'); // For optional HTTPS
const http = require('http');
const https = require('https');

const app = express();

// Enable CORS
app.use(cors({ origin: '*', methods: ['GET', 'POST'] })); // Adjust as needed for production
app.use(express.json());

// Connect to MongoDB
mongoose
  .connect('mongodb://localhost:27017/ecgDB', { useNewUrlParser: true, useUnifiedTopology: true })
  .then(() => console.log('MongoDB connected'))
  .catch((err) => console.error(err));

// Define a schema and model for ECG readings
const EcgSchema = new mongoose.Schema({
  value: Number,
  timestamp: { type: Date, default: Date.now },
});

const EcgReading = mongoose.model('EcgReading', EcgSchema);

// POST endpoint to receive ECG data
app.post('/api/ecg', async (req, res) => {
  try {
    const { value } = req.body;
    const newReading = new EcgReading({ value });
    await newReading.save();
    res.status(201).json(newReading);
  } catch (error) {
    res.status(400).json({ error: error.message });
  }
});

// GET endpoint to retrieve recent ECG data
app.get('/api/ecg', async (req, res) => {
  try {
    const readings = await EcgReading.find().sort({ timestamp: -1 }).limit(1000);
    res.json(readings);
  } catch (error) {
    res.status(400).json({ error: error.message });
  }
});

// Choose a port that isn’t in use—default is 5000
const HTTP_PORT = 5000;

// Optional: HTTPS configuration
/*
const HTTPS_PORT = 5443; // Choose an HTTPS port
const sslOptions = {
  key: fs.readFileSync('key.pem'),
  cert: fs.readFileSync('cert.pem'),
};

// Start HTTPS server
https.createServer(sslOptions, app).listen(HTTPS_PORT, '0.0.0.0', () => {
  console.log(`HTTPS Server running on https://0.0.0.0:${HTTPS_PORT}`);
});
*/

// Start HTTP server
app.listen(HTTP_PORT, '0.0.0.0', () => {
  console.log(`HTTP Server running on http://0.0.0.0:${HTTP_PORT}`);
});
