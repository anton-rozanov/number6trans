test:
	git submodule update --init --recursive
	mkdir -p build
	cd build && cmake ..
	cd build && make 
	cd build && make test 
clean:
	rm -r build
	rm -r vcpkg
	rm -r .cache
