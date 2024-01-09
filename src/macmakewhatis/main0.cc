/* main (makewhatis) */
/* lang=C++20 */


/*-
 * Copyright (c) 2002 John Rochester
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer,
 *    in this position and unchanged.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. The name of the author may not be used to endorse or promote products
 *    derived from this software without specific prior written permission
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
 * NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/* revision history:

	+ 2024-01-08, David A.D. Morano
	Updated (converted to C++ from C. And also fixed a number
	of type mismatches and one of which turned out to be a bad
	error (bug).  I fixed the bug (you have find it in the code
	below). There was one suspcicious code section that I left
	alone, but it might be checked by someone who knows what it
	is supposed to do.  It is marked in the code below as 'TODO'.

*****/

/* Copyright © 2017 David A­D­ Morano.  All rights reserved. */

/*******************************************************************************

	This is an implementation of the famous |MAKEWHATIS| facility.
	Although a number of implementations are (or were) written in
	SHELL language (POSIX® compliant generally), this version is
	written in C/C++.  It was originally written in C-lang, but
	was converted to C++ by David Morano.

	For how to use this program, see the various MAN pages on it
	and some other documentation on-line.

*******************************************************************************/

#include <sys/cdefs.h>
__FBSDID("$FreeBSD: src/usr.bin/makewhatis/makewhatis.c,v 1.9 2002/09/04 23:29:04 dwmalone Exp $");

#include <sys/types.h>
#include <sys/stat.h>
#include <sys/param.h>
#include <sys/queue.h>

/* Workaround for missing #define in sys/queue.h (3806865) */
#ifndef SLIST_HEAD_INITIALIZER
#define SLIST_HEAD_INITIALIZER(head) { nullptr }
#endif

#include <unistd.h>
#include <dirent.h>
#include <fcntl.h>
#include <err.h>
#include <climits>			/* for |UCHAR_MAX| */
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cctype>
#include <stringlist.h>
#include <zlib.h>


/* local defines */

#define	noex			noexcept

#define DEFAULT_MANPATH		"/usr/share/man"
#define LINE_ALLOC		4096


#define	PAGE_INFO		struct page_info
#define	SBUF			struct sbuf

#ifndef	DIRENT
#define	DIRENT			struct dirent
#endif


/* local namespaces */

using std::nullptr_t ;


/* local typedefs */

typedef char *edited_copy(char *from,char *to,int length) noex ;
typedef const int	cint ;
typedef const char	cchar ;
typedef const void	cvoid ;
typedef const char *const *mainv ;


/* local structures */

/*
 * Information collected about each man page in a section.
 */
struct page_info {
	char *	filename;
	char *	name;
	char *	suffix;
	int	gzipped;
	ino_t	inode;
};

/*
 * An entry kept for each visited directory.
 */
struct visited_dir {
	dev_t		device;
	ino_t		inode;
	SLIST_ENTRY(visited_dir)	next;
};

/*
 * an expanding string
 */
struct sbuf {
	char *	content;		/* the start of the buffer */
	char *	end;			/* just past the end of the content */
	char *	last;			/* the last allocated character */
};

/*
 * Removes the last amount characters from the sbuf.
 */
#define sbuf_retract(sbuf, amount)	\
	((sbuf)->end -= (amount))
/*
 * Returns the length of the sbuf content.
 */
#define sbuf_length(sbuf)		\
	((sbuf)->end - (sbuf)->content)

static char blank[] = 		"";

static int append;			/* -a flag: append to existing whatis */
static int verbose;			/* -v flag: be verbose with warnings */
static int indent = 24;			/* -i option: description indentation */
static cchar *whatis_name="whatis";	/* -n option: the name */
static char *common_output;		/* -o option: the single output file */
static char *locale;			/* user's locale if -L is used */
static char *lang_locale;		/* short form of locale */
#ifndef __APPLE__
static cchar *machine;
#endif /* !__APPLE__ */

static int exit_code;			/* exit code to use when finished */
static SLIST_HEAD(, visited_dir) visited_dirs =
    SLIST_HEAD_INITIALIZER(visited_dirs);

/*
 * While the whatis line is being formed, it is stored in whatis_proto.
 * When finished, it is reformatted into whatis_final and then appended
 * to whatis_lines.
 */
static SBUF	*whatis_proto;
static SBUF	*whatis_final;
static StringList *whatis_lines;	/* collected output lines */

static char tmp_file[MAXPATHLEN];	/* path of temporary file, if any */
static char tmp_rel_file[MAXPATHLEN];

/* fix-up for (f*cked-up) BROKEN |getopt(3c)| -- DAM */
static int getoptv(int argc,mainv argv,cchar *optstr) noexcept {
	char *const *argfuck = (char *const *) argv ;
	return getopt(argc,argfuck,optstr) ;
}
/* end subroutine (getoptv) */

/* A set of possible names for the NAME man page section */
static cchar *name_section_titles[] = {
	"NAME", 
	"Name", 
	"NAMN", 
	"BEZEICHNUNG", 
	"\xcc\xbe\xbe\xce",
	"\xee\xe1\xfa\xf7\xe1\xee\xe9\xe5", 
	nullptr
};

/* A subset of the mdoc(7) commands to ignore */
static char mdoc_commands[] = "ArDvErEvFlLiNmPa";

/*
 * Frees a struct page_info and its content.
 */
static void free_page_info(PAGE_INFO *info) noex {
	free(info->filename);
	free(info->name);
	free(info->suffix);
	free(info);
}

/*
 * Allocates and fills in a new struct page_info given the
 * name of the man section directory and the dirent of the file.
 * If the file is not a man page, returns nullptr.
 */
static PAGE_INFO *new_page_info(char *dir,DIRENT *dirent) noex {
	PAGE_INFO	*info;
	int basename_length;
	char *suffix;
	struct stat st;

	info = (PAGE_INFO *) malloc(sizeof(PAGE_INFO)) ;
	if (info == nullptr)
		err(1, "malloc");
	basename_length = strlen(dirent->d_name);
	suffix = &dirent->d_name[basename_length];
	asprintf(&info->filename, "%s/%s", dir, dirent->d_name);
	if ((info->gzipped = basename_length >= 4 && 
		strcmp(&dirent->d_name[basename_length - 3],".gz") == 0)) {
		suffix -= 3;
		*suffix = '\0';
	}
	for (;;) {
		if (--suffix == dirent->d_name || !isalnum(*suffix)) {
			if (*suffix == '.')
				break;
			if (verbose)
				warnx("%s: invalid man page name", 
				info->filename);
			free(info->filename);
			free(info);
			return nullptr;
		}
	} /* end for */
	*suffix++ = '\0';
	info->name = strdup(dirent->d_name);
	info->suffix = strdup(suffix);
	if (stat(info->filename, &st) < 0) {
		warn("%s", info->filename);
		free_page_info(info);
		return nullptr;
	}
	if (!S_ISREG(st.st_mode)) {
		if (verbose && !S_ISDIR(st.st_mode))
			warnx("%s: not a regular file", info->filename);
		free_page_info(info);
		return nullptr;
	}
	info->inode = st.st_ino;
	return info;
}
/* end subroutine (new_page_info) */

/*
 * Reset an sbuf's length to 0.
 */
static void sbuf_clear(SBUF *sbuf) noex {
	sbuf->end = sbuf->content;
}

/*
 * Allocate a new sbuf.
 */
static struct sbuf * new_sbuf(void) noex {
	struct sbuf *sbuf = (struct sbuf *) malloc(sizeof(struct sbuf));
	sbuf->content = (char *) malloc(LINE_ALLOC);
	sbuf->last = sbuf->content + LINE_ALLOC - 1;
	sbuf_clear(sbuf);
	return sbuf;
}

/*
 * Ensure that there is enough room in the sbuf for nchars more characters.
 */
static void sbuf_need(SBUF *sbuf, int nchars) noex {
	char *new_content;
	size_t size, cntsize;

	/* double the size of the allocation until the buffer is big enough */
	while (sbuf->end + nchars > sbuf->last) {
		size = sbuf->last + 1 - sbuf->content;
		size *= 2;
		cntsize = sbuf->end - sbuf->content;

		new_content = (char *)malloc(size);
		memcpy(new_content, sbuf->content, cntsize);
		free(sbuf->content);
		sbuf->content = new_content;
		sbuf->end = new_content + cntsize;
		sbuf->last = new_content + size - 1;
	}
}

/*
 * Appends a string of a given length to the sbuf.
 */
static void sbuf_append(SBUF *sbuf, cchar *text, int length) noex {
	if (length > 0) {
		sbuf_need(sbuf, length);
		memcpy(sbuf->end, text, length);
		sbuf->end += length;
	}
}

/*
 * Appends a null-terminated string to the sbuf.
 */
static void sbuf_append_str(SBUF *sbuf, char *text) noex {
	sbuf_append(sbuf, text, strlen(text));
}

/*
 * Appends an edited null-terminated string to the sbuf.
 */
static void sbuf_append_edited(SBUF *sbuf,char *text,
		edited_copy copy) noex {
	int length = strlen(text);
	if (length > 0) {
		sbuf_need(sbuf, length);
		sbuf->end = copy(text, sbuf->end, length);
	}
}

/*
 * Strips any of a set of chars from the end of the sbuf.
 */
static void sbuf_strip(SBUF *sbuf, cchar *set) noex {
	nullptr_t	np{} ;
	while (sbuf->end > sbuf->content && strchr(set,sbuf->end[-1]) != np) {
		sbuf->end-- ;
	}
}

/*
 * Returns the null-terminated string built by the sbuf.
 */
static char * sbuf_content(SBUF *sbuf) noex {
	*sbuf->end = '\0';
	return sbuf->content;
}

/*
 * Returns true if no man page exists in the directory with
 * any of the names in the StringList.
 */
static int no_page_exists(char *dir,StringList *names,char *suffix) noex {
	char path[MAXPATHLEN];
	size_t i;

	for (i = 0; i < names->sl_cur; i++) {
		snprintf(path, sizeof path, 
			"%s/%s.%s.gz", dir, names->sl_str[i], suffix);
		if (access(path, F_OK) < 0) {
			path[strlen(path) - 3] = '\0';
			if (access(path, F_OK) < 0)
				continue;
		}
		return 0;
	}
	return 1;
}
/* end subroutine (no_page_exists) */

static void trap_signal(int sig __unused) noex {
	if (tmp_file[0] != '\0')
		unlink(tmp_file);
	exit(1);
}

/*
 * Attempts to open an output file.  Returns nullptr if unsuccessful.
 */
static FILE *open_output(char *name, int dir_fd,cchar *rel_name) noex {
	FILE *output;
	int output_fd;
	struct stat statbuf;

	whatis_lines = sl_init();
	if (append) {
		char line[LINE_ALLOC];

		output_fd = openat(dir_fd, rel_name, O_RDONLY);
		if (output_fd == -1) {
			warn("%s", name);
			exit_code = 1;
			return nullptr;
		}
		output = fdopen(output_fd, "r");
		if (output == nullptr) {
			warn("%s", name);
			exit_code = 1;
			return nullptr;
		}
		while (fgets(line, sizeof line, output) != nullptr) {
			line[strlen(line) - 1] = '\0';
			sl_add(whatis_lines, strdup(line));
		}
		fclose(output);
	}
	if (common_output == nullptr) {
		snprintf(tmp_file, sizeof tmp_file, "%s.tmp", name);
		snprintf(tmp_rel_file, sizeof tmp_rel_file, "%s.tmp", rel_name);
		name = tmp_file;
		rel_name = tmp_rel_file;
	}
	/* Bail out if the file is actually a symlink or has another link.
	 * This script can run as root and we don't want to risk writing
	 * into a random location.
	 * See rdar://problem/55280616
	 */
	const int of = (O_WRONLY | O_NOFOLLOW | O_CREAT | O_TRUNC) ;
	output_fd = openat(dir_fd, rel_name,of, 0644);
	if (output_fd == -1) {
		warn("%s", name);
		exit_code = 1;
		return nullptr;
	}
	if (fstat(output_fd, &statbuf) == -1) {
		warn("%s: unable to stat", name);
		close(output_fd);
		exit_code = 1;
		return nullptr;
	}
	if (statbuf.st_nlink > 1) {
		warnx("%s: is a hardlink", name);
		close(output_fd);
		exit_code = 1;
		return nullptr;
	}
	output = fdopen(output_fd, "w");
	if (output == nullptr) {
		warn("%s", name);
		exit_code = 1;
		return nullptr;
	}
	return output;
}
/* end subroutine (open_output) */

static int linesort(cvoid *a, cvoid *b) noex {
	return strcmp((*(cchar * const *)a), (*(cchar * const *)b));
}

/*
 * Writes the unique sorted lines to the output file.
 */
static void finish_output(FILE *output,char *,int dir_fd,cchar *rel_name) noex {
	const size_t	esz = sizeof(char *) ;
	size_t i;
	char *prev = nullptr;

	qsort(whatis_lines->sl_str, whatis_lines->sl_cur,esz,linesort);
	for (i = 0; i < whatis_lines->sl_cur; i++) {
		char *line = whatis_lines->sl_str[i];
		if (i > 0 && strcmp(line, prev) == 0)
			continue;
		prev = line;
		fputs(line, output);
		putc('\n', output);
	}
	fclose(output);
	sl_free(whatis_lines, 1);
	if (common_output == nullptr) {
		renameat(dir_fd, tmp_rel_file, dir_fd, rel_name);
		unlinkat(dir_fd, tmp_rel_file, 0);
	}
}
/* end subroutine (finish_output) */

static FILE * open_whatis(char *mandir, int mandir_fd) noexcept {
	char filename[MAXPATHLEN];

	snprintf(filename, sizeof filename, "%s/%s", mandir, whatis_name);
	return open_output(filename, mandir_fd, whatis_name);
}

static void finish_whatis(FILE *output, char *mandir, int mandir_fd) noexcept {
	char filename[MAXPATHLEN];

	snprintf(filename, sizeof filename, "%s/%s", mandir, whatis_name);
	finish_output(output, filename, mandir_fd, whatis_name);
}

/*
 * Tests to see if the given directory has already been visited.
 */
static int already_visited(char *dir) noexcept {
	struct stat st;
	struct visited_dir *visit;

	if (stat(dir, &st) < 0) {
		warn("%s", dir);
		exit_code = 1;
		return 1;
	}
	SLIST_FOREACH(visit, &visited_dirs, next) {
		if (visit->inode == st.st_ino &&
		    visit->device == st.st_dev) {
			warnx("already visited %s", dir);
			return 1;
		}
	}
	visit = (struct visited_dir *) malloc(sizeof(struct visited_dir));
	visit->device = st.st_dev;
	visit->inode = st.st_ino;
	SLIST_INSERT_HEAD(&visited_dirs, visit, next);
	return 0;
}

/*
 * Removes trailing spaces from a string, returning a pointer to just
 * beyond the new last character.
 */
static char * trim_rhs(char *str) noex {
	char *rhs = &str[strlen(str)];
	while (--rhs > str && isspace(*rhs)) ;
	*++rhs = '\0';
	return rhs;
}

/*
 * Returns a pointer to the next non-space character in the string.
 */
static char * skip_spaces(char *s) noex {
	while (*s != '\0' && isspace(*s))
		s++;
	return s;
}

/*
 * Returns whether the string contains only digits.
 */
static int only_digits(char *line) noexcept {
	if (!isdigit(*line++))
		return 0;
	while (isdigit(*line))
		line++;
	return *line == '\0';
}

/*
 * Returns whether the line is of one of the forms:
 *	.Sh NAME
 *	.Sh "NAME"
 *	etc.
 * assuming that section_start is ".Sh".
 */
static int name_section_line(char *line, cchar *section_start) noexcept {
	char *rhs;
	cchar **title;

	if (strncmp(line, section_start, 3) != 0)
		return 0;
	line = skip_spaces(line + 3);
	rhs = trim_rhs(line);
	if (*line == '"') {
		line++;
		if (*--rhs == '"')
			*rhs = '\0';
	}
	for (title = name_section_titles; *title != nullptr; title++)
		if (strcmp(*title, line) == 0)
			return 1;
	return 0;
}

/*
 * Copies characters while removing the most common nroff/troff
 * markup:
 *	\(em, \(mi, \s[+-N], \&
 *	\fF, \f(fo, \f[font]
 *	\*s, \*(st, \*[stringvar]
 */
static char * de_nroff_copy(char *from, char *to, int fromlen) noexcept {
	char *from_end = &from[fromlen];
	while (from < from_end) {
	    cint	ch = int(*from & UCHAR_MAX) ;
		switch (ch) {
		case '\\':
			switch (*++from) {
			case '(':
				if (strncmp(&from[1], "em", 2) == 0 ||
					strncmp(&from[1], "mi", 2) == 0) {
					from += 3;
					continue;
				}
				break;
			case 's':
				if (*++from == '-')
					from++;
				while (isdigit(*from)) {
					from++;
				}
				continue;
			case 'f':
			case '*':
				if (*++from == '(') {
					from += 3;
				} else if (*from == '[') {
				  /* TODO: is this correct? or dangerous? */
				  while (*++from != ']' && from < from_end) ;
				  from++;
				} else {
					from++;
				}
				continue;
			case '&':
				from++;
				continue;
			}
			break;
		} /* end switch */
		*to++ = *from++;
	} /* end while */
	return to;
}
/* end subroutine (de_nroff_copy) */

/*
 * Appends a string with the nroff formatting removed.
 */
static void add_nroff(char *text) noexcept {
	sbuf_append_edited(whatis_proto, text, de_nroff_copy);
}

/*
 * Appends "name(suffix), " to whatis_final.
 */
static void add_whatis_name(char *name, char *suffix) noexcept {
	if (*name != '\0') {
		sbuf_append_str(whatis_final, name);
		sbuf_append(whatis_final, "(", 1);
		sbuf_append_str(whatis_final, suffix);
		sbuf_append(whatis_final, "), ", 3);
	}
}

/*
 * Processes an old-style man(7) line.  This ignores commands with only
 * a single number argument.
 */
static void process_man_line(char *line) noexcept {
	if (*line == '.') {
		while (isalpha(*++line))
			;
		line = skip_spaces(line);
		if (only_digits(line))
			return;
	} else {
		line = skip_spaces(line);
	}

#ifdef __APPLE__
	/* 4454557 */
	if (*line == '"')
		++line;
#endif /* __APPLE__ */

	if (*line != '\0') {
		add_nroff(line);
		sbuf_append(whatis_proto, " ", 1);
	}
}
/* end subroutine (process_man_line) */

/*
 * Processes a new-style mdoc(7) line.
 */
static void process_mdoc_line(char *line) noexcept {
	int xref;
	int arg = 0;
	char *line_end = &line[strlen(line)];
	int orig_length = sbuf_length(whatis_proto);
	char *next;

	if (*line == '\0')
		return;
	if (line[0] != '.' || !isupper(line[1]) || !islower(line[2])) {
		add_nroff(skip_spaces(line));
		sbuf_append(whatis_proto, " ", 1);
		return;
	}
	xref = strncmp(line, ".Xr", 3) == 0;
	line += 3;
	while ((line = skip_spaces(line)) < line_end) {
		if (*line == '"') {
			next = ++line;
			for (;;) {
				next = strchr(next, '"');
				if (next == nullptr)
					break;
				memmove(next, next + 1, strlen(next));
				line_end--;
				if (*next != '"')
					break;
				next++;
			} /* end for */
		} else {
			next = strpbrk(line, " \t");
		}
		if (next != nullptr) {
			*next++ = '\0';
		} else {
			next = line_end;
		}
		if (isupper(*line) && islower(line[1]) && line[2] == '\0') {
			if (strcmp(line, "Ns") == 0) {
				arg = 0;
				line = next;
				continue;
			}
			if (strstr(mdoc_commands, line) != nullptr) {
				line = next;
				continue;
			}
		}
		if (arg > 0 && strchr(",.:;?!)]", *line) == 0) {
			if (xref) {
				sbuf_append(whatis_proto, "(", 1);
				add_nroff(line);
				sbuf_append(whatis_proto, ")", 1);
				xref = 0;
				line = blank;
			} else {
				sbuf_append(whatis_proto, " ", 1);
			}
		}
		add_nroff(line);
		arg++;
		line = next;
	} /* end while */
	if (sbuf_length(whatis_proto) > orig_length) {
		sbuf_append(whatis_proto, " ", 1);
	}
}

/*
 * Collects a list of comma-separated names from the text.
 */
static void collect_names(StringList *names, char *text) noexcept {
	char *arg;

	for (;;) {
		arg = text;
		text = strchr(text, ',');
		if (text != nullptr) {
			*text++ = '\0';
		}
		sl_add(names, arg);
		if (text == nullptr) return;
		if (*text == ' ') {
			text++;
		}
	} /* end for */
}
/* end subroutine (collect_names) */

enum { STATE_UNKNOWN, STATE_MANSTYLE, STATE_MDOCNAME, STATE_MDOCDESC };

/*
 * Processes a man page source into a single whatis line and adds it
 * to whatis_lines.
 */
static void process_page(PAGE_INFO *page, char *section_dir) noexcept {
	gzFile		in{} ;		/* <- fixed f*ck-up bug! */
	char buffer[4096];
	char *line;
	StringList *names;
	char *descr;
	int state = STATE_UNKNOWN;
	size_t i;

	sbuf_clear(whatis_proto);
	if ((in = gzopen(page->filename, "r")) == nullptr) {
		warn("%s", page->filename);
		exit_code = 1;
		return;
	}
	while (gzgets(in, buffer, sizeof buffer) != nullptr) {
		line = buffer;
		if (strncmp(line, ".\\\"", 3) == 0) /* ignore comments */
			continue;
		switch (state) {
		/*
		 * haven't reached the NAME section yet.
		 */
		case STATE_UNKNOWN:
			if (name_section_line(line, ".SH"))
				state = STATE_MANSTYLE;
			else if (name_section_line(line, ".Sh"))
				state = STATE_MDOCNAME;
			continue;
		/*
		 * Inside an old-style .SH NAME section.
		 */
		case STATE_MANSTYLE:
			if ((strncmp(line, ".SH", 3) == 0) || 
				(strncmp(line, ".SS", 3) == 0))
				break;
			trim_rhs(line);
			if (strcmp(line, ".") == 0)
				continue;
			if (strncmp(line, ".IX", 3) == 0) {
				line += 3;
				line = skip_spaces(line);
			}
			process_man_line(line);
			continue;
		/*
		 * Inside a new-style .Sh NAME section (the .Nm part).
		 */
		case STATE_MDOCNAME:
			trim_rhs(line);
			if (strncmp(line, ".Nm", 3) == 0) {
				process_mdoc_line(line);
				continue;
			} else {
				if (strcmp(line, ".") == 0)
					continue;
				sbuf_append(whatis_proto, "- ", 2);
				state = STATE_MDOCDESC;
			}
			/* fall through */
			/* FALLTHROUGH */
		/*
		 * Inside a new-style .Sh NAME section (after the .Nm-s).
		 */
		case STATE_MDOCDESC:
			if (strncmp(line, ".Sh", 3) == 0)
				break;
			trim_rhs(line);
			if (strcmp(line, ".") == 0)
				continue;
			process_mdoc_line(line);
			continue;
		}
		break;
	} /* end if */
	gzclose(in);
	sbuf_strip(whatis_proto, " \t.-");
	line = sbuf_content(whatis_proto);
	/*
	 * line now contains the appropriate data, but without
	 * the proper indentation or the section appended to each name.
	 */
	descr = strstr(line, " - ");
	if (descr == nullptr) {
		descr = strchr(line, ' ');
		if (descr == nullptr) {
			if (verbose)
				fprintf(stderr,
			"	ignoring junk description \"%s\"\n", 
				line);
			return;
		}
		*descr++ = '\0';
	} else {
		*descr = '\0';
		descr += 3;
	}
	names = sl_init();
	collect_names(names, line);
	sbuf_clear(whatis_final);
	if (!sl_find(names, page->name) && 
		no_page_exists(section_dir, names, page->suffix)) {
		/*
		 * Add the page name since that's the only thing that
		 * man(1) will find.
		 */
		add_whatis_name(page->name, page->suffix);
	}
	for (i = 0; i < names->sl_cur; i++) {
		add_whatis_name(names->sl_str[i], page->suffix);
	} /* end for */
	sl_free(names, 0);
	sbuf_retract(whatis_final, 2);		/* remove last ", " */
	while (sbuf_length(whatis_final) < indent)
		sbuf_append(whatis_final, " ", 1);
	sbuf_append(whatis_final, " - ", 3);
	sbuf_append_str(whatis_final, skip_spaces(descr));
	sl_add(whatis_lines, strdup(sbuf_content(whatis_final)));
}
/* end subroutine (process_page) */

/*
 * Sorts pages first by inode number, then by name.
 */
static int pagesort(cvoid *a, cvoid *b) noex {
	const PAGE_INFO	*p1 = *(PAGE_INFO * const *) a;
	const PAGE_INFO *p2 = *(PAGE_INFO * const *) b;
	if (p1->inode == p2->inode)
		return strcmp(p1->name, p2->name);
	return p1->inode - p2->inode;
}

/*
 * Processes a single man section.
 */
static void process_section(char *section_dir) noex {
	DIRENT		**entries;
	int nentries;
	PAGE_INFO	**pages;
	int npages = 0;
	int i;
	ino_t prev_inode = 0;

	if (verbose)
		fprintf(stderr, "  %s\n", section_dir);

	/*
	 * scan the man section directory for pages
	 */
	nentries = scandir(section_dir, &entries, nullptr, alphasort);
	if (nentries < 0) {
		warn("%s", section_dir);
		exit_code = 1;
		return;
	}
	/*
	 * collect information about man pages
	 */
	pages = (PAGE_INFO **) calloc(nentries, sizeof(PAGE_INFO *));
	for (i = 0; i < nentries; i++) {
		PAGE_INFO	*info = new_page_info(section_dir, entries[i]);
		if (info != nullptr)
			pages[npages++] = info;
		free(entries[i]);
	}
	free(entries);
	qsort(pages, npages, sizeof(PAGE_INFO *), pagesort);
	/*
	 * process each unique page
	 */
	for (i = 0; i < npages; i++) {
		PAGE_INFO	*page = pages[i];
		if (page->inode != prev_inode) {
			prev_inode = page->inode;
			if (verbose)
				fprintf(stderr, "	reading %s\n", page->filename);
			process_page(page, section_dir);
		} else if (verbose)
			fprintf(stderr, "	skipping %s, duplicate\n", page->filename);
		free_page_info(page);
	}
	free(pages);
}
/* end subroutine (process_section) */

/*
 * Returns whether the directory entry is a man page section.
 */
static int select_sections(DIRENT *entry) noexcept {
	char *p = &entry->d_name[3];

	if (strncmp(entry->d_name, "man", 3) != 0)
		return 0;
	while (*p != '\0') {
		if (!isalnum(*p++))
			return 0;
	}
	return 1;
}
/* end subroutine (select_sections) */

/*
 * Processes a single top-level man directory by finding all the
 * sub-directories named man* and processing each one in turn.
 */
static void process_mandir(char *dir_name) noexcept {
	int dir_fd;
	DIR *dir;
	FILE *fp = nullptr;
	DIRENT	*entry;
	/* struct stat st; */			/* <- was not used! */

	if (already_visited(dir_name))
		return;
	if (verbose)
		fprintf(stderr, "man directory %s\n", dir_name);

	dir_fd = open(dir_name, O_RDONLY | O_DIRECTORY | O_NOFOLLOW);
	if (dir_fd == -1) {
		warn("%s", dir_name);
		exit_code = 1;
		return;
	}
	dir = fdopendir(dir_fd);
	if (dir == nullptr) {
		warn("%s", dir_name);
		close(dir_fd);
		exit_code = 1;
		return;
	}
	if (common_output == nullptr && 
			(fp = open_whatis(dir_name, dir_fd)) == nullptr) {
		closedir(dir);
		return;
	}
	while ((entry = readdir(dir)) != nullptr) {
		char section_dir[MAXPATHLEN];
		if (select_sections(entry) == 0)
			continue;
		snprintf(section_dir, sizeof section_dir, 
			"%s/%s", dir_name, entry->d_name);
		process_section(section_dir);
#ifndef __APPLE__
		snprintf(section_dir, sizeof section_dir, 
			"%s/%s/%s", dir_name,
		    entry->d_name, machine);
		if (stat(section_dir, &st) == 0 && S_ISDIR(st.st_mode))
			process_section(section_dir);
#endif /* !__APPLE__ */
	} /* end while */
	if (common_output == nullptr)
		finish_whatis(fp, dir_name, dir_fd);
	closedir(dir);
}
/* end subroutine (process_mandir) */

/*
 * Processes one argument, which may be a colon-separated list of
 * directories.
 */
static void process_argument(cchar *arg) noexcept {
	char *dir;
	char *mandir;
	char *parg;

	parg = strdup(arg);
	if (parg == nullptr)
		err(1, "out of memory");
	while ((dir = strsep(&parg, ":")) != nullptr) {
		if (locale != nullptr) {
			asprintf(&mandir, "%s/%s", dir, locale);
			process_mandir(mandir);
			free(mandir);
			if (lang_locale != nullptr) {
				asprintf(&mandir, "%s/%s", dir, lang_locale);
				process_mandir(mandir);
				free(mandir);
			}
		} else {
			process_mandir(dir);
		}
	} /* end while */
	free(parg);
}
/* end subroutine (process_argument) */

int main(int argc,mainv argv,mainv) noexcept {
	int opt;
	FILE *fp = nullptr;

	while ((opt = getoptv(argc,argv,"ai:n:o:vL")) != -1) {
		switch (opt) {
		case 'a':
			append++;
			break;
		case 'i':
			indent = atoi(optarg);
			break;
		case 'n':
			whatis_name = optarg;
			break;
		case 'o':
			common_output = optarg;
			break;
		case 'v':
			verbose++;
			break;
		case 'L':
			locale = getenv("LC_ALL");
			if (locale == nullptr)
				locale = getenv("LC_CTYPE");
			if (locale == nullptr)
				locale = getenv("LANG");
			if (locale != nullptr) {
			    char *sep = strchr(locale, '_');
			    if (sep != nullptr && isupper(sep[1]) &&
				    isupper(sep[2])) {
				cint	fl = int(sep - locale) ;
				cchar *fmt = "%.*s%s" ;
				asprintf(&lang_locale,fmt,
					fl, locale, &sep[3]);
			    } /* end if */
			} /* end if */
			break;
		default:
			fprintf(stderr, "usage: %s [-a] [-i indent] [-n name] [-o output_file] [-v] [-L] [directories...]\n", argv[0]);
			exit(1);
		}
	}

	signal(SIGINT, trap_signal);
	signal(SIGHUP, trap_signal);
	signal(SIGQUIT, trap_signal);
	signal(SIGTERM, trap_signal);
	SLIST_INIT(&visited_dirs);
	whatis_proto = new_sbuf();
	whatis_final = new_sbuf();

#ifndef __APPLE__
	if ((machine = getenv("MACHINE")) == nullptr)
		machine = MACHINE;
#endif /* !__APPLE__ */

	if (common_output != nullptr && (fp = open_output(common_output, AT_FDCWD, common_output)) == nullptr)
		err(1, "%s", common_output);
	if (optind == argc) {
		cchar *manpath = getenv("MANPATH");
		if (manpath == nullptr)
			manpath = DEFAULT_MANPATH;
		process_argument(manpath);
	} else {
		while (optind < argc)
			process_argument(argv[optind++]);
	}
	if (common_output != nullptr) {
		finish_output(fp, common_output, AT_FDCWD, common_output);
	}

	return exit_code ;
}
/* end subroutine (main) */


