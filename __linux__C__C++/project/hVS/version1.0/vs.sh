case $1	in
	re )
	make clean
	make
	cd bin
	./lobby
	;;
	clean )
	make clean
	;;
	run )
	cd bin
	./lobby
	;;
	make )
	make
	cd bin
	./lobby
	;;
esac