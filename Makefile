all: mbrot heat

mbrot:
	g++ mandelbrot/mandelbrot_parallel.cpp -O2 -std=c++20 -lpthread -o mbrot

heat:
	mpic++ heat_equation/heat_equation_parallel.cpp -O2 -lm -o heat

clean:
	rm mbrot
	rm heat
