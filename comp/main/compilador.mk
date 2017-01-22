##
## Auto Generated makefile by CodeLite IDE
## any manual changes will be erased      
##
## Debug
ProjectName            :=compilador
ConfigurationName      :=Debug
WorkspacePath          :=/home/rafaellb/Documents/main
ProjectPath            :=/home/rafaellb/Desktop/bosta/compilador
IntermediateDirectory  :=./Debug
OutDir                 := $(IntermediateDirectory)
CurrentFileName        :=
CurrentFilePath        :=
CurrentFileFullPath    :=
User                   :=rafaellb
Date                   :=22/01/17
CodeLitePath           :=/home/rafaellb/.codelite
LinkerName             :=gcc
SharedObjectLinkerName :=gcc -shared -fPIC
ObjectSuffix           :=.o
DependSuffix           :=.o.d
PreprocessSuffix       :=.o.i
DebugSwitch            :=-g 
IncludeSwitch          :=-I
LibrarySwitch          :=-l
OutputSwitch           :=-o 
LibraryPathSwitch      :=-L
PreprocessorSwitch     :=-D
SourceSwitch           :=-c 
OutputFile             :=$(IntermediateDirectory)/$(ProjectName)
Preprocessors          :=
ObjectSwitch           :=-o 
ArchiveOutputSwitch    := 
PreprocessOnlySwitch   :=-E 
ObjectsFileList        :="compilador.txt"
PCHCompileFlags        :=
MakeDirCommand         :=mkdir -p
LinkOptions            :=  
IncludePath            :=  $(IncludeSwitch). $(IncludeSwitch). 
IncludePCH             := 
RcIncludePath          := 
Libs                   := 
ArLibs                 :=  
LibPath                := $(LibraryPathSwitch). 

##
## Common variables
## AR, CXX, CC, AS, CXXFLAGS and CFLAGS can be overriden using an environment variables
##
AR       := ar rcus
CXX      := gcc
CC       := gcc
CXXFLAGS :=  -g -O0 -Wall $(Preprocessors)
CFLAGS   :=  -g -O0 -Wall $(Preprocessors)
ASFLAGS  := 
AS       := as


##
## User defined environment variables
##
CodeLiteDir:=/usr/share/codelite
Objects0=$(IntermediateDirectory)/up_lexer.c$(ObjectSuffix) $(IntermediateDirectory)/up_tokenize.c$(ObjectSuffix) $(IntermediateDirectory)/up_check.c$(ObjectSuffix) $(IntermediateDirectory)/up_helper.c$(ObjectSuffix) $(IntermediateDirectory)/up_rexxCom.c$(ObjectSuffix) $(IntermediateDirectory)/up_generate.c$(ObjectSuffix) $(IntermediateDirectory)/up_ast.c$(ObjectSuffix) $(IntermediateDirectory)/up_main.c$(ObjectSuffix) $(IntermediateDirectory)/up_grammar.c$(ObjectSuffix) 



Objects=$(Objects0) 

##
## Main Build Targets 
##
.PHONY: all clean PreBuild PrePreBuild PostBuild MakeIntermediateDirs
all: $(OutputFile)

$(OutputFile): $(IntermediateDirectory)/.d $(Objects) 
	@$(MakeDirCommand) $(@D)
	@echo "" > $(IntermediateDirectory)/.d
	@echo $(Objects0)  > $(ObjectsFileList)
	$(LinkerName) $(OutputSwitch)$(OutputFile) @$(ObjectsFileList) $(LibPath) $(Libs) $(LinkOptions)

MakeIntermediateDirs:
	@test -d ./Debug || $(MakeDirCommand) ./Debug


$(IntermediateDirectory)/.d:
	@test -d ./Debug || $(MakeDirCommand) ./Debug

PreBuild:


##
## Objects
##
$(IntermediateDirectory)/up_lexer.c$(ObjectSuffix): ../lexer.c $(IntermediateDirectory)/up_lexer.c$(DependSuffix)
	$(CC) $(SourceSwitch) "/home/rafaellb/Desktop/bosta/lexer.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_lexer.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_lexer.c$(DependSuffix): ../lexer.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_lexer.c$(ObjectSuffix) -MF$(IntermediateDirectory)/up_lexer.c$(DependSuffix) -MM ../lexer.c

$(IntermediateDirectory)/up_lexer.c$(PreprocessSuffix): ../lexer.c
	$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_lexer.c$(PreprocessSuffix) ../lexer.c

$(IntermediateDirectory)/up_tokenize.c$(ObjectSuffix): ../tokenize.c $(IntermediateDirectory)/up_tokenize.c$(DependSuffix)
	$(CC) $(SourceSwitch) "/home/rafaellb/Desktop/bosta/tokenize.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_tokenize.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_tokenize.c$(DependSuffix): ../tokenize.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_tokenize.c$(ObjectSuffix) -MF$(IntermediateDirectory)/up_tokenize.c$(DependSuffix) -MM ../tokenize.c

$(IntermediateDirectory)/up_tokenize.c$(PreprocessSuffix): ../tokenize.c
	$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_tokenize.c$(PreprocessSuffix) ../tokenize.c

$(IntermediateDirectory)/up_check.c$(ObjectSuffix): ../check.c $(IntermediateDirectory)/up_check.c$(DependSuffix)
	$(CC) $(SourceSwitch) "/home/rafaellb/Desktop/bosta/check.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_check.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_check.c$(DependSuffix): ../check.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_check.c$(ObjectSuffix) -MF$(IntermediateDirectory)/up_check.c$(DependSuffix) -MM ../check.c

$(IntermediateDirectory)/up_check.c$(PreprocessSuffix): ../check.c
	$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_check.c$(PreprocessSuffix) ../check.c

$(IntermediateDirectory)/up_helper.c$(ObjectSuffix): ../helper.c $(IntermediateDirectory)/up_helper.c$(DependSuffix)
	$(CC) $(SourceSwitch) "/home/rafaellb/Desktop/bosta/helper.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_helper.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_helper.c$(DependSuffix): ../helper.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_helper.c$(ObjectSuffix) -MF$(IntermediateDirectory)/up_helper.c$(DependSuffix) -MM ../helper.c

$(IntermediateDirectory)/up_helper.c$(PreprocessSuffix): ../helper.c
	$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_helper.c$(PreprocessSuffix) ../helper.c

$(IntermediateDirectory)/up_rexxCom.c$(ObjectSuffix): ../rexxCom.c $(IntermediateDirectory)/up_rexxCom.c$(DependSuffix)
	$(CC) $(SourceSwitch) "/home/rafaellb/Desktop/bosta/rexxCom.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_rexxCom.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_rexxCom.c$(DependSuffix): ../rexxCom.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_rexxCom.c$(ObjectSuffix) -MF$(IntermediateDirectory)/up_rexxCom.c$(DependSuffix) -MM ../rexxCom.c

$(IntermediateDirectory)/up_rexxCom.c$(PreprocessSuffix): ../rexxCom.c
	$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_rexxCom.c$(PreprocessSuffix) ../rexxCom.c

$(IntermediateDirectory)/up_generate.c$(ObjectSuffix): ../generate.c $(IntermediateDirectory)/up_generate.c$(DependSuffix)
	$(CC) $(SourceSwitch) "/home/rafaellb/Desktop/bosta/generate.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_generate.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_generate.c$(DependSuffix): ../generate.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_generate.c$(ObjectSuffix) -MF$(IntermediateDirectory)/up_generate.c$(DependSuffix) -MM ../generate.c

$(IntermediateDirectory)/up_generate.c$(PreprocessSuffix): ../generate.c
	$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_generate.c$(PreprocessSuffix) ../generate.c

$(IntermediateDirectory)/up_ast.c$(ObjectSuffix): ../ast.c $(IntermediateDirectory)/up_ast.c$(DependSuffix)
	$(CC) $(SourceSwitch) "/home/rafaellb/Desktop/bosta/ast.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_ast.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_ast.c$(DependSuffix): ../ast.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_ast.c$(ObjectSuffix) -MF$(IntermediateDirectory)/up_ast.c$(DependSuffix) -MM ../ast.c

$(IntermediateDirectory)/up_ast.c$(PreprocessSuffix): ../ast.c
	$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_ast.c$(PreprocessSuffix) ../ast.c

$(IntermediateDirectory)/up_main.c$(ObjectSuffix): ../main.c $(IntermediateDirectory)/up_main.c$(DependSuffix)
	$(CC) $(SourceSwitch) "/home/rafaellb/Desktop/bosta/main.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_main.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_main.c$(DependSuffix): ../main.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_main.c$(ObjectSuffix) -MF$(IntermediateDirectory)/up_main.c$(DependSuffix) -MM ../main.c

$(IntermediateDirectory)/up_main.c$(PreprocessSuffix): ../main.c
	$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_main.c$(PreprocessSuffix) ../main.c

$(IntermediateDirectory)/up_grammar.c$(ObjectSuffix): ../grammar.c $(IntermediateDirectory)/up_grammar.c$(DependSuffix)
	$(CC) $(SourceSwitch) "/home/rafaellb/Desktop/bosta/grammar.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_grammar.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_grammar.c$(DependSuffix): ../grammar.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_grammar.c$(ObjectSuffix) -MF$(IntermediateDirectory)/up_grammar.c$(DependSuffix) -MM ../grammar.c

$(IntermediateDirectory)/up_grammar.c$(PreprocessSuffix): ../grammar.c
	$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_grammar.c$(PreprocessSuffix) ../grammar.c


-include $(IntermediateDirectory)/*$(DependSuffix)
##
## Clean
##
clean:
	$(RM) -r ./Debug/


