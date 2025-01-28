const express = require('express');
const { spawn } = require('child_process');
const fs = require('fs');
const cors = require('cors');
const path = require('path');
const nodemailer = require('nodemailer');
const archiver = require('archiver');

const app = express();
const port = 3001;

app.use(cors());
app.use(express.json({ limit: '50mb' }));

// Setup Gmail email transporter using nodemailer
const transporter = nodemailer.createTransport({
    service: 'gmail',
    auth: {
        user: 'dna.primes@gmail.com',  // Replace with your Gmail address
        pass: 'tvmv blrk kkbh hmaa',  // Replace with your Gmail app password (ensure it's kept private)
    },
});

// Function to compress file into ZIP
function compressFile(filePath, outputDir, callback) {
    const zipFilePath = path.join(outputDir, 'results.zip');
    const output = fs.createWriteStream(zipFilePath);
    const archive = archiver('zip', { zlib: { level: 9 } });

    output.on('close', () => {
        console.log(`Zip file created: ${archive.pointer()} total bytes`);
        callback(zipFilePath);
    });

    archive.pipe(output);
    archive.file(filePath, { name: 'list_prim_tripl' });
    archive.finalize();
}

// Function to send an email with ZIP attachment
function sendEmail(zipFilePath, recipientEmail, res) {
    const mailOptions = {
        from: 'dna.primes@gmail.com',
        to: recipientEmail,
        subject: 'Your DNA Processing Results',
        text: 'Your DNA sequence has been processed successfully. Please find the results attached.',
        attachments: [{ filename: 'results.zip', path: zipFilePath }],
    };

    transporter.sendMail(mailOptions, (error, info) => {
        if (error) {
            console.error('Error sending email:', error);
            return res.status(500).send('Error sending email');
        }
        console.log('Email sent: ' + info.response);
        res.json({ message: 'Successfully processed DNA and email sent to client' });
    });
}

// Endpoint to process the DNA sequence
app.post('/process-dna', (req, res) => {
    const { dnaSequence, dnaSequence2, email } = req.body;
    if (!dnaSequence || !dnaSequence2 || !email) {
        return res.status(400).send('DNA sequence and client email are required');
    }

    console.log('Received DNA sequence:', dnaSequence);

    const dnaRequestDir = path.join(__dirname, 'dna_request');
    if (!fs.existsSync(dnaRequestDir)) fs.mkdirSync(dnaRequestDir);

    const uniqueFolderName = `dna_${Date.now()}`;
    const dnaFolderPath = path.join(dnaRequestDir, uniqueFolderName);
    fs.mkdirSync(dnaFolderPath);

    const fastaFileName = `${uniqueFolderName}.fasta`;
    const fastaFilePath = path.join(dnaFolderPath, fastaFileName);
    fs.writeFileSync(fastaFilePath, `>DNA_sequence\n${dnaSequence}`, 'utf8');

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
        const resultFileName = output.trim();  // The result filename from Python output
        const resultFilePath = path.join(__dirname, resultFileName);  // Original location

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
            console.log(newResultFilePath);
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
                    // Step 7: Check if the file exists inside the unique folder
                    const resultFilePath = path.join(dnaFolderPath, 'list_prim_tripl'); // The file we expect from C++
                    if (fs.existsSync(resultFilePath)) {
                        console.log('File found, compressing before sending...');
                        compressFile(resultFilePath, dnaFolderPath, (zipFilePath) => {
                            sendEmail(zipFilePath, email, res);
                        });
                    } else {
                        console.error('File not found in the unique folder:', resultFilePath);
                        res.status(500).send('Processed file not found');
                    }
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
