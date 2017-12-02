#
#	"_CPU_" Generator
#
def convert(x):
	x = x.replace("!","_PLING").replace("@","_AT_").replace("+","_PLUS_")
	x = x.replace("/","_SLASH_").replace(">","_GREATER_").replace(";","_SEMICOLON_")
	x = x.replace("=","_EQUALS_").replace("[","_LQ_").replace("]","_RQ_")
	x = x.replace("-","_CMINUS_").replace("<","_LESS_").replace("?","_QUESTION_")
	x = x.replace("*","_STAR_").replace(".","_DOT_")
	x = "KWD_"+x
	while x.find("__") >= 0:
		x = x.replace("__","_")
	return x[:-1] if x[-1] == "_" else x

keyWords = []
keyWordCode = {}
keyWordIdentifier = {}

def loadSource(fileName):
	src = [x.replace("\t"," ") for x in open(fileName).readlines()]
	src = [x if x.find("//") < 0 else x[:x.find("//")] for x in src]
	src = [x.strip() for x in src]
	src = [x.strip() for x in (" ".join(src)).split(".define") if x.strip() != ""]


	for l in src:
		p = l.find(" ")
		word = l[:p].strip().lower()
		code = l[p:].strip()+";break;".replace(";;",";")
		keyWords.append(word)
		keyWordCode[word] = code 
		keyWordIdentifier[word] = convert(word.upper())

loadSource("corewords.def")
#loadSource("group1.def")

loadAddress = 0x6000
coreAddress = 0x8000

h = open("__vmcase.h","w")
for w in keyWords:
	h.write("case {0}:/*** {2} ***/\n    {1}\n".format(keyWordIdentifier[w],keyWordCode[w],w))
h.close()

h = open("__vminclude.h","w")
h.write("#ifndef __VMI\n#define __VMI\n\n")
for i in range(0,len(keyWords)):
	h.write("#define {0} (0x{1:04x})\n".format(keyWordIdentifier[keyWords[i]],i))	
h.write("#define KWD_COUNT ({0})\n\n".format(len(keyWords)))
h.write("#define SYS_ADDR_LOAD (0x{0:04x})\n".format(loadAddress))
h.write("\n#ifdef MNEMONICS\n")
h.write("static const char *_mnemonics[] = { \n")
h.write(",".join(['"'+x+'"' for x in keyWords]))
h.write("\n};\n#endif\n")
h.write("#endif\n")
h.close()

wordList = ",".join(['"'+x+'"' for x in keyWords])
h = open("compilerinfo.py","w")
h.write("#\n# Generated by generate.py \n#\n")
h.write("class CompilerInfo:\n")
h.write("  def getWordList(self):\n")
h.write("    return ["+wordList+"]\n")
h.write("  def getCoreAddress(self):\n")
h.write("    return 0x{0:04x}\n".format(coreAddress))
h.write("  def getLoadAddress(self):\n")
h.write("    return 0x{0:04x}\n".format(loadAddress))
h.write("  def getBinarySize(self):\n")
h.write("    return 0x{0:04x}\n".format(0))
