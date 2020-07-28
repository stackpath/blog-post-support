const express = require('express');
const sha1 = require('sha1');
const fetch = require('node-fetch');
const NodeCache = require( "node-cache" );
const port = process.env.PORT || 80;

const app = express();
app.use(express.json());
app.listen(port);

const hashedPasswordCache = new NodeCache({ stdTTL: 3600 });

const generateResponse = (res, text, suffix) => text.includes(suffix) ?
  res.status(400).json({message: 'Please select a more secure password. This one has already been Pwned.'}) :
  res.status(200).json({message: 'Your password is secure and you may use it to register.'});

app.post('/auth', (req, res) => {
  // Get the password from the request
  const password = req.body['password'];

  if (password) {
    // Shah-1 hash it and break it into a prefix and suffix
    const hashedPassword = sha1(password).toUpperCase();
    const prefix = hashedPassword.substring(0, 5);
    const suffix = hashedPassword.substring(5);

    // Check against cache
    const cachedResults = hashedPasswordCache.get(prefix);
    if (cachedResults) {
      // Check if the cached result includes the suffix of the hash
      return generateResponse(res, cachedResults, suffix);
    } else {
      // Send the first 5 chars to pwned password API
      fetch('https://api.pwnedpasswords.com/range/' + prefix)
        .then(tmp => tmp.text())
        .then(body => {
          // Cache the response for future use
          hashedPasswordCache.set(prefix, body);
          // Check if the response includes the suffix of the hash
          return generateResponse(res, body, suffix);
        }).catch(err => {
        // Handle any API errors
        console.error(err);
        res.status(400).json({message: 'Something went wrong.'});
      });
    }
  } else {
    res.status(400).json({message: '"password" field not found.'});
  }
});

console.log(`Listening on ${port}`);
