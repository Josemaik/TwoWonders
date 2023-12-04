APP		  := ZeldaWonders
CC        :=  g++
CCFLAGS   := -std=c++23 -Wall -Wpedantic -Wextra -Wconversion -Isrc/
LIBS      := -lraylib
SANITIZE  := -fsanitize=address,undefined

MKDIR     := mkdir -p
SRC  	  := src
OBJ  	  := obj

ALLCPP       := $(shell find $(SRC) -type f -iname *.cpp)
ALLCPPOBJ    := $(patsubst %.cpp,%.o,$(ALLCPP))
SUBDIRS      := $(shell find $(SRC) -type d)
OBJSUBDIRS   := $(patsubst $(SRC)%,$(OBJ)%,$(SUBDIRS))

$(APP) : $(OBJSUBDIRS) $(ALLCPPOBJ)
	$(CC) -o $(APP) $(patsubst $(SRC)%,$(OBJ)%,$(ALLCPPOBJ)) $(LIBS) $(SANITIZE)

%.o : %.cpp
	$(CC) -o $(patsubst $(SRC)%,$(OBJ)%,$@) -c $^ $(CCFLAGS) -g $(SANITIZE)

# TODO --> que cree solo lo que tenga que crear y no todo
$(OBJSUBDIRS) :
	$(MKDIR) $(OBJSUBDIRS)
	
.PHONY : dir clean game

game: $(OBJSUBDIRS) $(ALLCPPOBJ)
	$(CC) -o $(APP) $(patsubst $(SRC)%,$(OBJ)%,$(ALLCPPOBJ)) $(SANITIZE) $(LIBS) && ./$(APP)

dir :
	$(info $(SUBDIRS))
	$(info $(OBJSUBDIRS))
	$(info $(ALLCPP))
	$(info $(ALLCPPOBJ))

clean:
	rm -r ./$(OBJ)/ ./$(APP)