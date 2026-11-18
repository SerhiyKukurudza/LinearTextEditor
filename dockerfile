FROM ubuntu:latest

RUN apt update && apt install -y valgrind build-essential

WORKDIR /app
COPY . .

CMD ["/bin/bash"]