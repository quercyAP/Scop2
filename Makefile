SRCS			= lib/Vec3.cpp \
				 lib/Mat4.cpp \
				 main.cpp \
				 core/Texture.cpp \
				 core/MaterialManager.cpp \
				 core/Transform.cpp \
				 core/Camera.cpp \
				 core/Application.cpp \
				 core/Mesh.cpp \
				 core/ObjLoader.cpp \
				 core/ShaderProgram.cpp \
				
SRC_DIR			=	./src/
SOURCES			=	$(addprefix $(SRC_DIR), $(SRCS))

OBJ_DIR			=	./obj/
OBJ_DIR_DEBUG	=	./obj_debug/
OBJ_FILE 		= 	$(patsubst %.cpp, %.o, $(SRCS))
OBJ_FILE 		:= 	$(patsubst %.tpp, %.o, $(OBJ_FILE))
OBJ_FILE 		:= 	$(patsubst %.ipp, %.o, $(OBJ_FILE))
OBJECTS			=	$(addprefix $(OBJ_DIR), $(OBJ_FILE))
OBJECTS_DEBUG	=	$(addprefix $(OBJ_DIR_DEBUG), $(OBJ_FILE))

NAME			=	scop
CC				=	c++

HDR_FLAGS		= -I includes \
				 -I includes/lib \
				 -I includes/utils \
				 -I includes/core \
				
LIB				= -lglfw -lGLEW -lGL		
LIB_DIR			=	

all						:	$(NAME)

$(NAME)					:	obj $(OBJ_DIR) $(OBJECTS)
							@$(CC) $(CCFLAG) $(OBJECTS) $(HDR_FLAGS) $(LIB_DIR) $(LIB) -o $(NAME)
							@echo "\033[0;92m\n* $(NAME) program file was created *\033[0m"

$(OBJ_DIR)%.o 			: 	$(SRC_DIR)%.cpp
							@$(CC) $(CCFLAG) $(HDR_FLAGS)-c $< -o $@
							@echo "\033[0;96m* $(notdir $<) -> $(notdir $@) * \033[0m: \033[0;92mOK\033[0m"

$(OBJ_DIR_DEBUG)%.o		: 	$(SRC_DIR)%.cpp
							@$(CC) -g $(CCFLAG) $(HDR_FLAGS) -c $< -o $@
							@echo "\033[0;96m* $(notdir $<) -> $(notdir $@) * \033[0m: \033[0;93mOK (debug)\033[0m"


clean					:
							@rm -rf $(OBJ_DIR) && rm -rf $(OBJ_DIR_DEBUG)
							@echo "\033[0;91m* $(NAME) object files were deleted *\033[0m"

fclean					:	clean 
							@rm -rf $(NAME)
							@echo "\033[0;91m* $(NAME) was deleted *\033[0m"

re						:	fclean $(NAME)

sanitize				:	fclean obj_debug $(OBJ_DIR_DEBUG) $(OBJECTS_DEBUG)
							@$(CC) -fsanitize=address -g3 $(CCFLAG) $(OBJECTS_DEBUG) $(LIB_DIR) $(LIB) -o $(NAME)
							@echo "\033[0;93m\n* $(NAME) program file was created in sanitize mode*\033[0m"

debug					:	fclean obj_debug $(OBJ_DIR_DEBUG) $(OBJECTS_DEBUG)
							@$(CC) -g $(CCFLAG) $(OBJECTS_DEBUG) $(LIB_DIR) $(LIB) -o $(NAME)
							@echo "\033[0;93m\n* $(NAME) program file was created in debug mode*\033[0m"


add						:
							@script="tools/update_srcs.sh"; \
							while [ ! -f $$script ] && [ ! $$PWD = "/" ]; do cd ..; done; \
							if [ -f $$script ]; then \
								./$$script "$(CURDIR)"; \
								echo "SRCS File added successfully."; \
							else \
								echo "update_srcs.sh not found in parent directories"; \
							fi

create					:
							@echo "Enter the names of your repertory and files (separated by spaces):"
							@read args; \
							./tools/new_c_h_creator.sh $$args; \
							make add

obj						:
							@mkdir -p obj
							@dirs=$$(find "src/" -type d | sed "s|^src/||"); \
							for dir in $$dirs; do \
								mkdir -p obj/$$dir; \
								echo "\033[0;92m* obj$$dir dir created *\033[0m"; \
							done

obj_debug				:
							@mkdir -p obj_debug
							@dirs=$$(find "src/" -type d | sed "s|^src/||"); \
							for dir in $$dirs; do \
								mkdir -p obj_debug/$$dir; \
								echo "\033[0;92m* obj_debug$$dir dir created *\033[0m"; \
							done



.PHONY					:	all clean fclean re sanitize debug obj obj_debug create add