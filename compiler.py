import sys
import os, os.path
import argparse

def main(args):	
	"""
	Adds all files to command line for c compile
	prefix for for folder to search for c files in
	delExe deletes the old executable
	"""
	if args.remove_exe:
		print(f"Removing old exe: {args.remove_exe}")
		os.system("rm " + args.remove_exe)
	#Gather all c files to compile on cmd line
	cFiles = getCppFiles(args.dir, args.exceptions)
	command = args.compiler + " ".join(
		[str(opt) for opt in [*args.options]]) + " " + args.file + " " + \
		 " ".join([str(file) for file in [*cFiles]])

	print(f"\n\n===RUNNING COMMAND===\n\n{command}\n\n")
	os.system(command)

def getCppFiles(prefix, exceptions):
	cppFiles = []
	for root, dirs, files in os.walk(prefix):
		for file in files:
			if file.startswith(tuple(exceptions)):
				pass
			elif file.endswith(".c") or file.endswith(".cpp"):
				#print(os.path.join(root, file))
				cppFiles.append(os.path.join(root, file))
			else:
				pass
	return cppFiles

if __name__ == "__main__":
	parser = argparse.ArgumentParser(
		description="Recursive compiler for c/c++")
	parser.add_argument("-f", "--file", type=str,
		help="Base file to compile")
	parser.add_argument("-d", "--dir", type=str,
		help="Folder to search for c/cpp files in")
	parser.add_argument("-c", "--compiler", type=str, default="cl",
		help="Compiler to use for the compile")
	parser.add_argument("-r", "--remove-exe",
	 type=str,	 default=False,
	  help="Remove the old exe before compiling")
	parser.add_argument("-e", "--exceptions", type=str,
		nargs="*", help="Exceptions to avoid compiling")
	parser.add_argument("-o", "--options", type=str,
		nargs="*", help="Flags to compile with")
	args, unknown = parser.parse_known_args()
	args.options += unknown
	main(args)