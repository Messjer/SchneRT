login:
	ssh -p 3522 huzy@166.111.139.44
down:
	scp -P 3522 huzy@166.111.139.44:/home/huzy/SchneRT/result/try.png .
run:
	cd result
	./SchneRT
build:
	cd result; cmake -DCMAKE_BUILD_TYPE=Release ..; make
