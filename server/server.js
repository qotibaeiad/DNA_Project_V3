const express = require('express');
const { spawn } = require('child_process');
const fs = require('fs');
const cors = require('cors');
const path = require('path');
const app = express();
const port = 3001;

app.use(cors());
app.use(express.json({ limit: '50mb' })); // Increase to 50MB or adjust as needed

app.post('/process-dna', (req, res) => {
    const { dnaSequence, dnaSequence2, email } = req.body;

    if (!dnaSequence) {
        console.error('❌ Error: DNA sequence is missing in the request');
        return res.status(400).json({ error: 'DNA sequence is required' });
    }

    console.log('Received DNA sequence:', dnaSequence);

    // Step 1: Create a parent directory for DNA requests if it doesn't exist
    const dnaRequestDir = path.join(__dirname, 'dna_request');
    if (!fs.existsSync(dnaRequestDir)) {
        fs.mkdirSync(dnaRequestDir);
    }

    // Step 2: Create a unique folder for this DNA sequence request
    const uniqueFolderName = `dna_${Date.now()}`;
    const dnaFolderPath = path.join(dnaRequestDir, uniqueFolderName);
    if (!fs.existsSync(dnaFolderPath)) {
        fs.mkdirSync(dnaFolderPath);
    }

    // Step 3: Create a FASTA file with the DNA sequence inside the unique folder
    const fileName = `${uniqueFolderName}.fasta`;  // Just the file name
    const filePath = path.join(dnaFolderPath, fileName);  // Full path for file creation
    const fastaContent = `>DNA_sequence\n${dnaSequence}`;
    fs.writeFileSync(filePath, fastaContent, 'utf8');

    console.log('FASTA file created at:', filePath);

    // Step 4: Call the C++ executable with only the file name (not the full path)
    const executablePath = './proccess.exe';
    const fileNameToSend = fileName;  // Sending only the file name
    const process = spawn(executablePath, [fileNameToSend]);

    let output = '';
    let errorOutput = '';

    // Capture stdout from the C++ process
    process.stdout.on('data', (data) => {
        output += data.toString();
        console.log('C++ stdout:', data.toString());
    });

    // Capture stderr from the C++ process
    process.stderr.on('data', (data) => {
        errorOutput += data.toString();
        console.error('C++ stderr:', data.toString());
    });

    // After the C++ process finishes
    process.on('close', (code) => {
        console.log(`C++ program exited with code: ${code}`);
        if (code !== 0) {
            console.error(`Error executing C++ program: ${errorOutput || 'No error output captured'}`);
            return res.status(500).send('Error processing DNA sequence');
        }

        console.log('Processed DNA:', output.trim());

        // Send the processed DNA result back to the client
        return res.json({ processedDna: output.trim() });
    });
});

app.listen(port, () => {
    console.log(`Server running at http://localhost:${port}`);
});
