const express = require('express');
const { spawn } = require('child_process');
const fs = require('fs');
const cors = require('cors');
const path = require('path');

const app = express();
const port = 3001;

app.use(cors());
app.use(express.json({ limit: '50mb' }));

// Endpoint to process the DNA sequence
app.post('/process-dna', (req, res) => {
    const { dnaSequence } = req.body;

    if (!dnaSequence) {
        return res.status(400).send('DNA sequence is required');
    }

    console.log('Received DNA sequence:', dnaSequence);

    // Step 1: Create a parent directory for DNA requests if it doesn't exist
    const dnaRequestDir = path.join(__dirname, 'dna_request');
    if (!fs.existsSync(dnaRequestDir)) {
        fs.mkdirSync(dnaRequestDir);
    }

    // Step 2: Create a unique folder for this request
    const uniqueFolderName = `dna_${Date.now()}`;
    const dnaFolderPath = path.join(dnaRequestDir, uniqueFolderName);
    fs.mkdirSync(dnaFolderPath);

    // Step 3: Write the DNA sequence to a .fasta file
    const fastaFileName = `${uniqueFolderName}.fasta`;
    const fastaFilePath = path.join(dnaFolderPath, fastaFileName);
    const fastaContent = `>DNA_sequence\n${dnaSequence}`;
    fs.writeFileSync(fastaFilePath, fastaContent, 'utf8');

    console.log('FASTA file created at:', fastaFilePath);

    // Step 4: Call the Python script
    const pythonScriptPath = './blast_process.py';
    const process = spawn('python', [pythonScriptPath, fastaFilePath]);

    let output = '';
    let errorOutput = '';

    process.stdout.on('data', (data) => {
        output += data.toString();
        console.log('Python stdout:', data.toString());
    });

    process.stderr.on('data', (data) => {
        errorOutput += data.toString();
        console.error('Python stderr:', data.toString());
    });

    process.on('close', (code) => {
        console.log(`Python script exited with code: ${code}`);
        if (code !== 0) {
            console.error(`Error executing Python script: ${errorOutput || 'No error output captured'}`);
            return res.status(500).send('Error processing DNA sequence');
        }

        // Step 5: Move the result file to the unique folder
        const resultFileName = output.trim(); // The result filename from Python output
        const resultFilePath = path.join(__dirname, resultFileName); // Original location

        const newResultFileName = `${uniqueFolderName}_blast`;
        const newResultFilePath = path.join(dnaFolderPath, newResultFileName);

        fs.rename(resultFilePath, newResultFilePath, (err) => {
            if (err) {
                console.error('Error moving result file:', err);
                return res.status(500).send('Error moving result file');
            }

            console.log(`Moved result file to: ${newResultFilePath}`);

            // Step 6: Call the C++ executable
            const executablePath = './proccess.exe';
            const cppProcess = spawn(executablePath, [fastaFileName, newResultFilePath]);

            cppProcess.stdout.on('data', (data) => {
                console.log('C++ executable output:', data.toString());
            });

            cppProcess.stderr.on('data', (data) => {
                console.error('C++ executable error:', data.toString());
            });

            cppProcess.on('close', (code) => {
                console.log(`C++ executable exited with code: ${code}`);
                if (code === 0) {
                    res.json({ message: 'Successfully processed DNA and sent to C++ executable' });
                } else {
                    res.status(500).send('Error processing with C++ executable');
                }
            });
        });
    });
});

// Start the server
app.listen(port, () => {
    console.log(`Server running at http://localhost:${port}`);
});
