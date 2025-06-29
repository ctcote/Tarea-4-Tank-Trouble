CXX = g++
CXXFLAGS = -std=c++17 -Wall -O2

TARGET = TankTroubleMazes
SRC = TankTroubleMazes.cpp

all: $(TARGET)

$(TARGET): $(SRC)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(SRC)

clean:
	rm -f $(TARGET)