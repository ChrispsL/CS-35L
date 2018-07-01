#!/usr/bin/python

import argparse, string, random, sys
from optparse import OptionParser

def isInt(i):
	try:
		int(i)
		return True
	except ValueError:
		return False

def displayHelp():
	usage_msg="""Usage: {0} [OPTION]... FILE
   or: {0} -e [OPTION]... [ARG]...
Write a random permutation of the input lines to standard output.


With no FILE, or when FILE is -, read standard input.

Mandatory arguments to long options are mandatory for short options too.
  -e, --echo                treat each ARG as an input line
  -n, --head-count=COUNT    output at most COUNT lines
  -r, --repeat              output lines can be repeated
      --help     display this help and exit""".format(__file__)
	print(usage_msg)

class shufline:
	def __init__(self, inputType, inputLines):
		if inputType==1: # inputted a file
			try:
				f = open(inputLines, 'r')
			except IOError:
				print("Could not open file: ", inputLines)
				sys.exit()
			with f:
				self.lines = f.readlines()
				f.close()

		if inputType==2: # inputted from echo
			self.lines = []
			for s in inputLines:
				self.lines.append(s)
		if inputType==3: # no input
			self.lines = sys.stdin.read().splitlines()

	def printShuffled(self, numlines, repeat):
		while (self.lines and not numlines == 0):
			try:
				index = random.choice(range(0, len(self.lines)))
				print(self.lines[index])
				if(not repeat): del self.lines[index]
				numlines -= 1
			except KeyboardInterrupt:
				sys.exit()

def main():
	ver_msg="{0} 1.0".format(__file__)
	parser = argparse.ArgumentParser(add_help=False, description="Write a random permutation of the input lines to standard output.")
	parser.add_argument('filename', nargs='?')
	parser.add_argument('-e', '--echo', nargs='*', help="treat each ARG as an input line\n")
	parser.add_argument('-n', '--head-count', nargs=1, dest="numlines", help="output at most COUNT lines\n")
	parser.add_argument('-r', '--repeat', action="store_true", help="output lines can be repeated\n")
	parser.add_argument('--help', action="store_true")
	args = parser.parse_args()

	if (args.help is True):
		displayHelp()
		sys.exit()

	if (args.numlines):
		if (not isInt(args.numlines[0]) or int(args.numlines[0])<0):
			print("invalid line count: {0}".format(args.numlines[0]))
			sys.exit()
		else:
			numlines = int(args.numlines[0])
	else:
		numlines = -1	# allow for unbounded if no count is set

	if (args.filename=='-'):
		shuffler = shufline(3, 0)
	elif(args.filename):
		shuffler = shufline(1, args.filename)
	elif (args.echo):
		shuffler = shufline(2, args.echo)
	else:
		shuffler = shufline(3, 0)

	shuffler.printShuffled(numlines, args.repeat)

	#print(shuffler.lines)
	#print(numlines)

		#parser.error("invalid line count: %s" % format(args.numlines))

if __name__ == "__main__":
	main()
