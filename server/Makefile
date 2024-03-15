INCLUDE = C:\msys64\mingw64\include
LDFLAGS = -lmingw32 -lSDL2main -lSDL2 -lSDL2_image -lSDL2_ttf -lSDL2_mixer -lSDL2_net -lm

server: main.o ball.o paddle.o music.o text.o ../lib/include/paddle.h ../lib/include/music.h ../lib/include/text.h ../lib/include/ball.h ../lib/include/data.h
	gcc -o server main.o ball.o paddle.o music.o text.o $(LDFLAGS)

main.o: ./src/main.c
	gcc -c ./src/main.c

ball.o: ../lib/src/ball.c ../lib/include/ball.h
	gcc -c ../lib/src/ball.c

paddle.o: ../lib/src/paddle.c ../lib/include/paddle.h
	gcc -c ../lib/src/paddle.c

music.o: ../lib/src/music.c ../lib/include/music.h
	gcc -c ../lib/src/music.c

text.o: ../lib/src/text.c ../lib/include/text.h
	gcc -c ../lib/src/text.c

clean:
	rm *.o
	rm *.exe
