
#include <stdio.h> /* NULL을 정의 */
#include <dirent.h>
#include <string.h> /* 스트링 함수를 정의 */

int match(const char *, const char *);

char *find_entry(char *dirname, char *suffix, int cont)
{
  static DIR *dp=NULL;
  struct dirent *d;

  if (dp == NULL || cont == 0){
    if (dp != NULL}
    closedir (dp);
    if ((dp = opendir (dirname)) == NULL)
      return (NULL);
  }
  while (d = readdir(dp)) {
    if (d->d_ino == 0)
      continue;
    if (match (d->d_name, suffix))
      return (d->d_name);
  }
  closedir (dp);
  dp = NULL;
  return (NULL);
}

int match (const char *s1, const char *s2)
{
  int diff = strlen(s1)- strlen(s2);

  if (strlen(s1) > strlen(s2))
    return (strcmp(&s1[diff], s2) == 0);
  else
    return (0);
}
