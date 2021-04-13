FROM golang:1.16.3-buster AS build
COPY odbc-ipfs-go-node .
RUN go build
RUN odbc-ipfs-go-node.exe
FROM ipfs/go-ipfs:master-latest
EXPOSE 4001
EXPOSE 5001
EXPOSE 8080