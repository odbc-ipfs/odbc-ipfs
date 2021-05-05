FROM golang:1.16.3-buster

WORKDIR /src

COPY . .

WORKDIR /src/odbc-ipfs-go-node-main

RUN env GOOS=linux go build -o app

CMD ["./app"]
