##
## Auto Generated makefile by CodeLite IDE
## any manual changes will be erased      
##
## Debug
ProjectName            :=Compilador
ConfigurationName      :=Debug
WorkspacePath          :=/home/rafaellb/Documents/main
ProjectPath            :=/home/rafaellb/Desktop/comp-cobol/comp
IntermediateDirectory  :=./Debug
OutDir                 := $(IntermediateDirectory)
CurrentFileName        :=
CurrentFilePath        :=
CurrentFileFullPath    :=
User                   :=rafaellb
Date                   :=20/01/17
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
ObjectsFileList        :="Compilador.txt"
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
Objects0=$(IntermediateDirectory)/Source_ast.c$(ObjectSuffix) $(IntermediateDirectory)/Source_check.c$(ObjectSuffix) $(IntermediateDirectory)/Source_generate.c$(ObjectSuffix) $(IntermediateDirectory)/Source_grammar.c$(ObjectSuffix) $(IntermediateDirectory)/Source_helper.c$(ObjectSuffix) $(IntermediateDirectory)/Source_lexer.c$(ObjectSuffix) $(IntermediateDirectory)/Source_rexxCom.c$(ObjectSuffix) $(IntermediateDirectory)/Source_tokenize.c$(ObjectSuffix) $(IntermediateDirectory)/main.c$(ObjectSuffix) 



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
$(IntermediateDirectory)/Source_ast.c$(ObjectSuffix): Source/ast.c $(IntermediateDirectory)/Source_ast.c$(DependSuffix)
	$(CC) $(SourceSwitch) "/home/rafaellb/Desktop/comp-cobol/comp/Source/ast.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/Source_ast.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/Source_ast.c$(DependSuffix): Source/ast.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/Source_ast.c$(ObjectSuffix) -MF$(IntermediateDirectory)/Source_ast.c$(DependSuffix) -MM Source/ast.c

$(IntermediateDirectory)/Source_ast.c$(PreprocessSuffix): Source/ast.c
	$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/Source_ast.c$(PreprocessSuffix) Source/ast.c

$(IntermediateDirectory)/Source_check.c$(ObjectSuffix): Source/check.c $(IntermediateDirectory)/Source_check.c$(DependSuffix)
	$(CC) $(SourceSwitch) "/home/rafaellb/Desktop/comp-cobol/comp/Source/check.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/Source_check.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/Source_check.c$(DependSuffix): Source/check.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/Source_check.c$(ObjectSuffix) -MF$(IntermediateDirectory)/Source_check.c$(DependSuffix) -MM Source/check.c

$(IntermediateDirectory)/Source_check.c$(PreprocessSuffix): Source/check.c
	$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/Source_check.c$(PreprocessSuffix) Source/check.c

$(IntermediateDirectory)/Source_generate.c$(ObjectSuffix): Source/generate.c $(IntermediateDirectory)/Source_generate.c$(DependSuffix)
	$(CC) $(SourceSwitch) "/home/rafaellb/Desktop/comp-cobol/comp/Source/generate.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/Source_generate.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/Source_generate.c$(DependSuffix): Source/generate.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/Source_generate.c$(ObjectSuffix) -MF$(IntermediateDirectory)/Source_generate.c$(DependSuffix) -MM Source/generate.c

$(IntermediateDirectory)/Source_generate.c$(PreprocessSuffix): Source/generate.c
	$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/Source_generate.c$(PreprocessSuffix) Source/generate.c

$(IntermediateDirectory)/Source_grammar.c$(ObjectSuffix): Source/grammar.c $(IntermediateDirectory)/Source_grammar.c$(DependSuffix)
	$(CC) $(SourceSwitch) "/home/rafaellb/Desktop/comp-cobol/comp/Source/grammar.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/Source_grammar.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/Source_grammar.c$(DependSuffix): Source/grammar.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/Source_grammar.c$(ObjectSuffix) -MF$(IntermediateDirectory)/Source_grammar.c$(DependSuffix) -MM Source/grammar.c

$(IntermediateDirectory)/Source_grammar.c$(PreprocessSuffix): Source/grammar.c
	$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/Source_grammar.c$(PreprocessSuffix) Source/grammar.c

$(IntermediateDirectory)/Source_helper.c$(ObjectSuffix): Source/helper.c $(IntermediateDirectory)/Source_helper.c$(DependSuffix)
	$(CC) $(SourceSwitch) "/home/rafaellb/Desktop/comp-cobol/comp/Source/helper.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/Source_helper.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/Source_helper.c$(DependSuffix): Source/helper.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/Source_helper.c$(ObjectSuffix) -MF$(IntermediateDirectory)/Source_helper.c$(DependSuffix) -MM Source/helper.c

$(IntermediateDirectory)/Source_helper.c$(PreprocessSuffix): Source/helper.c
	$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/Source_helper.c$(PreprocessSuffix) Source/helper.c

$(IntermediateDirectory)/Source_lexer.c$(ObjectSuffix): Source/lexer.c $(IntermediateDirectory)/Source_lexer.c$(DependSuffix)
	$(CC) $(SourceSwitch) "/home/rafaellb/Desktop/comp-cobol/comp/Source/lexer.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/Source_lexer.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/Source_lexer.c$(DependSuffix): Source/lexer.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/Source_lexer.c$(ObjectSuffix) -MF$(IntermediateDirectory)/Source_lexer.c$(DependSuffix) -MM Source/lexer.c

$(IntermediateDirectory)/Source_lexer.c$(PreprocessSuffix): Source/lexer.c
	$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/Source_lexer.c$(PreprocessSuffix) Source/lexer.c

$(IntermediateDirectory)/Source_rexxCom.c$(ObjectSuffix): Source/rexxCom.c $(IntermediateDirectory)/Source_rexxCom.c$(DependSuffix)
	$(CC) $(SourceSwitch) "/home/rafaellb/Desktop/comp-cobol/comp/Source/rexxCom.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/Source_rexxCom.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/Source_rexxCom.c$(DependSuffix): Source/rexxCom.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/Source_rexxCom.c$(ObjectSuffix) -MF$(IntermediateDirectory)/Source_rexxCom.c$(DependSuffix) -MM Source/rexxCom.c

$(IntermediateDirectory)/Source_rexxCom.c$(PreprocessSuffix): Source/rexxCom.c
	$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/Source_rexxCom.c$(PreprocessSuffix) Source/rexxCom.c

$(IntermediateDirectory)/Source_tokenize.c$(ObjectSuffix): Source/tokenize.c $(IntermediateDirectory)/Source_tokenize.c$(DependSuffix)
	$(CC) $(SourceSwitch) "/home/rafaellb/Desktop/comp-cobol/comp/Source/tokenize.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/Source_tokenize.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/Source_tokenize.c$(DependSuffix): Source/tokenize.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/Source_tokenize.c$(ObjectSuffix) -MF$(IntermediateDirectory)/Source_tokenize.c$(DependSuffix) -MM Source/tokenize.c

$(IntermediateDirectory)/Source_tokenize.c$(PreprocessSuffix): Source/tokenize.c
	$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/Source_tokenize.c$(PreprocessSuffix) Source/tokenize.c

$(IntermediateDirectory)/main.c$(ObjectSuffix): main.c $(IntermediateDirectory)/main.c$(DependSuffix)
	$(CC) $(SourceSwitch) "/home/rafaellb/Desktop/comp-cobol/comp/main.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/main.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/main.c$(DependSuffix): main.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/main.c$(ObjectSuffix) -MF$(IntermediateDirectory)/main.c$(DependSuffix) -MM main.c

$(IntermediateDirectory)/main.c$(PreprocessSuffix): main.c
	$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/main.c$(PreprocessSuffix) main.c


-include $(IntermediateDirectory)/*$(DependSuffix)
##
## Clean
##
clean:
	$(RM) -r ./Debug/


