objects = cyrus-beck.o
target = cyrus-beck

all: $(target)

$(target):$(objects)
	gcc -o $(target) $(objects) -lglut -lGL -lGLU -lm
$(objects): %.o : %.c
	gcc -c $< -o $@

.PHONY : all
.PHONY : clean

clean:
	rm -rf *.o
	rm -rf $(target)
