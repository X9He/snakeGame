# super simple makefile
# call it using 'make NAME=name_of_code_file_without_extension'
# (assumes a .cpp extension)
NAME = snake

#
# Add $(MAC_OPT) to the compile line for Mac OSX.
MAC_OPT = "-I/opt/X11/include"

all:
	@echo "Compiling..."
	g++ -std=c++11 -o $(NAME) $(NAME).cpp -L/opt/X11/lib -lX11 -lstdc++ $(MAC_OPT)

run: all
	@echo "Running..."
	./$(NAME) 

clean:
	rm ./snake
