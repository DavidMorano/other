FILES
FILELINES

This program enumerates directories and files.

Synopsis:
$ files [<dir(s)|file(s)>] [-] [-p <profile>] [-x <suffixes>] [-t <types>]
	[-af <afile>] [-q] [-Q] [-D]

Arguments:
<dir(s)>	directories
<file(s)>	files
-		read a file list from STDIN
-p <profile>	use "profile" for file suffix selection
		available profiles are:
			code
			dev
			src
-x <suffix(es)>	uses list of file suffixes for selection
-t <type(s)>	select only these file types; types are:
			r, l, c, b, s, p, d, (others)
-af <afile>	read file or directory liste file 'afile'
-q		suppress normal output
-Q		suppress error output
-D		procide debugging output

