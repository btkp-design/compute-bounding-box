all:
	c++ -O3 bounding-onion.cpp -DCGAL_EIGEN3_ENABLED -I/usr/include/eigen3/ -lgmp -o compute-bounding-box
