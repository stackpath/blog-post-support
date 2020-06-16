# Stackpath + Algolia

This Node Express app demonstrates using StackPath to generate a private Algolia key on the edge rather than using a traditional server-side Node app. 

## Running this app locally
- Set up an Algolia index using [this CSV file of Nobel Prize Winners](https://github.com/OpenRefine/OpenRefine/blob/master/main/tests/data/nobel-prize-winners.csv)
- Build the Docker image: `docker build -t <YOUR_USERNAME>/stackpath-algolia .`
- Run the container with your Algolia credentials and a randomly chosen `ALGOLIA_SECRET` string (used to encrypt secured keys): 

```bash
docker run --rm -p 8000:80 -e ALGOLIA_APP_ID=... \
  -e ALGOLIA_APP_KEY=... \
  -e ALGOLIA_SECRET=... \
  -d <YOUR_USERNAME>/stackpath-algolia
```
