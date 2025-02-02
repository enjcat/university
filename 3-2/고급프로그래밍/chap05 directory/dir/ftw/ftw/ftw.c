
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <limits.h>
#include <errno.h>

#define	PATH_MAX_GUESS	1024	/* if PATH_MAX is indeterminate */

/* function type that's called for each filename */
typedef	int Myfunc(const char *, const struct stat *, int);

#ifdef	PATH_MAX
static int pathmax = PATH_MAX;
#else
static int pathmax = 0;
#endif

static long	nreg, ndir, nblk, nchr, nfifo, nslink, nsock, ntot;

/* Function Prototypes */
static Myfunc	myfunc;
static int	myftw(char *, Myfunc *);
static int	dopath(Myfunc *);
static char *path_alloc (int *size);

int main (int argc, char *argv[])
{
  int ret;

  if (argc != 2) {
    printf ("usage:  ftw  <starting-pathname>\n");
    exit (0);
  }

  ret = myftw(argv[1], myfunc);	/* does it all */

  if ( (ntot = nreg + ndir + nblk + nchr + nfifo + nslink + nsock) == 0)
    ntot = 1;	       /* avoid divide by 0; print 0 for all counts */

  printf("regular files  = %7ld, %5.2f %%\n", nreg,  nreg*100.0/ntot);
  printf("directories    = %7ld, %5.2f %%\n", ndir,  ndir*100.0/ntot);
  printf("block special  = %7ld, %5.2f %%\n", nblk,  nblk*100.0/ntot);
  printf("char special   = %7ld, %5.2f %%\n", nchr,  nchr*100.0/ntot);
  printf("FIFOs          = %7ld, %5.2f %%\n", nfifo, nfifo*100.0/ntot);
  printf("symbolic links = %7ld, %5.2f %%\n", nslink,nslink*100.0/ntot);
  printf("sockets        = %7ld, %5.2f %%\n", nsock, nsock*100.0/ntot);

  exit(ret);
}

/*
 * Descend through the hierarchy, starting at "pathname".
 * The caller's func() is called for every file.
 */

#define	FTW_F	1		/* file other than directory */
#define	FTW_D	2		/* directory */
#define	FTW_DNR	3		/* directory that can't be read */
#define	FTW_NS	4		/* file that we can't stat */

static char *fullpath; /* contains full pathname for every file */
char date[64];

/* we return whatever func() returns */
static int myftw (char *pathname, Myfunc *func) 
{
  fullpath = path_alloc(NULL);	/* malloc's for PATH_MAX+1 bytes */
				/* ({Prog pathalloc}) */
  strcpy (fullpath, pathname);	/* initialize fullpath */

  return (dopath(func));
}

/*
 * Descend through the hierarchy, starting at "fullpath".
 * If "fullpath" is anything other than a directory, we lstat() it,
 * call func(), and return.  For a directory, we call ourself
 * recursively for each name in the directory.
 */

/* we return whatever func() returns */
static int dopath (Myfunc* func)
{
  struct stat statbuf;
  struct dirent	*dirp;
  DIR	*dp;
  int	ret;
  char	*ptr;
  char *ymd;

  if (stat (fullpath, &statbuf) < 0)
    return (func(fullpath, &statbuf, FTW_NS));	/* stat error */

  if (S_ISDIR(statbuf.st_mode) == 0)
    return (func(fullpath, &statbuf, FTW_F));	/* not a directory */

  /*
   * It's a directory.  First call func() for the directory,
   * then process each filename in the directory.
   */

  if ((ret = func(fullpath, &statbuf, FTW_D)) != 0)
    return (ret);

  ptr = fullpath + strlen(fullpath);	/* point to end of fullpath */
  *ptr++ = '/';
  *ptr = 0;
  ymd = date + strlen (date);
  
  if ((dp = opendir(fullpath)) == NULL)
    return (func(fullpath, &statbuf, FTW_DNR)); /* can't read directory */

  while ((dirp = readdir(dp)) != NULL) {
    if (strcmp(dirp->d_name, ".") == 0  || strcmp(dirp->d_name, "..") == 0)
      continue; /* ignore dot and dot-dot */

    strcpy (ptr, dirp->d_name); /* append name after slash */
    strcpy (ymd, dirp->d_name);

    if ((ret = dopath(func)) != 0) /* recursive */
      break;	/* time to leave */
  }
  ptr[-1] = 0;	/* erase everything from slash onwards */
  ymd[-1] = 0;

  if (closedir(dp) < 0)
    printf ("can't close directory %s\n", fullpath);

  return (ret);
}


static int myfunc (const char *pathname, const struct stat *statptr, int type)
{
  switch (type) {
  case FTW_F:
    switch (statptr->st_mode & S_IFMT) {
    case S_IFREG:  nreg++;   break;
    case S_IFBLK:  nblk++;   break;
    case S_IFCHR:  nchr++;   break;
    case S_IFIFO:  nfifo++;  break;
    case S_IFLNK:  nslink++; break;
    case S_IFSOCK: nsock++;  break;
    case S_IFDIR:  printf ("for S_IFDIR for %s", pathname); /* directories should have type = FTW_D */
    }
    break;

  case FTW_D:
    ndir++;
    if (strlen (date) == 8) {
      printf ("date for this file %s\n", date);
      printf ("fullpath for this dir %s\n", pathname);
    }
    break;
    
  case FTW_DNR:
    printf ("can't read directory %s\n", pathname);
    break;

  case FTW_NS:
    printf ("stat error for %s\n", pathname);
    break;

  default:
    printf ("unknown type %d for pathname %s\n", type, pathname);
  }

  return(0);
}


/* we're not guaranteed this is adequate */
/* also return allocated size, if nonnull */

static char *path_alloc (int *size)
{
  char	*ptr;

  if (pathmax == 0) {		/* first time through */
    errno = 0;
    if ((pathmax = pathconf("/", _PC_PATH_MAX)) < 0) {
      if (errno == 0)
	pathmax = PATH_MAX_GUESS; /* it's indeterminate */
      else
	printf ("pathconf error for _PC_PATH_MAX\n");
    }
    else
      pathmax++;	     /* add one since it's relative to root */
  }

  if ((ptr = malloc(pathmax + 1)) == NULL)
    printf ("malloc error for pathname\n");

  if (size != NULL)
    *size = pathmax + 1;

  return (ptr);
}
