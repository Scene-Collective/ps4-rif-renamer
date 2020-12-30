#include "ps4.h"

int rename_rifs(const char *license_path) {
  DIR *dir;
  struct dirent *dp;
  dir = opendir(license_path);

  if (dir) {
    char original_filename[255];
    char new_filename[255];
    while ((dp = readdir(dir)) != NULL) {
      struct stat info;
      sprintf(original_filename, "%s/%s", license_path, dp->d_name);

      if (stat(original_filename, &info) == -1) {
        // Failed to stat object... skip it
        continue;
      }

      // Rename if it's a file or symbolic link and starts with "fake"
      if ((S_ISREG(info.st_mode) || S_ISLNK(info.st_mode)) && strncmp("fake", dp->d_name, strlen("fake")) == 0) {
        char *modified_filename;

        modified_filename = malloc(strlen(dp->d_name) + 1);
        strcpy(modified_filename, dp->d_name);

        modified_filename[1] = 0x72; // r
        modified_filename[2] = 0x65; // e
        modified_filename[3] = 0x65; // e

        sprintf(new_filename, "%s/%s", license_path, modified_filename);
        rename(original_filename, new_filename);
        free(modified_filename);
      }
    }
    closedir(dir);
  } else {
    return -1;
  }
  return 0;
}

int _main(struct thread *td) {
  UNUSED(td);

  initKernel();
  initLibc();

  jailbreak();

  initSysUtil();

  rename_rifs("/system_data/priv/license");
  rename_rifs("/user/license");

  printf_notification("RIFs renamed!");

  return 0;
}
