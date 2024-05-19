APP		   	:= TwoWonders
CCFLAGS    	:= -std=c++2b -Wall -Wpedantic -Wextra -Wconversion -Isrc/ -I/usr/include/freetype2 -I/mingw64/include/freetype2

ifeq ($(OS),Windows_NT)
	CC 	   		:= g++
	CCACHE 	   	:= ccache
    LIBS 	   	:= -L./ libs/fmod.dll libs/fmodstudio.dll -lwinmm -lgdi32 -lglfw3 -lopengl32 -lglew32 -lglu32 -lm -lassimp -lfreetype -lgif
	SANITIZE   	:=
	LIBS_COPY  	:= libs/*.dll
else
	CC 		   	:= g++-12
	CCACHE 	   	:= ccache
    LIBS 		:= -L./fmodlibs -lfmod -lfmodstudio -lglfw -lGLEW -lGLU -lGL -lm -lassimp -lfreetype -lgif
	SANITIZE   	:= -fsanitize=address,undefined
	LIBS_COPY  	:= fmodlibs/libfmod.so.13 fmodlibs/libfmodstudio.so.13 libs/*.so* 
endif

# agregar g++ | clang++

MKDIR      	:= mkdir -p
SRC  	   	:= src
OBJ  	   	:= obj
RELEASE    	:= release
ASSETS     	:= assets
LIBS_DIR   	:= libs

ALLCPP     	:= $(shell find $(SRC) -type f -iname *.cpp)
ALLCPPOBJ  	:= $(patsubst %.cpp,%.o,$(ALLCPP))
SUBDIRS    	:= $(shell find $(SRC) -type d)
OBJSUBDIRS 	:= $(patsubst $(SRC)%,$(OBJ)%,$(SUBDIRS))

DATE       	:= $(shell date +'%d-%m-%y')
ZIP_NAME   	:= $(APP)_$(RELEASE)_$(DATE).zip


# Variables si es make release
ifeq ($(filter release,$(MAKECMDGOALS)),release)
    SANITIZE :=
    CCFLAGS  += -O3 -DNDEBUG
else
    CCFLAGS  += -g
endif

# Regla principal (enlazado de los .o)
$(APP) : $(OBJSUBDIRS) $(ALLCPPOBJ)
	$(CCACHE) $(CC) -o $(APP) $(patsubst $(SRC)%,$(OBJ)%,$(ALLCPPOBJ)) $(LIBS) $(SANITIZE) -Wl,-rpath=libs -Wl,-rpath,./fmodlibs
ifeq ($(OS),Windows_NT)
	mv $(APP) ./libs/
endif

# Regla que compila los .cpp
%.o : %.cpp
	$(CCACHE) $(CC) -o $(patsubst $(SRC)%,$(OBJ)%,$@) -c $^ $(CCFLAGS) $(SANITIZE)

# Regla que crea una release
$(RELEASE) : $(APP) $(ASSETS)
	$(MKDIR) $(RELEASE)
	cp -r $(ASSETS) $(RELEASE)/
ifeq ($(OS),Windows_NT)
	cp $(LIBS_COPY) $(RELEASE)/
	mv $(LIBS_DIR)/$(APP) $(RELEASE)/
else
	$(MKDIR) $(RELEASE)/$(LIBS_DIR)
	cp $(APP) $(RELEASE)/
	cp $(LIBS_COPY) $(RELEASE)/$(LIBS_DIR)
endif
	zip -r $(ZIP_NAME) $(RELEASE)/

# Reglas auxiliares que crean carpetas

$(OBJSUBDIRS) :
	$(MKDIR) $(OBJSUBDIRS)

$(ASSETS):
	$(MKDIR) $(ASSETS)

# Otras reglas

.PHONY : dir clean game

game: $(APP)
ifeq ($(OS),Windows_NT)
	./$(LIBS_DIR)/$(APP)
else
	./$(APP)
endif

dir:
	$(info $(ASSETS))
	$(info $(SUBDIRS))
	$(info $(OBJSUBDIRS))
	$(info $(ALLCPP))
	$(info $(ALLCPPOBJ))

clean:
ifeq ($(OS),Windows_NT)
	rm -r ./$(OBJ)/ ./$(LIBS_DIR)/$(APP) ./$(RELEASE)/ ./$(ZIP_NAME) 
else
	rm -r ./$(OBJ)/ ./$(APP) ./$(RELEASE)/ ./$(ZIP_NAME)
endif