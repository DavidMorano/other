UTMPLINE

This program prints out the UTMP "line" field for the UTMP entry
that the process session might be registerd with.  If the process
is part of a logged-in session, then it should (normally) have an
entry in the system UTMP database. Note that there is no srict rule
that terminal sessions must have entries in the system UTMP database;
so processes cannot rely on this always being the case.  Processes
can indeed belong to a terminal sesson without that session being
registered in the system UTMP database. But if the terminal session
is registered, then the process-session-ID of the terminal session
should be registerd within one (and hopefully only one) UTMP database
entry in the "pid" field of that UTMP entry. But the probless arises
when programs that setup terminal sessions do not register the
terminal session-ID in the UTMP entry, but rather some pseudo-random
PID like their own PID instead of the terminal-session-ID. This
occurs rampantly on GNU-Linux based systems. I mean really! When
a mistake like this occurs (with a bad program that sets up a
terminal session), there is no way for a process within the session
to find its associated session entry in the UTMP databases by
using its session-ID (SID) alone -- since it is not registered
in the database to begin with. A process may have to do this
because its standard-input file-descriptor is not associated with
the terminal but rather some other file. In this latter case, the
process can read out the environment variable 'UTMPLINE' and then
lookup that 'line' in the system UTMP database to find its entry.
That is what this program us used for: to print out and make
available the UTMP 'line' field for use by child processes which
have no other way to determine their UTMP entries (or even 
whether they are logged in or not).

Synopsis:
$ utmpline

Arguments:
-

Returns:
-	the UTMP "line" field that the process session might be registered with
	or nothing otherwise (no registration)

