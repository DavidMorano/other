MACUSER

This is an old version of this little program that will still compile
on the oldest (brain-damaged) systems!

I refactored and enhanced much of this code in August 2007 because
some dumb systems (MacOS Darwin 10.3.x) are too brain-damaged to
compile modern programs.  This is because MacOS (a.k.a. Apple-Darwin)
does not support either the full or a moderately enhanced POSIX®
development (compilation) environment. I think that the Mac
is supposed to be getting better at POSIX®, but it is a "process."

Names:
username
userhome
usershell
groupname
uid
gid
sid
shells

Arguments:
-		NONE, because we are too lean even for that

Returns:
EXIT_SUCCESS	(numeric 0) successfully processed the user
EXIT_FAILURE	(numeric 1) one of many errors occurred

