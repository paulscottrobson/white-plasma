# *******************************************************************************************
# *******************************************************************************************
#	
#									  Forth compiler
#
# *******************************************************************************************
# *******************************************************************************************

from compilerinfo import CompilerInfo
import re,sys 

# *******************************************************************************************
#									Compiler Exception
# *******************************************************************************************

class CompilerException(Exception):
	pass

# *******************************************************************************************
#									   Memory Object
# *******************************************************************************************

class Memory:
	#
	#	Initialise.
	#
	def __init__(self):
		self.memory = [None] * 0x10000	
		self.highest = 0x0000
		self.echo = False
	#
	#	Write a byte direct
	#
	def writeByte(self,address,byte):
		if address < 0x0000 or address > 0xFFFF or byte < 0 or byte > 0xFF:
			raise CompilerException("Bad memory write {0:04x} <- {1:02x}".format(address,byte))
		if self.memory[address] is not None:
			raise CompilerException("Memory overwritten {0:04x}".format(address))
		self.memory[address] = byte
		self.highest = max(self.highest,address)
		if self.echo:
			ch = ((byte & 0x3F)^32) + 32
			ch = ch if (ch < ord('A') or ch > ord('Z')) else ch + 32
			print("{0:04x} : {1:02x} '{2:c}'".format(address,byte,ch))
	#
	#	Write a word 
	#
	def writeWord(self,address,word):
		self.writeByte(address,word & 0xFF)
		self.writeByte(address+1,word >> 8)
	#
	#	Write to file
	#
	def writeBinary(self,fileName):
		loadAddress = CompilerInfo().getLoadAddress()
		bin = [x if x is not None else 0 for x in self.memory[loadAddress:self.highest+2]]
		bin = "".join([chr(x) for x in bin])
		open(fileName,"wb").write(bin)

# *******************************************************************************************
#									Dictionary item
# *******************************************************************************************

class DictionaryItem:
	#
	#	Initialise.
	#
	def __init__(self,name,address,isCoreWord = False):
		self.name = name.strip().lower()
		self.address = address
		self.immediate = False
		self.private = False
		self.coreWord = isCoreWord 
	#
	#	Accessors
	#
	def getName(self):
		return self.name
	def getAddress(self):
		return self.address
	def isImmediate(self):
		return self.immediate
	def isPrivate(self):
		return self.private
	def isCoreWord(self):
		return self.coreWord
	#
	#	Mutators
	#
	def setPrivate(self):
		self.private = True
	def setImmediate(self):
		self.immediate = True

# *******************************************************************************************
#										Dictionary
# *******************************************************************************************

class Dictionary:
	#
	#	Initialise.
	#
	def __init__(self):
		# list of dictionaryitem objects in order
		self.entries = [] 
		# hash from name of dictionary item to entry in table.
		self.findItem = {}
		self.loadCoreWords()
	#
	#	Load in core words
	#
	def loadCoreWords(self):
		cwList = CompilerInfo().getWordList()
		self.coreCount = len(cwList)
		for i in range(0,len(cwList)):
			self.add(DictionaryItem(cwList[i],i,True))
	#
	#	Add a new dictionary entry.
	#
	def add(self,dItem):
		# we don't allow duplication in this compiler. 
		if dItem.getName() in self.findItem:
			raise CompilerException("Word '{0}' has been defined twice".format(dItem.getName()))
		# add to list and lookup
		self.entries.append(dItem)
		self.findItem[dItem.getName()] = dItem 
	#
	#	Get the last dictionary entry.
	#
	def getLastEntry(self):
		return self.entries[-1]
	#
	#	Find an item by name
	#
	def search(self,name):
		name = name.lower().strip()
		return None if name not in self.findItem else self.findItem[name]
	#
	#	Generate the dictionary entries
	#
	def generate(self,memory):
		memory.echo = False
		dictPointer = CompilerInfo().getLoadAddress()+32		# starts here.
		dCount = 0
		for dItem in self.entries:			# for each item 		
			if not dItem.isPrivate():		# don't need to know about private items.
				lastWord = dictPointer
				# address/ref
				if dItem.isCoreWord():
					memory.writeByte(dictPointer+1,dItem.getAddress())
				else:
					memory.writeByte(dictPointer+1,dItem.getAddress() >> 8)
					memory.writeByte(dictPointer+2,dItem.getAddress() & 0xFF)
				# control byte (not paged)
				iByte = (0x80 if dItem.isImmediate() else 0)+(0x40 if dItem.isCoreWord() else 0)
				memory.writeByte(dictPointer+3,iByte)
				# name
				name = dItem.getName()
				for i in range(0,len(name)):
					memory.writeByte(dictPointer+4+i,(ord(name[i].upper()) & 0x3F)+(0x80 if i < len(name)-1 else 0x00))
				dictPointer = dictPointer + 4 + len(dItem.getName())
				# offset to previous
				memory.writeByte(lastWord,dictPointer-lastWord)
				dCount += 1
		# add ending zero.
		memory.writeByte(dictPointer,0)
		memory.writeByte(dictPointer+1,0)
		# core address reference.
		loadAddress = CompilerInfo().getLoadAddress()
		memory.writeWord(loadAddress+2,CompilerInfo().getCoreAddress())
		# dictionary start
		memory.writeWord(loadAddress+4,loadAddress+32)
		# next free dictionary byte working up.
		memory.writeWord(loadAddress+6,dictPointer)
		# put _main in if there.
		mainItem = self.search("_main")
		if mainItem is not None:
			# it'll be in page 0
			memory.writeByte(loadAddress+11,0)
			# start address.
			memory.writeWord(loadAddress+12,mainItem.getAddress())
		print("Dictionary : {0} entries ({1} core) down to ${2:04x}".format(dCount,self.coreCount,dictPointer))
		memory.echo = False

# *******************************************************************************************
#										Compiler
# *******************************************************************************************

class Compiler:
	#
	#	Initialise.
	#
	def __init__(self):
		self.compilerInfo = CompilerInfo()
		# memory storage
		self.memory = Memory()
		# internal dictionary
		self.dictionary = Dictionary()
		# current source file/line number
		self.source = ""
		self.lineNumber = 0
		# compile currently enabled
		self.compileEnabled = True 
		# memory pointer.
		self.pointer = self.compilerInfo.getCoreAddress()+self.compilerInfo.getBinarySize()+1
		# no structures
		self.structureList = "do,loop,if,then,else,begin,until".split(",")
		self.memory.echo = False
		# compile mode
		self.isCompiling = False
		# structure stuff
		self.ifLink = None
	#
	#	Compile a source file.
	#
	def compileFile(self,sourceFile):
		print("Compiling "+sourceFile)
		self.compileText(open(sourceFile).readlines(),sourceFile)
	#
	#	Compile an array of text lines
	#
	def compileText(self,textlines,sourceFile = "(direct entry)"):
		self.source = sourceFile
		self.lineNumber = 1
		for l in textlines:
			self.compileLine(l.strip())
			self.lineNumber += 1
	#
	#	Compile a single line.
	#
	def compileLine(self,line):
		# remove comments
		while line.find("{") >= 0:
			p1 = line.find("{")
			p2 = line.find("}")
			if p2 < p1 or p2 < 0:
				raise CompilerException("Bad comment")
			line = line[:p1]+line[p2+1:]
		# remove tabs, make l/c		
		line = line.replace("\t"," ").lower()
		self.words = [x.strip() for x in line.split(" ") if x.strip() != ""]
		while len(self.words) != 0:
			self.compileWord(self.getWord())
	#
	#	Get a word from the input line.
	#
	def getWord(self):
		if len(self.words) == 0:
			return ""
		w = self.words[0]
		self.words = self.words[1:]
		return w
	#
	#	Compile a single word
	#
	def compileWord(self,word):
		print(">>",word)
		# Check for definition and conditional compilation.
		if word == ':':
			word = self.getWord()
			if word == "":
				raise CompilerException(": without word")
			if self.isCompiling:
				raise CompilerException("Previous word not closed.")
			# if doesn't exist
			if self.dictionary.search(word) is None:
				# enable compiling
				self.compileEnabled = True
				# if in paged memory pad out to align 8
				if self.pointer >= 0xC000:
					while (self.pointer & 7) != 0:
						self.compile("[nop]")
				# add to dictionary
				self.dictionary.add(DictionaryItem(word,self.pointer))
				self.isCompiling = True
			else:
				self.compileEnabled = False
			return

		# Not compiling so exit.
		if not self.compileEnabled:
			if word == ";":
				self.compileEnabled = True
				self.isCompiling = False
			return False

		# Control structures for..next if..then[;]
		if word in self.structureList:
			self.compileStructure(word)
			return

		# Check for dictionary item
		dItem = self.dictionary.search(word)
		if dItem is not None:
			if not self.isCompiling:
				raise CompilerException("Code outside a colon definition")
			# core word or direct reference to another word.
			if dItem.isCoreWord():
				self.compileDataByte(dItem.getAddress())
			else:
				addr = dItem.getAddress();
				self.compileDataByte(addr >> 8)
				self.compileDataByte(addr & 0xFF)
			# ; turns off compilation
			if word == ";":
				self.isCompiling = False
			return		
		# Constants
		if re.match("^\-?\d+$",word) is not None:
			self.compileLiteral(int(word,10))
			return
		if re.match("^\$[0-9a-f]+$",word) is not None:
			self.compileLiteral(int(word[1:],16))
			return
		if word == "[[sysinfo]]":
			self.compileLiteral(self.compilerInfo.getLoadAddress())
			return
		# Modifiers
		if word == "immediate":
			self.dictionary.getLastEntry().setImmediate()
			return
		if word == "private":
			self.dictionary.getLastEntry().setPrivate()
			return
		# Variable
		if word == "variable":
			word = self.getWord()
			if word == "":
				raise CompilerException("variable without word")
			if self.isCompiling:
				raise CompilerException("Previous word not closed.")
			self.dictionary.add(DictionaryItem(word,self.pointer))
			self.isCompiling = True
			self.compileWord("[variable]")
			self.isCompiling = False
			self.compileDataWord(0)
			return

		raise CompilerException("Unknown word '{0}'".format(word))
	#
	#	Generate dictionary and other system values.
	#
	def complete(self):
		loadAddress = self.compilerInfo.getLoadAddress()
		# empty memory word
		self.memory.writeWord(loadAddress,0)
		# absolute next free code byte address and page
		self.memory.writeWord(loadAddress+8,self.pointer)
		self.memory.writeByte(loadAddress+10,0)
		# screen size.
		self.memory.writeByte(loadAddress+14,self.getScreenSize()[0])
		self.memory.writeByte(loadAddress+15,self.getScreenSize()[1])
		# create dictionary.
		self.dictionary.generate(self.memory)
		print("Code block: Code to ${0:04x}".format(self.pointer))
	#
	#	Compiler helpers
	#
	def compileDataWord(self,word):
		self.memory.writeWord(self.pointer,word)
		self.pointer += 2
	def compileDataByte(self,byte):
		self.memory.writeByte(self.pointer,byte)
		self.pointer += 1
	#
	#	Compile code to generate a literal
	#
	def compileLiteral(self,value):
		if not self.isCompiling:
			raise CompilerException("Code outside a colon definition")
		if value >= 0 and value < 256:
			self.compileWord("[literal.s]")
			self.compileDataByte(int(value))
		else:
			self.compileWord("[literal]")
			self.compileDataWord(int(value))
	#
	#	Handle if..then do..until and for..next structures
	#
	def compileStructure(self,word):
		if word == "if":
			if self.ifLink is not None:
				raise CompilerException("Unclosed IF")
			self.compileWord("[br.zero]")
			self.ifLink = self.pointer
			self.pointer += 1
			return
		if word == "else":
			if self.ifLink is None:
				raise CompilerException("ELSE without IF")
			link = self.ifLink
			self.compileWord("[br]")
			self.ifLink = self.pointer
			self.pointer += 1
			self.memory.writeByte(link,(self.pointer-link-1) & 0xFF)
			return

		if word == "then":
			if self.ifLink is None:
				raise CompilerException("THEN without IF")
			self.memory.writeByte(self.ifLink,(self.pointer-self.ifLink-1) & 0xFF)
			self.ifLink = None
			return
		assert False,"not implemented "+word
	##
	#	screen dimensions.
	#
	def getScreenSize(self):
		return [24,24]

c = Compiler()
c.compileFile("group1.forth")
c.compileText(""" """.split("\n"))
c.complete()
c.memory.writeBinary("vmboot.bin")


# TODO: Editor.
# TODO: ELSE
