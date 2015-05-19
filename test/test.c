/*
	This test program is the _gethtent implementation from bionic's
	libc/dns/net/getaddrinfo.c, augmented with a binary search if the
	correct header is found.

	This is only here to test locally that the implementation is correct.
	Speed comparisons should be done in a real-world situation on the
	phone.
*/
#include <stdio.h>
#include <string.h>
#define LFHFF_IDENTIFIER "#LFHFFv1\n"

int main(int argc, char *argv[]) {
	FILE *hostfr = fopen("hosts", "r");
	FILE **hostf = &hostfr;
	char hostbuf[8*1024];
	char *p, *cp, *tname, *addr, *cname;

	if(!hostfr) {
		printf("Place a hosts file in the current directory please\n");
		return;
	}
	if(argc != 2) {
		printf("Supply a host to lookup in argv[1] please\n");
		return;
	}
	const char *name = argv[1];

	int check_order = 1, is_ordered = 0, omax, omin, opivot, opivot_down = -1, ocmp;

 again:
	if (is_ordered) {
		int diff = opivot > 255 ? 255 : opivot;
		int eol;
		fseek(*hostf, opivot - diff, SEEK_SET);
		fread(hostbuf, diff, 1, *hostf);
		for(p = &hostbuf[diff]; *p != '\n' && p != hostbuf; p--);
		if(p == hostbuf)
			return;
		p++;
		if(opivot_down < 0)
			opivot_down = opivot - diff + (p - hostbuf);
		if(!fgets(hostbuf + diff, sizeof(hostbuf) - (p - hostbuf), *hostf))
			return;
		opivot = ftell(*hostf);
	}
	else
	if (!(p = fgets(hostbuf, sizeof hostbuf, *hostf)))
		return;
	if (check_order) {
		is_ordered = (strcmp(p, LFHFF_IDENTIFIER) == 0);
		check_order = 0;
		if (is_ordered) {
			fseek(*hostf, 0L, SEEK_END);
			omax = ftell(*hostf);
			omin = 0;
			opivot = (omax + omin) / 2;
			goto again;
		}
	}
	if (*p == '#')
		goto again;
	if (!(cp = strpbrk(p, "#\n")))
		goto again;
	*cp = '\0';
	if (!(cp = strpbrk(p, " \t")))
		goto again;
	*cp++ = '\0';
	addr = p;
	/* if this is not something we're looking for, skip it. */
	cname = NULL;
	while (cp && *cp) {
		if (*cp == ' ' || *cp == '\t') {
			cp++;
			continue;
		}
		if (!cname)
			cname = cp;
		tname = cp;
		if ((cp = strpbrk(cp, " \t")) != NULL)
			*cp++ = '\0';
//		fprintf(stderr, "\ttname = '%s'", tname);
		if ((ocmp = strcasecmp(name, tname)) == 0)
			goto found;
	}
	if (is_ordered) {
//		fprintf(stderr, "omin = %d, omax = %d, opivot = %d, opivot_down: %d: %s\n", omin, omax, opivot, opivot_down, tname);
		if (opivot_down == omax)
			return (NULL);
		if (omax == omin)
			return (NULL);
		if (ocmp < 0) {
			omax = opivot_down;
		}
		else
			omin = opivot;
		opivot = (omax + omin) / 2;
		opivot_down = -1;
	}
	goto again;

found:
	printf("found addr=%s, name=%s\n", addr, cname);
}
