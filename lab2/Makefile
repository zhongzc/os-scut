task1/full: FORCE
	cd task1 && \
	sed -i 's/FULL = 0/FULL = 1/g' common.h && \
	make && make clean && cd ..

task1/empty: FORCE
	cd task1 && \
	sed -i 's/FULL = 1/FULL = 0/g' common.h && \
	make && make clean && cd ..

task2/busy: FORCE
	cd task2 && \
	sed -i 's/FREE = 1/FREE = 0/g' common.c && \
	make && make clean && cd ../..

task2/free: FORCE
	cd task2 && \
	sed -i 's/FREE = 0/FREE = 1/g' common.c && \
	make && make clean && cd ../..

task3: FORCE
	cd task3 && make && make clean && cd ..

FORCE: