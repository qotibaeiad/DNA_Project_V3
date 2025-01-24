import React, { useState } from 'react';
import axios from 'axios';
import { TextField, Button, Container, Typography, Box, FormControl, Snackbar, Alert, CircularProgress } from '@mui/material';
import { CloudUpload as CloudUploadIcon } from '@mui/icons-material';
import backgroundImage from './background.png';

function App() {
  const [dnaSequence, setDnaSequence] = useState('');
  const [email, setEmail] = useState('');
  const [processedDna, setProcessedDna] = useState('');
  const [loading, setLoading] = useState(false);
  const [emailLoading, setEmailLoading] = useState(false);
  const [file1, setFile1] = useState(null);
  const [file2, setFile2] = useState(null);
  const [fileName1, setFileName1] = useState('');
  const [fileName2, setFileName2] = useState('');
  const [alertOpen, setAlertOpen] = useState(false);
  const [alertMessage, setAlertMessage] = useState('');
  const [alertSeverity, setAlertSeverity] = useState('error');

  const showAlert = (message, severity = 'error') => {
    setAlertMessage(message);
    setAlertSeverity(severity);
    setAlertOpen(true);
  };

  const handleSubmit = async (e) => {
    e.preventDefault();

    if (!email) {
      showAlert('Please enter your email.', 'warning');
      return;
    }

    let sequenceToProcess1 = dnaSequence;
    let sequenceToProcess2 = '';

    if (!dnaSequence && !file1) {
      showAlert('Please enter a DNA sequence or upload the first FASTA file.', 'warning');
      return;
    }

    if (!file1 && !file2) {
      showAlert('Please upload the first and second FASTA files.', 'warning');
      return;
    }

    // Handle first file upload
    if (file1) {
      try {
        const fileText1 = await file1.text();
        sequenceToProcess1 = fileText1.split('\n').slice(1).join('').trim();
      } catch (error) {
        showAlert('Error reading the first FASTA file.', 'error');
        console.error(error);
        return;
      }
    }

    // Handle second file upload
    if (file2) {
      try {
        const fileText2 = await file2.text();
        sequenceToProcess2 = fileText2.split('\n').slice(1).join('').trim();
      } catch (error) {
        showAlert('Error reading the second FASTA file.', 'error');
        console.error(error);
        return;
      }
    }

    setLoading(true);

    setTimeout(async () => {
      try {
        setLoading(false);
        showAlert('Processing... The result will be sent to your email.', 'success');

        // Now proceed with the actual server request
        const response = await axios.post('http://localhost:3001/process-dna', {
          dnaSequence: sequenceToProcess1,
          dnaSequence2: sequenceToProcess2,
          email: email,
        });

        setProcessedDna(response.data.processedDna);

        setEmailLoading(true);
        setTimeout(() => {
          setEmailLoading(false);
          showAlert('The result will be sent to your email.', 'success');
        }, 2000);

      } catch (error) {
        console.error('Error processing DNA:', error);
        showAlert('Error processing DNA sequence.', 'error');
      }
    }, 2000); // Wait 2 seconds before showing alert
  };

  const copyToClipboard = () => {
    navigator.clipboard.writeText(processedDna);
    showAlert('Processed DNA sequence copied to clipboard!', 'success');
  };

  const handleFileChange1 = (e) => {
    const uploadedFile = e.target.files[0];
    setFile1(uploadedFile);
    setFileName1(uploadedFile ? uploadedFile.name : '');
  };

  const handleFileChange2 = (e) => {
    const uploadedFile = e.target.files[0];
    setFile2(uploadedFile);
    setFileName2(uploadedFile ? uploadedFile.name : '');
  };

  return (
    <div style={{
      backgroundImage: `url(${backgroundImage})`,
      backgroundSize: 'cover',
      backgroundPosition: 'center',
      minHeight: '100vh',
      color: '#fff',
      display: 'flex',
      justifyContent: 'center',
      alignItems: 'center',
    }}>
      <Container maxWidth="md" style={{ textAlign: 'center' }}>
        <Typography variant="h3" gutterBottom sx={{ color: 'blue', fontWeight: 'bold' }}>
          DNA Sequence Processor
        </Typography>

        <form onSubmit={handleSubmit} style={{ marginBottom: '20px', display: 'flex', flexDirection: 'column', alignItems: 'center' }}>
          <Box sx={{ display: 'flex', flexDirection: 'column', width: '100%', maxWidth: '500px', mb: 2 }}>
            <TextField
              id="email-helper-text"
              label="Email"
              variant="outlined"
              fullWidth
              value={email}
              onChange={(e) => setEmail(e.target.value)}
              style={{ backgroundColor: '#fff', borderRadius: '4px' }}
            />
          </Box>

          <Box mb={2} style={{ width: '100%', maxWidth: '500px' }}>
            <TextField
              label="Enter DNA Sequence"
              variant="outlined"
              fullWidth
              value={dnaSequence}
              onChange={(e) => setDnaSequence(e.target.value)}
              multiline
              rows={4}
              style={{ backgroundColor: '#fff', borderRadius: '4px' }}
            />
          </Box>

          <Box mb={2} style={{ width: '100%', maxWidth: '500px' }}>
            <FormControl fullWidth>
              <Box display="flex" alignItems="center" justifyContent="space-between">
                <Button
                  component="label"
                  variant="contained"
                  color="secondary"
                  startIcon={<CloudUploadIcon />}
                >
                  {fileName1 || 'Choose First File'}
                  <input type="file" accept=".fasta" onChange={handleFileChange1} hidden />
                </Button>
              </Box>
            </FormControl>
          </Box>

          <Box mb={2} style={{ width: '100%', maxWidth: '500px' }}>
            <FormControl fullWidth>
              <Box display="flex" alignItems="center" justifyContent="space-between">
                <Button
                  component="label"
                  variant="contained"
                  color="secondary"
                  startIcon={<CloudUploadIcon />}
                >
                  {fileName2 || 'Choose Second File'}
                  <input type="file" accept=".fasta" onChange={handleFileChange2} hidden />
                </Button>
              </Box>
            </FormControl>
          </Box>

          <Box mb={2} style={{ width: '100%', maxWidth: '500px', display: 'flex', justifyContent: 'flex-start' }}>
            <Button
              type="submit"
              variant="contained"
              color="primary"
              style={{ width: 'auto', padding: '8px 16px', marginTop: '10px', display: 'flex', alignItems: 'center' }}
              disabled={loading || emailLoading}
            >
              {loading ? <CircularProgress size={24} style={{ color: 'white' }} /> : 'Process DNA'}
            </Button>
          </Box>
        </form>

        {processedDna && (
          <div>
            <Typography variant="h6" gutterBottom>Processed DNA Sequence</Typography>
            <Box style={{ maxHeight: '200px', overflowY: 'auto', padding: '10px', border: '1px solid #ccc', borderRadius: '4px', backgroundColor: '#f9f9f9' }}>
              <div style={{ fontFamily: 'monospace', whiteSpace: 'pre-wrap' }}>{processedDna}</div>
            </Box>
            <Button variant="contained" color="success" onClick={copyToClipboard} style={{ marginTop: '10px' }}>
              Copy to Clipboard
            </Button>
          </div>
        )}

        <Snackbar open={alertOpen} autoHideDuration={3000} onClose={() => setAlertOpen(false)}>
          <Alert onClose={() => setAlertOpen(false)} severity={alertSeverity} sx={{ width: '100%' }}>
            {alertMessage}
          </Alert>
        </Snackbar>
      </Container>
    </div>
  );
}

export default App;
