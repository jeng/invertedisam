OBJ	= eprintf.c stop.c document.c posting.c fileutils.c words.c settings.c

bin_PROGRAMS = buildInvertedIsam searchInvertedIsam
buildInvertedIsam_SOURCES = buildInvertedIsam.c $(OBJ)
searchInvertedIsam_SOURCES = searchInvertedIsam.c $(OBJ)

noinst_PROGRAMS = stop_test document_manager_test posting_test words_test
stop_test_SOURCES = stop_test.c $(OBJ)
document_manager_test_SOURCES = document_manager_test.c $(OBJ)
posting_test_SOURCES = posting_test.c $(OBJ)
words_test_SOURCES = words_test.c $(OBJ)
