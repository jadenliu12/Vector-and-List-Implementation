all:
	g++ I2P2_main.cpp src/*.cpp -DTEST_LIST -DTEST_VECTOR -DDOUBLE -std=c++11