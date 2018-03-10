hr: hr.c main.c
	cl /W3 /D_CRT_SECURE_NO_WARNINGS /Ot /Ox /TC hr.c main.c /link /OUT:hr.exe
