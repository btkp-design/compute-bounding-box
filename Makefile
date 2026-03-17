CXX=x86_64-w64-mingw32-c++
CFLAGS := -O3 -std=c++17 -pthread -DCGAL_DO_NOT_USE_BOOST_MP -DCGAL_EIGEN3_ENABLED

EIGEN_DIR = /usr/include/eigen3
CGAL_VER := 6.0.3
CGAL_DIR := $(CURDIR)/CGAL-$(CGAL_VER)/include
BOOST_DIR := $(CURDIR)/boost_1_83_0

INCLUDES := -I$(CGAL_DIR) -I$(BOOST_DIR) -I$(EIGEN_DIR)

.PHONY: all dependencies clean

all: compute-bounding-box.exe

compute-bounding-box.exe: bounding-onion.cpp
	$(CXX) $(CFLAGS) $(INCLUDES) $< -o $@

CGAL-$(CGAL_VER).tar.xz:
	curl -OL https://github.com/CGAL/cgal/releases/download/v$(CGAL_VER)/CGAL-$(CGAL_VER).tar.xz

boost_1_83_0.tar.bz2:
	curl -OL https://archives.boost.io/release/1.83.0/source/boost_1_83_0.tar.bz2

dependencies: CGAL-$(CGAL_VER).tar.xz boost_1_83_0.tar.bz2
	-rm -r CGAL-$(CGAL_VER) boost_1_83_0
	tar -xf CGAL-$(CGAL_VER).tar.xz
	tar -xf boost_1_83_0.tar.bz2

clean:
	-rm compute-bounding-box compute-bounding-box.exe
