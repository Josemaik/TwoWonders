APP		   := ZeldaWonders
CC         := ccache g++
CCFLAGS    := -std=c++23 -Wall -Wpedantic -Wextra -Wconversion -Isrc/ -Ilibs/
LIBS       := libs/libraylib.a
SANITIZE   := -fsanitize=address,undefined

MKDIR      := mkdir -p
SRC  	   := src
OBJ  	   := obj
RELEASE    := release
ASSETS     := assets
#LIBS_DIR   := libs
#LIBS_COPY  := /usr/lib/libraylib.so.450 /usr/lib/libasan.so.8 /usr/lib/libstdc++.so.6 /usr/lib/libm.so.6 /usr/lib/libubsan.so.1 /usr/lib/libgcc_s.so.1 /usr/lib/libc.so.6 /usr/lib64/ld-linux-x86-64.so.2

ALLCPP     := $(shell find $(SRC) -type f -iname *.cpp)
ALLCPPOBJ  := $(patsubst %.cpp,%.o,$(ALLCPP))
SUBDIRS    := $(shell find $(SRC) -type d)
OBJSUBDIRS := $(patsubst $(SRC)%,$(OBJ)%,$(SUBDIRS))

DATE       := $(shell date +'%d-%m-%y')
ZIP_NAME   := $(APP)_$(RELEASE)_$(DATE).zip

$(APP) : $(OBJSUBDIRS) $(ALLCPPOBJ)
	$(CC) -o $(APP) $(patsubst $(SRC)%,$(OBJ)%,$(ALLCPPOBJ)) $(LIBS) $(SANITIZE)

# ifdef release
# 	SANITIZE :=
# 	CCFLAGS  += -O3
#   -DNDEBUG (al compilador)
# else
# 	CCFLAGS  += -g
# endif

%.o : %.cpp
	$(CC) -o $(patsubst $(SRC)%,$(OBJ)%,$@) -c $^ $(CCFLAGS) -g $(SANITIZE)

$(RELEASE) : $(APP) $(ASSETS)
	$(MKDIR) $(RELEASE)
	cp $(APP) $(RELEASE)/
	cp -r $(ASSETS) $(RELEASE)/
	zip -r $(ZIP_NAME) $(RELEASE)/
	make clean
#	$(MKDIR) $(RELEASE)/$(LIBS_DIR)
#	cp $(LIBS_COPY) $(RELEASE)/$(LIBS_DIR)

$(OBJSUBDIRS) :
	$(MKDIR) $(OBJSUBDIRS)

$(ASSETS): 
	$(MKDIR) $(ASSETS)
	
.PHONY : dir clean game

game: $(APP)
	./$(APP)

dir:
	$(info $(ASSETS))
	$(info $(SUBDIRS))
	$(info $(OBJSUBDIRS))
	$(info $(ALLCPP))
	$(info $(ALLCPPOBJ))

clean:
	rm -r ./$(OBJ)/ ./$(APP) ./$(RELEASE)/