const axios = require('axios');

// Function to send DNA sequence for BLAST search
async function blastDNASequence(dnaSequence) {
  try {
    // Step 1: Submit the DNA sequence to BLAST (Put command)
    const response = await axios.post('https://blast.ncbi.nlm.nih.gov/Blast.cgi', null, {
      params: {
        CMD: 'Put', // Command to initiate BLAST query
        PROGRAM: 'blastn', // BLASTN for nucleotide sequences (DNA)
        DATABASE: 'nt', // nt = NCBI Nucleotide Database
        QUERY: dnaSequence, // The DNA sequence you are querying
        FORMAT_TYPE: 'JSON', // Response format (could also be 'JSON')
      },
    });

    // Extract the RID (Request ID) from the response
    const requestId = response.data.split('RID = ')[1].split('\n')[0];
    
    if (!requestId) {
      console.error('Error: No RID returned');
      return;
    }

    console.log('BLAST request ID:', requestId);

    // Step 2: Retrieve the BLAST results using the RID (Get command)
    await getBlastResults(requestId);
  } catch (error) {
    console.error('Error submitting DNA sequence to BLAST:', error.message);
  }
}

// Function to fetch results using the request ID
async function getBlastResults(requestId) {
  try {
    // Periodically check for the results
    console.log('Fetching results for RID:', requestId);

    const resultResponse = await axios.get('https://blast.ncbi.nlm.nih.gov/Blast.cgi', {
      params: {
        CMD: 'Get', // Command to fetch BLAST results
        RID: requestId, // Use the request ID received in the first step
        FORMAT_TYPE: 'XML', // Response format (could also be 'JSON')
      },
    });

    if (resultResponse.data.includes('Status=WAITING')) {
      console.log('Results not ready yet, retrying...');
      // Add a delay here before retrying
      setTimeout(() => getBlastResults(requestId), 5000); // Retry after 5 seconds
    } else {
      // Process the results (XML in this case)
      console.log('BLAST results:', resultResponse.data);
    }
  } catch (error) {
    console.error('Error fetching BLAST results:', error.message);
  }
}

// Example DNA sequence (Replace with your actual sequence)
const dnaSequence = 'ATTAAAGGTTTATACCTTCCCAGGTAACAAACCAACCAACTTTCGATCTCTTGTAGATCTGTTCTCTAAACGAACTTTAA'; // Replace with your DNA sequence
blastDNASequence(dnaSequence);
