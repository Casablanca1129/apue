#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <pwd.h>
#include <grp.h>
#include <time.h>

// 函数用于将文件类型和权限位转换为字符串形式
const char *get_file_type(mode_t mode) {
    if (S_ISREG(mode)) return "-";
    if (S_ISDIR(mode)) return "d";
    if (S_ISLNK(mode)) return "l";
    return "?";
}

// 函数用于将权限位转换为字符串形式
const char *get_permissions(mode_t mode) {
    static char perms[10];
    snprintf(perms, sizeof(perms), "%c%c%c%c%c%c%c%c%c",
        (mode & S_IRUSR) ? 'r' : '-',
        (mode & S_IWUSR) ? 'w' : '-',
        (mode & S_IXUSR) ? 'x' : '-',
        (mode & S_IRGRP) ? 'r' : '-',
        (mode & S_IWGRP) ? 'w' : '-',
        (mode & S_IXGRP) ? 'x' : '-',
        (mode & S_IROTH) ? 'r' : '-',
        (mode & S_IWOTH) ? 'w' : '-',
        (mode & S_IXOTH) ? 'x' : '-');
    return perms;
}

// 函数用于将时间戳转换为字符串形式
const char *get_time_str(time_t timestamp) {
    static char buffer[80];
    struct tm *tm_info;

    tm_info = localtime(&timestamp);
    strftime(buffer, sizeof(buffer), "%b %d %H:%M", tm_info);

    return buffer;
}

int main(int argc, char *argv[]) {
    const char *dirname;

    if (argc == 1) {
        dirname = ".";
    } else if (argc == 2) {
        dirname = argv[1];
    } else {
        fprintf(stderr, "Usage: %s [directory]\n", argv[0]);
        exit(1);
    }

    DIR *dir;
    struct dirent *entry;
    struct stat statbuf;
    struct passwd *pwd;
    struct group *grp;

    if ((dir = opendir(dirname)) == NULL) {
        perror("Error opening directory");
        exit(1);
    }

    while ((entry = readdir(dir)) != NULL) {
        char path[256];
        snprintf(path, sizeof(path), "%s/%s", dirname, entry->d_name);

        if (stat(path, &statbuf) == -1) {
            perror("Error getting file information");
            continue;
        }

        pwd = getpwuid(statbuf.st_uid);
        grp = getgrgid(statbuf.st_gid);

        printf("%s%s %ld %s %s %5ld %s %s\n",
            get_file_type(statbuf.st_mode),
            get_permissions(statbuf.st_mode),
            (long)statbuf.st_nlink,
            pwd->pw_name,
            grp->gr_name,
            (long)statbuf.st_size,
            get_time_str(statbuf.st_mtime),
            entry->d_name);
    }

    closedir(dir);

    return 0;
}

