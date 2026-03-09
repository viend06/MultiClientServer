CXX = g++
CXXFLAGS = -std=c++17 -pthread

SERVER_SRC = Server.cpp transport/send_recv.cpp
CLIENT_SRC = Clients.cpp transport/send_recv.cpp

SERVER_BIN = server
CLIENT_BIN = client

all: $(SERVER_BIN) $(CLIENT_BIN)

$(SERVER_BIN): $(SERVER_SRC)
	$(CXX) $(CXXFLAGS) $(SERVER_SRC) -o $(SERVER_BIN)

$(CLIENT_BIN): $(CLIENT_SRC)
	$(CXX) $(CXXFLAGS) $(CLIENT_SRC) -o $(CLIENT_BIN)

run: all
	wt.exe new-tab wsl ./server \; \
	new-tab wsl ./client \; \
	new-tab wsl ./client

clean:
	rm -f $(SERVER_BIN) $(CLIENT_BIN)