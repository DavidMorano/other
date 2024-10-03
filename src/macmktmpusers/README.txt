MACMKTMPUSERS

Two functions here.

The MKTMPUSERS function is used (near system startup) to create the
directory:
	/tmp/users/
It is created with mode 0777+t with the user-ID of the invoking caller
(normally the user 'extra').  Once this directory is created and
exists, other programs or functions will place (usually) symbolic
links in that directory for a temporary directory for each 
individual user (see |tmpuserdir(1kf)|).

The TMPMOUNTS function is meant to be invoked by individual users
to (as needed) create the directory 'var' under their HOME directory,
and to created symbolic links under there to three specialized
temporary directories (often maintained by the operating system
itself).  The three links created in that directory are:
	public	used for making temporary files available to the public
	tmp	used for per-used only temporary files
	cache	used for storing semipersistent cache files per-user

The TMPUSERDIR function mounts (links) the user-specific temporary
directory at the mount (link) point:
	/tmp/users/<user>
where <user> is the user-specific "tmp" directory provided by the 
operating system (on a per user basis).

Synopsis:
$ mktmpusers
$ tmpusers
$ tmpmounts
$ tmpuserdir

Arguments:
-		NONE, because we are too lean even for that

Returns:
0		OK
>0		errors (see |sysexits| or |exitcodes|)

