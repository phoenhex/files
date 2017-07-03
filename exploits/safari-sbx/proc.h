// http://stackoverflow.com/questions/18820199/unable-to-detect-application-running-with-another-user-via-switch-user/18821357#18821357
typedef struct kinfo_proc kinfo_proc;
struct proc_list {
  kinfo_proc *procList;
  size_t procCount;
} proc_list;

int get_process_list() {
  int                 err;
  kinfo_proc *        result;
  bool                done;
  static const int    name[] = { CTL_KERN, KERN_PROC, KERN_PROC_ALL, 0 };
  size_t              length;

  proc_list.procCount = 0;

  result = NULL;
  done = false;
  do {
    assert(result == NULL);

    length = 0;
    err = sysctl( (int *) name, (sizeof(name) / sizeof(*name)) - 1,
                  NULL, &length,
                  NULL, 0);
    if (err == -1) {
        err = errno;
    }

    if (err == 0) {
      result = malloc(length);
      if (result == NULL) {
        err = ENOMEM;
      }
    }

    if (err == 0) {
      err = sysctl( (int *) name, (sizeof(name) / sizeof(*name)) - 1,
                    result, &length,
                    NULL, 0);
      if (err == -1) {
        err = errno;
      }
      if (err == 0) {
        done = true;
      } else if (err == ENOMEM) {
        assert(result != NULL);
        free(result);
        result = NULL;
        err = 0;
      }
    }
  } while (err == 0 && ! done);

  if (err != 0 && result != NULL) {
    free(result);
    result = NULL;
  }
  proc_list.procList = result;
  if (err == 0) {
    proc_list.procCount = length / sizeof(kinfo_proc);
  }
  assert( (err == 0) == (proc_list.procList != NULL) );
  return err;
}

bool have_pid(pid_t pid) {
  for (size_t i = 0; i < proc_list.procCount; ++i)
    if (proc_list.procList[i].kp_proc.p_pid == pid)
      return 1;
  return 0;
}

pid_t pid_for_proc(const char* name) {
  assert(!get_process_list());
  for (size_t i = 0; i < proc_list.procCount; ++i) {
    if (!strncmp(proc_list.procList[i].kp_proc.p_comm, name, strlen(name)))
      return proc_list.procList[i].kp_proc.p_pid;
  }
  return 0;
}
