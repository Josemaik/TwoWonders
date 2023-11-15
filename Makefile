APP		  := Prueba
CC        := g++
CCFLAGS   := -std=c++23 -Wall -Wpedantic -Wextra -Wconversion 
LIBS      := -lraylib

MKDIR     := mkdir -p
SRC  	  := src
OBJ  	  := obj

ALLCPP       := $(shell find $(SRC) -type f -iname *.cpp)
ALLCPPOBJ    := $(patsubst %.cpp,%.o,$(ALLCPP))
SUBDIRS      := $(shell find $(SRC) -type d)
OBJSUBDIRS   := $(patsubst $(SRC)%,$(OBJ)%,$(SUBDIRS))

$(APP) : $(OBJSUBDIRS) $(ALLCPPOBJ)
	$(CC) -o $(APP) $(patsubst $(SRC)%,$(OBJ)%,$(ALLCPPOBJ)) $(LIBS)

%.o : %.cpp
	$(CC) -o $(patsubst $(SRC)%,$(OBJ)%,$@) -c $^ $(CCFLAGS)

# TODO --> que cree solo lo que tenga que crear y no todo
$(OBJSUBDIRS) :
	$(MKDIR) $(OBJSUBDIRS)
	
.PHONY : dir clean game

game: $(OBJSUBDIRS) $(ALLCPPOBJ)
	$(CC) -o $(APP) $(patsubst $(SRC)%,$(OBJ)%,$(ALLCPPOBJ)) $(LIBS) && ./$(APP)

dir :
	$(info $(SUBDIRS))
	$(info $(OBJSUBDIRS))
	$(info $(ALLCPP))
	$(info $(ALLCPPOBJ))

clean:
	rm -r ./$(OBJ)/ ./$(APP)