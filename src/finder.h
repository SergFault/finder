#ifndef FINDER_FINDER_H
#define FINDER_FINDER_H

/* send to stderr specific [opendir] errors messages except EACCES*/
void print_error();
/* concatenate two paths with by separator */
char *concat_path(const char *path_preffix, const char *path_postfix);

/* recursively iterates trough directories and print files names */
int list_files(const char *dir_path);

/* preferred file separator */
# define SYSTEM_SEPARATOR "/"

#endif //FINDER_FINDER_H
