FROM ipfs/go-ipfs:master-latest
RUN cd odbc-ipfs/odbc-ipfs-go-node
RUN go build
RUN odbc-ipfs-go-node.exe
EXPOSE 4001
EXPOSE 5001
EXPOSE 8080