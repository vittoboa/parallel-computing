all: mbrot.x heat.x

mbrot.x:
	g++ mandelbrot/mandelbrot_parallel.cpp -O2 -std=c++20 -lpthread -o mbrot.x

heat.x:
	mpic++ heat_equation/heat_equation_parallel.cpp -O2 -lm -o heat.x

clean:
	rm mbrot.x
	rm heat.x
