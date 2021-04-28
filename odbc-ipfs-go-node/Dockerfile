FROM golang:1.16.3-buster

WORKDIR /src

COPY . .

RUN env GOOS=linux go build -o app

CMD ["./app"]
